#include "stdafx.h"
#include "OSWindowWin.h"
#include "DwmThumbnailWindow.h"
#include "OSWindowEvent.h"
#include <Primitives.h>
#include <GLTexture2.h>

OSWindowWin::OSWindowWin(HWND hwnd):
  hwnd{hwnd},
  m_phBitmapBits{nullptr}
{
  m_szBitmap.cx = 0;
  m_szBitmap.cy = 0;
  m_prevSize = m_szBitmap;
}

OSWindowWin::~OSWindowWin(void)
{
}

void OSWindowWin::CheckSize(AutoFired<OSWindowEvent>& evt) {
  RECT rect;
  GetWindowRect(hwnd, &rect);

  SIZE sz;
  sz.cx = rect.right - rect.left;
  sz.cy = rect.bottom - rect.top;

  // Detect changes and then fire as needed:
  if(m_prevSize.cx == sz.cx && m_prevSize.cy == sz.cy)
    evt(&OSWindowEvent::OnResize)(*this);
  m_prevSize = sz;
}

bool OSWindowWin::IsValid(void) {
  return !!IsWindow(hwnd);
}

std::shared_ptr<OSApp> OSWindowWin::GetOwnerApp(void) {
  return nullptr;
}

uint32_t OSWindowWin::GetOwnerPid(void) {
  DWORD pid;
  GetWindowThreadProcessId(hwnd, &pid);
  return pid;
}

std::shared_ptr<ImagePrimitive> OSWindowWin::GetWindowTexture(std::shared_ptr<ImagePrimitive> img)  {
  // Create a proxy window if necessary:
  if(!m_proxyWindow)
    m_proxyWindow.reset(new DwmThumbnailWindow(hwnd));

  // Perform the update:
  m_proxyWindow->Update();
  return img;
}

bool OSWindowWin::GetFocus(void) {
  HWND foreground = GetForegroundWindow();
  return !!IsChild(foreground, hwnd);
}

void OSWindowWin::SetFocus(void) {
  ::SetForegroundWindow(hwnd);
}

std::wstring OSWindowWin::GetTitle(void) {
  std::wstring retVal(256, 0);
  int nch = GetWindowTextW(hwnd, &retVal[0], retVal.size());
  retVal.resize(nch);
  return retVal;
}

OSPoint OSWindowWin::GetPosition(void) {
  RECT rect;
  GetWindowRect(hwnd, &rect);

  OSPoint retVal;
  retVal.x = (float) rect.left;
  retVal.y = (float) rect.top;
  return retVal;
}

OSSize OSWindowWin::GetSize(void) {
  RECT rect;
  GetWindowRect(hwnd, &rect);

  OSSize retVal;
  retVal.width = (float) (rect.right - rect.left);
  retVal.height = (float) (rect.bottom - rect.top);
  return retVal;
}

void OSWindowWin::Cloak(void) {

}

void OSWindowWin::Uncloak(void) {
}

bool OSWindowWin::IsVisible(void) const {
  return !!::IsWindowVisible(hwnd);
}
