#include "stdafx.h"
#include "LeapInput.h"
#include "LeapInputListener.h"
#include "OSVirtualScreen.h"
#include "interaction/FrameFragmenter.h"

LeapInput::LeapInput(void):
  ContextMember("LeapInput"),
  m_isAcceptingInput(false)
{
  m_controller->enableGesture(Leap::Gesture::TYPE_CIRCLE);
  m_controller->setPolicyFlags(Leap::Controller::POLICY_BACKGROUND_FRAMES);
  m_controller->addListener(*this);
}

LeapInput::~LeapInput(void)
{
  m_controller->removeListener(*this);
}

bool LeapInput::AcceptInput(void) const {
  // Ignore input when the screen saver is running
  return m_virtualScreen && !m_virtualScreen->IsScreenSaverActive();
}

void LeapInput::onDisconnect(const Leap::Controller& controller) {
  m_isAcceptingInput = false;
  CurrentContextPusher pshr(this->GetContext());
  // Send an invalid frame when the device has disconnected
  m_listener(&LeapInputListener::OnLeapFrame)(Leap::Frame::invalid());
}

void LeapInput::onFrame(const Leap::Controller& controller) {
  if (!AcceptInput()) {
    if (m_isAcceptingInput) {
      m_isAcceptingInput = false;
      CurrentContextPusher pshr(this->GetContext());
      // Send an invalid frame when transitioning from "accepting input" to "not-accepting input"
      m_listener(&LeapInputListener::OnLeapFrame)(Leap::Frame::invalid());
    }
    return;
  }
  m_isAcceptingInput = true;
  CurrentContextPusher pshr(this->GetContext());
  m_listener(&LeapInputListener::OnLeapFrame)(controller.frame());
}
