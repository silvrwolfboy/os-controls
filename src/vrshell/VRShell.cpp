#include "stdafx.h"
#include "VRShell.h"
#include "utility/PlatformInitializer.h"
#include "osinterface/MakesRenderWindowFullScreen.h"
#include "OculusVR.h"
#include <autowiring/AutoNetServer.h>
#include <iostream>

int main(int argc, char **argv)
{
  PlatformInitializer init;
  AutoCurrentContext ctxt;
  
  ctxt->Initiate();

  try {
    AutoCreateContextT<VRShellContext> shellCtxt;
    shellCtxt->Initiate();
    CurrentContextPusher pshr(shellCtxt);

    AutoRequired<VRShell> shell;
    AutoConstruct<sf::ContextSettings> contextSettings(0, 0, 16);
    AutoConstruct<sf::RenderWindow> mw(
      sf::VideoMode(1, 1),
      "VRShell", sf::Style::None,
      *contextSettings
    );
    AutoRequired<MakesRenderWindowFullScreen>();
    AutoConstruct<OculusVR> hmdInterface;
    hmdInterface->SetHWND(mw->getSystemHandle());
    hmdInterface->Init();

    // Run as fast as possible:
    mw->setFramerateLimit(120);
    mw->setVerticalSyncEnabled(false);

    // Handoff to the main loop:
    shell->Main();
  }
  catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }

  ctxt->SignalShutdown(true);
  return 0;
}

VRShell::VRShell(void)
{
}

VRShell::~VRShell(void) {}

void VRShell::Main(void) {
  //AutoFired<Updatable> upd;

  // Dispatch events until told to quit:
  auto then = std::chrono::steady_clock::now();
  for(AutoCurrentContext ctxt; !ctxt->IsShutdown(); ) {
    // Handle all events:
    for(sf::Event evt; m_mw->pollEvent(evt);)
      HandleEvent(evt);

    // Broadcast update event to all interested parties:
    //auto now = std::chrono::steady_clock::now();
    //upd(&Updatable::Tick)(now - then);
    //then = now;
  }
  m_mw->close();
}

void VRShell::HandleEvent(const sf::Event& ev) const {
  switch (ev.type) {
  case sf::Event::Closed:
    AutoCurrentContext()->SignalShutdown();
    break;
  default:
    break;
  }
}

void VRShell::Filter(void) {
  try {
    throw;
  }
  catch (std::exception& ex) {
    std::cerr << ex.what() << std::endl;
  }
}