#include <core/MainManager.h>

#include <cassert>
#include <cstdlib>

#include <ui/BasicRender.h>
#include <ui/SDL.h>
#include <ui/SDL_image.h>
#include <ui/SDL_mixer.h>
#include <ui/SDL_opengl.h>
#include <ui/SDL_ttf.h>
#include <ui/ShaderUtil.h>
#include <util/Exception.h>
#include <util/Log.h>
#include <util/Timer.h>


MainManager::MainManager()
  : logger_("MainManager"),
    graphics_(nullptr),
    runtime_(nullptr),
    basicRender_(nullptr),
    isRunning_(true)
{
  initSDL();
  initSDLimg();
  initSDLttf();
  initSDLmixer();

  graphics_.reset(new GraphicsManager);
  runtime_.reset(new Timer);
  runtime_->start();

  // TODO swarminglogic, 2013-09-15: Add a renderer, possibly refarctor to Game
  // class, which manages the renderer?
  basicRender_.reset(nullptr);
}

MainManager::~MainManager()
{
}


MainManager& MainManager::instance()
{
  static MainManager instance;
  return instance;
}


void MainManager::initialize()
{
  logger_.info("Inititalizing resources.");
  if (basicRender_)
    basicRender_->initialize();
}


void MainManager::finalize()
{
  logger_.info("Cleaning up resources.");
  if (basicRender_)
    basicRender_->finalize();
}

void MainManager::run() {
  assert(basicRender_ && "You need a renderer, fool!");

  SDL_Event event;
  bool isDirty = true;
  while (isRunning_) {
    while (SDL_PollEvent(&event)) {
      handleEvent(event);
      isDirty |= basicRender_->handleEvent(event);
    }

    SDL_Delay(20);
    if (isDirty) {
      basicRender_->render(runtime_->getSeconds());
    }
    graphics_->swapBuffers();
    isDirty = false;
  }
}


void MainManager::handleEvent(const SDL_Event& event)
{
  switch (event.type) {
  case SDL_MOUSEBUTTONDOWN:
    break;
  case SDL_KEYDOWN:
    switch (event.key.keysym.sym) {
    case SDLK_q: case SDLK_ESCAPE: isRunning_ = false; break;
    case SDLK_f: graphics_->toggleFullScreen(); break;
    case SDLK_v: graphics_->toggleVSync(); break;
    default:
      break;
    }
    break;
  case SDL_QUIT:
    isRunning_ = false;
    break;
  default:
    break;
  }
}


void MainManager::initSDL()
{
  logger_.debug("Initializing SDL");
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    throw logger_.exception("Failed to initialize SDL", SDL_GetError);
  atexit(SDL_Quit);
}

void MainManager::initSDLimg()
{
  logger_.debug("Initializing SDL_img");
  int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
  int imgFlagsInit = IMG_Init(imgFlags);
  if ((imgFlagsInit & imgFlags) != imgFlags)
    throw logger_.exception("Failed to initialize SDL_img", SDL_GetError);
  atexit(IMG_Quit);
}

void MainManager::initSDLttf()
{
  logger_.debug("Initializing SDL_ttf");
  if ( TTF_Init() != 0)
    throw logger_.exception("Failed to initialize SDL_ttf", SDL_GetError);
  atexit(TTF_Quit);
}

void MainManager::initSDLmixer()
{

  logger_.debug("Initializing SDL_mixer");
  int mixFlags = MIX_INIT_FLAC;
  int mixFlagsInit = Mix_Init(mixFlags);
  if ((mixFlagsInit & mixFlags) != mixFlags)
    throw logger_.exception("Failed to initialize SDL_mixer", Mix_GetError);
  if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 1024 ) == -1 )
    throw logger_.exception("Failed to aquire sound device", Mix_GetError);
  atexit(Mix_CloseAudio);
}
