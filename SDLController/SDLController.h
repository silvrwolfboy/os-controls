#pragma once

#include "SDL.h"
#include <SDL_syswm.h>

#include <string>

// Encapsulates various SDL/OpenGL parameters to be passed into SDLController::Initialize()
// Currently the following fields are supported:
// - Desired window width in pixels (ignored and reset when fullscreen = true)
// - Desired window height in pixels (ignored and reset when fullscreen = true)
// - Desired window position X in pixels (ignored and reset when fullscreen = true)
// - Desired window position Y in pixels (ignored and reset when fullscreen = true)
// - Whether to resize the window the be the resolution of the primary monitor
// - Transparency on or off
// - Whether the window will stay on top of other windows
// - Use vertical synchronization on or off
// - Use anti-aliasing on or off
// - Window title of the application
struct SDLControllerParams {
  SDLControllerParams() :
    windowWidth(640),
    windowHeight(480),
    windowPosX(100),
    windowPosY(100),
    fullscreen(false),
    transparentWindow(false),
    alwaysOnTop(false),
    vsync(false),
    antialias(true),
    windowTitle("GLApp")
  { }

  int windowWidth;
  int windowHeight;
  int windowPosX;
  int windowPosY;
  bool fullscreen;
  bool transparentWindow;
  bool alwaysOnTop;
  bool vsync;
  bool antialias;
  std::string windowTitle;
};

// This class bundles all the SDL usage/state into a single point of control.
class SDLController {
public:

  SDLController();
  ~SDLController();

  // This creates the window, renderer, and GL context.  Throws an exception to denote error.
  void Initialize(const SDLControllerParams& params = SDLControllerParams());
  // Shuts everything down, in order opposite of initialization.  No exceptions should be thrown.
  void Shutdown();

  void BeginRender() const;
  void EndRender() const;

  static std::string BasePath();

  const SDLControllerParams& GetParams() const { return m_Params; }

private:

  void InitSDL();
  void ConfigureFrameBuffer();
  void ConfigureAntialiasing();
  void InitWindow();
  void ConfigureRenderer();
  void InitGLContext();
  void ConfigureTransparentWindow();

#if _WIN32
  void MakeTransparent_Windows(const SDL_SysWMinfo &sys_wm_info);
#endif

  SDL_Window *m_SDL_Window;
  SDL_Renderer *m_SDL_Renderer;
  SDL_GLContext m_SDL_GLContext;
  SDLControllerParams m_Params;
};
