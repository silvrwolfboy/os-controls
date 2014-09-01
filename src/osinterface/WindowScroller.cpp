#include "stdafx.h"
#include "WindowScroller.h"

IWindowScroller::IWindowScroller(void):
  m_virtualPosition(OSPointZero),
  m_remainingMomentum(OSPointZero),
  m_scrollPartialPixel(OSPointZero),
  m_scrollPartialLine(OSPointZero),
  m_pixelsPerLine(OSPointMake(1.0f, 1.0f)),
  m_velocitiesCount(0),
  m_velocitiesIndex(0),
  m_lastScrollTimePoint(std::chrono::steady_clock::now())
{}

IWindowScroller::~IWindowScroller(void)
{}

void IWindowScroller::AdjustScrollBy(float deltaX, float deltaY, bool isMomentum) {
  const float ppi = 120.0f; // Pixels per inch (base this on the DPI of the monitors -- FIXME)
  const float ppmm = ppi/25.4f; // Convert pixels per inch to pixels per millimeter
  OSPoint deltaPixel = OSPointMake(deltaX*ppmm, deltaY*ppmm); // Convert to pixels
  OSPoint deltaLine  = OSPointMake(deltaPixel.x/m_pixelsPerLine.x, deltaPixel.y/m_pixelsPerLine.y); // Convert to lines

  // Adjust partial pixels
  m_scrollPartialPixel.x += deltaPixel.x;
  m_scrollPartialPixel.y += deltaPixel.y;
  deltaPixel.x = round(m_scrollPartialPixel.x);
  deltaPixel.y = round(m_scrollPartialPixel.y);
  m_scrollPartialPixel.x -= deltaPixel.x;
  m_scrollPartialPixel.y -= deltaPixel.y;

  // Adjust partial lines
  m_scrollPartialLine.x += deltaLine.x;
  m_scrollPartialLine.y += deltaLine.y;
  deltaLine.x = floor(m_scrollPartialLine.x);
  deltaLine.y = floor(m_scrollPartialLine.y);
  m_scrollPartialLine.x -= deltaLine.x;
  m_scrollPartialLine.y -= deltaLine.y;

  // Perform the actual scrolling
  DoScrollBy(deltaPixel, deltaLine, isMomentum);
}

void IWindowScroller::ScrollBy(const OSPoint& virtualPosition, float deltaX, float deltaY) {
  std::lock_guard<std::mutex> lk(GetLock());
  m_virtualPosition = virtualPosition;

  const auto now = std::chrono::steady_clock::now();
  const auto dt = std::chrono::duration_cast<std::chrono::microseconds>(now - m_lastScrollTimePoint).count();
  if (dt > 0) { // Just to prevent a potential divide by zero. Should never happen in practice.
    OSPoint velocity(OSPointMake(deltaX/dt, deltaY/dt)); // Store as scroll units per microsecond

    m_velocitiesIndex = ++m_velocitiesIndex % MAX_VELOCITY_ENTRIES;
    m_velocities[m_velocitiesIndex] = velocity;
    if (m_velocitiesCount < MAX_VELOCITY_ENTRIES) {
      m_velocitiesCount++;
    }
    m_lastScrollTimePoint = now;
  }
  AdjustScrollBy(deltaX, deltaY, false);
}

void IWindowScroller::CancelScroll(void) {
  // Fix the scroll momentum at zero
  m_remainingMomentum = OSPointZero;

  // We don't care to retain the weak pointer anymore
  m_curScrollOp.reset();
}

std::shared_ptr<IScrollOperation> IWindowScroller::BeginScroll(void) {
  std::lock_guard<std::mutex> lk(GetLock());
  if(!m_curScrollOp.expired())
    return nullptr;

  auto retVal = std::shared_ptr<IScrollOperation>(
    static_cast<IScrollOperation*>(this),
    [this] (IScrollOperation*) {
      OSPoint momentum(OSPointZero);
      for (size_t i = m_velocitiesIndex; i < m_velocitiesCount; i++) {
        const auto& velocity = m_velocities[i % MAX_VELOCITY_ENTRIES];
        const auto absVx = std::abs(velocity.x);
        const auto absVy = std::abs(velocity.y);
        if (absVy > absVx) {
          if (absVy > std::abs(momentum.y)) {
            momentum.x = velocity.x;
            momentum.y = velocity.y;
          }
        } else {
          if (absVx > std::abs(momentum.x)) {
            momentum.x = velocity.x;
            momentum.y = velocity.y;
          }
        }
      }
      m_remainingMomentum.x = momentum.x;
      m_remainingMomentum.y = momentum.y;
      *this += std::chrono::microseconds(16667), [this] { OnPerformMomentumScroll(); };
    }
  );
  m_curScrollOp = retVal;
  return retVal;
}

void IWindowScroller::OnPerformMomentumScroll() {
  // Do not continue performing a momentum scroll if another op is outstanding
  if(!m_curScrollOp.expired()) {
    m_remainingMomentum = OSPointZero;
    return;
  }
  const auto absMx = std::abs(m_remainingMomentum.x);
  const auto absMy = std::abs(m_remainingMomentum.y);
  if (absMy < 0.00001f && absMx < 0.00001f) {
    // Is the momentum still large enough that we want to continue?
    m_remainingMomentum = OSPointZero;
    DoScrollBy(OSPointZero, OSPointZero, true);
    m_wse(&WindowScrollerEvents::OnScrollStopped)();
    return;
  }

  std::lock_guard<std::mutex> lk(GetLock());
  const auto now = std::chrono::steady_clock::now();
  const auto dt = std::chrono::duration_cast<std::chrono::microseconds>(now - m_lastScrollTimePoint).count();
  if (dt > 0) { // Just to prevent a potential divide by zero. Should never happen in practice.
    m_lastScrollTimePoint = now;
    const auto deltaX = m_remainingMomentum.x*dt;
    const auto deltaY = m_remainingMomentum.y*dt;
    AdjustScrollBy(deltaX, deltaY, true);

    // Apply drag by an exponential curve
    if (absMy < 0.0001f && absMx < 0.0001f) {
      m_remainingMomentum.x *= 0.95;
      m_remainingMomentum.y *= 0.95;
    } else {
      m_remainingMomentum.x *= 0.97;
      m_remainingMomentum.y *= 0.97;
    }
  }
  *this += std::chrono::microseconds(16667), [this] { OnPerformMomentumScroll(); };
}
