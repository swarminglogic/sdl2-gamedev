#include <ui/GraphicsManager.h>

#include <config/ConfigManager.h>
#include <config/ViewConfig.h>
#include <util/Exception.h>


GraphicsManager::GraphicsManager()
  : logger_("GraphicsManager")
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

void GraphicsManager::setFullScreen(bool isFullScreen)
{
  if (isFullScreen) {
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


void GraphicsManager::setIsVSync(bool isVSync)
{
  isVSync_ = isVSync;
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
  int sdlWindowFlags = SDL_WINDOW_OPENGL;
  if (viewConfig.isFullScreen())
    sdlWindowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
  if (viewConfig.isResizeable())
    sdlWindowFlags |= SDL_WINDOW_RESIZABLE;

  // Create window
  logger_.info("Creating window ()");
  window_.reset(SDL_CreateWindow(viewConfig.getWindowTitle().c_str(),
                                 SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED,
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
}
