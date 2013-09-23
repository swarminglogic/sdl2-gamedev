#include <ui/GraphicsManager.h>

#include <cassert>
#include <iomanip>
#include <sstream>

#include <config/ConfigManager.h>
#include <config/ViewConfig.h>
#include <ui/SDL_opengl.h>
#include <util/Exception.h>


GraphicsManager::GraphicsManager()
  : log_("GraphicsManager"),
    window_(nullptr),
    context_(nullptr),
    isFullScreen_(false),
    isVSync_(true),
    isOpenGlDebugEnabled_(true)
{
  const ViewConfig& viewConfig = ConfigManager::instance().getViewConfig();
  isFullScreen_ = viewConfig.isFullScreen();
  initalizeOpenGL(viewConfig);
}


GraphicsManager::~GraphicsManager()
{
}


void GraphicsManager::swapBuffers()
{
  SDL_GL_SwapWindow(window_.get());
}


void GraphicsManager::toggleFullScreen()
{
  isFullScreen_ = !isFullScreen_;
  setFullScreen(isFullScreen_);
}

void GraphicsManager::setFullScreen(bool isFullScreenEnabled)
{
  if (isFullScreenEnabled) {
    log_.i("Enabling fullscreen");
    SDL_SetWindowFullscreen(window_.get(), SDL_WINDOW_FULLSCREEN);
  }
  else {
    log_.i("Disabling fullscreen");
    SDL_SetWindowFullscreen(window_.get(), 0);
  }
}

bool GraphicsManager::isFullScreen() const
{
  return isFullScreen_;
}


void GraphicsManager::toggleVSync()
{
  isVSync_ = !isVSync_;
  setIsVSync(isVSync_);
}


bool GraphicsManager::isVSync() const
{
  return isVSync_;
}


void GraphicsManager::setIsVSync(bool isVSyncEnabled)
{
  log_.i() << (isVSync_ ? "Enabling" : "Disabling") << " vsync" << Log::end;

  const int ret = SDL_GL_SetSwapInterval(static_cast<int>(isVSync_));
  if (ret >= 0)
    isVSync_ = isVSyncEnabled;
  else
    log_.w() << "Failed to change vsync mode: " << SDL_GetError() << Log::end;
}


void GraphicsManager::initalizeOpenGL(const ViewConfig& viewConfig)
{
  log_.i("Initalizing OpenGL");

  // TODO swarminglogic, 2013-09-23: Improve cross system compatibility!
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,           8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,         8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,          8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,         8);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,       1);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,        24);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,         24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,       8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,     8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,   8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,    8);
  SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,   8);
  SDL_GL_SetAttribute(SDL_GL_STEREO,             0);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 32);
  // SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1); // Causes error, SDL bug?
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  if (isOpenGlDebugEnabled_)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

  int sdlWindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
  if (viewConfig.isFullScreen())
    sdlWindowFlags |= SDL_WINDOW_FULLSCREEN;
  if (viewConfig.isResizeable())
    sdlWindowFlags |= SDL_WINDOW_RESIZABLE;

  // Create SDL Window
  window_.reset(SDL_CreateWindow(viewConfig.getWindowTitle().c_str(),
                                 500, 200,
                                 viewConfig.getScreenWidth(),
                                 viewConfig.getScreenHeight(),
                                 sdlWindowFlags));
  if (!window_)
    throw log_.exception("Failed to initialize OpenGL", SDL_GetError);

  // Create OpenGL Context
  log_.i("Creating OpenGL Context");
  context_.reset(new SDL_GLContext(SDL_GL_CreateContext(window_.get())));
  if (!context_)
    throw log_.exception("Failed to create OpenGL Context", SDL_GetError);

  // Set VSync
  if(SDL_GL_SetSwapInterval(static_cast<int>(viewConfig.isVSync())) < 0)
    log_.w() << "Failed to change vsync mode: " << SDL_GetError() << Log::end;


  // TODO swarminglogic, 2013-09-14: Have fallback methods when creating
  // the OpenGL context.

  // // Make it the current context
  SDL_GL_MakeCurrent(window_.get(), *context_);

  logStaticOpenGLInfo();
  logGraphicsDriverInfo();
  logOpenGLContextInfo();
}


void GraphicsManager::logStaticOpenGLInfo() const
{
  log_.d() << "OpenGL GLEXT version: " << GL_GLEXT_VERSION << Log::end;
}


void GraphicsManager::logOpenGLContextInfo() const
{
  assert(context_ && "Missing OpenGL Context");
  std::stringstream ss;
  ss << std::setw(20) << std::left << "OpenGL Version: "
     << glGetString(GL_VERSION);
  log_.i(ss.str()); ss.str("");
  ss << std::setw(20) << std::left << "OpenGL GLSL: "
     << glGetString(GL_SHADING_LANGUAGE_VERSION);
  log_.i(ss.str()); ss.str("");
  ss << std::setw(20) << std::left << "OpenGL Renderer: "
     << glGetString(GL_RENDERER);
  log_.i(ss.str()); ss.str("");
  ss << std::setw(20) << std::left << "OpenGL Vendor: "
     << glGetString(GL_VENDOR);
  log_.i(ss.str()); ss.str("");

  // supported extensions:
  GLint nExtensions;
  glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
  ss << std::setw(20) << std::left << "OpenGL #EXT: "
     << nExtensions;
  log_.d(ss.str()); ss.str("");

  #ifdef DEBUG_OPENGL_EXT
  for (GLint i = 0 ; i < nExtensions ; ++i) {
    ss << glGetStringi(GL_EXTENSIONS, i);
    log_.d(ss.str()); ss.str("");
  }
  #endif
}


void GraphicsManager::logGraphicsDriverInfo() const
{
  assert(context_ && "Missing OpenGL Context");

  const int nVideoDrivers = SDL_GetNumVideoDrivers();
  log_.d() << "Found " << nVideoDrivers << " video drivers" << Log::end;

  const std::string currentVideoDriver(SDL_GetCurrentVideoDriver());
  for (int i = 0; i < nVideoDrivers; i++) {
    const std::string videoDriver(SDL_GetVideoDriver(i));
    log_.d() << "Video Driver #" << i << ": " << videoDriver;
    if (currentVideoDriver == videoDriver)
      log_ << " (Current)";
    log_ << Log::end;
  }

  const int nRenderDrivers = SDL_GetNumRenderDrivers();
  log_.d() << "Found " << nRenderDrivers << " render drivers" << Log::end;

  SDL_RendererInfo info;
  for (int i = 0 ; i < nRenderDrivers ; ++i) {
    SDL_GetRenderDriverInfo(i, &info);
    log_.d() << "Render Driver #" << i << ": " << info.name;

    bool isSoftware      = info.flags & SDL_RENDERER_SOFTWARE;
    bool isHardware      = info.flags & SDL_RENDERER_ACCELERATED;
    bool isVSyncEnabled  = info.flags & SDL_RENDERER_PRESENTVSYNC;
    bool isTargetTexture = info.flags & SDL_RENDERER_TARGETTEXTURE;

    log_ << "\t [";
    if (isSoftware) log_ << "SW";
    if (isHardware) log_ << "HW";
    if (isVSyncEnabled) log_ << ", VSync";
    if (isTargetTexture) log_ << ", TT";
    log_ << "]" << Log::end;
  }
}
