#include "NativeUI.h"
#include "NativeUIWin.h"

using namespace Shortcuts;

void NativeUI::ShowUI() {
  NativeUIWin::AddTrayIcon(*this);
}

void NativeUI::DestroyUI(void) {
  Application::Exit();
}

void NativeUI::ConfigChanged(const std::string& config, const json11::Json& value) {
  NativeUIWin::ConfigChanged(config, value.bool_value());
}