#include <ui/GraphicsManager.h>

#include <cassert>
#include <iomanip>
#include <sstream>

#include <config/ConfigManager.h>
#include <config/ViewConfig.h>
#include <ui/SDL_opengl.h>
#include <util/Exception.h>


GraphicsManager::GraphicsManager()
  : logger_("GraphicsManager"),
    window_(nullptr),
    context_(nullptr),
    isFullScreen_(false),
    isVSync_(true)
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
    logger_.info("Enabling fullscreen");
    SDL_SetWindowFullscreen(window_.get(), SDL_WINDOW_FULLSCREEN_DESKTOP);
  }
  else {
    logger_.info("Disabling fullscreen");
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
  isVSync_ = isVSyncEnabled;
  if (isVSync_)
    logger_.info("Enabling vsync");
  else
    logger_.info("Disabling vsync");

  const int ret = SDL_GL_SetSwapInterval(static_cast<int>(isVSync_));
  if (ret < 0) {
    logger_.warning("Failed to change vsync mode.");
    logger_.warning(SDL_GetError());
  }
}


void GraphicsManager::initalizeOpenGL(const ViewConfig& viewConfig)
{
  logger_.debug("Initalizing OpenGL");
  int sdlWindowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
  if (viewConfig.isFullScreen())
    sdlWindowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
  if (viewConfig.isResizeable())
    sdlWindowFlags |= SDL_WINDOW_RESIZABLE;

  // Create window
  logger_.info("Creating window ()");
  window_.reset(SDL_CreateWindow(viewConfig.getWindowTitle().c_str(),
                                 500,
                                 200,
                                 viewConfig.getScreenWidth(),
                                 viewConfig.getScreenHeight(),
                                 sdlWindowFlags));
  if (!window_)
    throw logger_.exception("Failed to initialize OpenGL", SDL_GetError);

  // Create OpenGL Context
  // TODO swarminglogic, 2013-09-14: Have fallback methods when creating
  // the OpenGL context.
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,4);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  logger_.debug("Creating OpenGL Context");
  context_.reset(new SDL_GLContext(SDL_GL_CreateContext(window_.get())));
  if (!context_)
    throw logger_.exception("Failed to create OpenGL Context", SDL_GetError);

  // Make it the current context
  SDL_GL_MakeCurrent(window_.get(), *context_);

  logStaticOpenGLInfo();
  logGraphicsDriverInfo();
  logOpenGLContextInfo();
}


void GraphicsManager::logStaticOpenGLInfo() const
{
  logger_.d() << "OpenGL GLEXT version: " << GL_GLEXT_VERSION << Log::end;
}


void GraphicsManager::logOpenGLContextInfo() const
{
  assert(context_ && "Missing OpenGL Context");
  std::stringstream ss;
  ss << std::setw(20) << std::left << "OpenGL Version: "
     << glGetString(GL_VERSION);
  logger_.debug(ss.str()); ss.str("");
  ss << std::setw(20) << std::left << "OpenGL GLSL: "
     << glGetString(GL_SHADING_LANGUAGE_VERSION);
  logger_.debug(ss.str()); ss.str("");
  ss << std::setw(20) << std::left << "OpenGL Renderer: "
     << glGetString(GL_RENDERER);
  logger_.debug(ss.str()); ss.str("");
  ss << std::setw(20) << std::left << "OpenGL Vendor: "
     << glGetString(GL_VENDOR);
  logger_.debug(ss.str()); ss.str("");

  // supported extensions:
  GLint nExtensions;
  glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);
  ss << std::setw(20) << std::left << "OpenGL #EXT: "
     << nExtensions;
  logger_.debug(ss.str()); ss.str("");

  #ifdef DEBUG_OPENGL_EXT
  for (GLint i = 0 ; i < nExtensions ; ++i) {
    ss << glGetStringi(GL_EXTENSIONS, i);
    logger_.debug(ss.str()); ss.str("");
  }
  #endif
}


void GraphicsManager::logGraphicsDriverInfo() const
{
  assert(context_ && "Missing OpenGL Context");

  const int nVideoDrivers = SDL_GetNumVideoDrivers();
  logger_.d() << "Found " << nVideoDrivers << " video drivers" << Log::end;

  const std::string currentVideoDriver(SDL_GetCurrentVideoDriver());
  for (int i = 0; i < nVideoDrivers; i++) {
    const std::string videoDriver(SDL_GetVideoDriver(i));
    logger_.d() << "Video Driver #" << i << ": " << videoDriver;
    if (currentVideoDriver == videoDriver)
      logger_ << " (Current)";
    logger_ << Log::end;
  }

  const int nRenderDrivers = SDL_GetNumRenderDrivers();
  logger_.d() << "Found " << nRenderDrivers << " render drivers" << Log::end;

  SDL_RendererInfo info;
  for (int i = 0 ; i < nRenderDrivers ; ++i) {
    SDL_GetRenderDriverInfo(i, &info);
    logger_.d() << "Render Driver #" << i << ": " << info.name;

    bool isSoftware      = info.flags & SDL_RENDERER_SOFTWARE;
    bool isHardware      = info.flags & SDL_RENDERER_ACCELERATED;
    bool isVSyncEnabled  = info.flags & SDL_RENDERER_PRESENTVSYNC;
    bool isTargetTexture = info.flags & SDL_RENDERER_TARGETTEXTURE;

    logger_ << "\t [";
    if (isSoftware) logger_ << "SW";
    if (isHardware) logger_ << "HW";
    if (isVSyncEnabled) logger_ << ", VSync";
    if (isTargetTexture) logger_ << ", TT";
    logger_ << "]" << Log::end;
  }
}
