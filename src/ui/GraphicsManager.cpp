#include <ui/GraphicsManager.h>

#include <config/ConfigManager.h>
#include <config/ViewConfig.h>
#include <util/Exception.h>


GraphicsManager::GraphicsManager()
  : logger_("GraphicsManager")
{
  initalizeOpenGL(ConfigManager::instance().getViewConfig());
}


GraphicsManager::~GraphicsManager()
{
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
}
