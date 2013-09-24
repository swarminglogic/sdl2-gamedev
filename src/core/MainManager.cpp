#include <core/MainManager.h>

#include <cassert>

#include <ui/BasicRender.h>
#include <ui/SDL.h>
#include <ui/SDL_image.h>
#include <ui/SDL_mixer.h>
#include <ui/SDL_opengl.h>
#include <ui/SDL_ttf.h>
#include <ui/ShaderUtil.h>
#include <ui/VoidRenderer.h>
#include <util/Exception.h>
#include <util/Log.h>
#include <util/Timer.h>


MainManager::MainManager()
  : log_("MainManager"),
    graphics_(nullptr),
    runtime_(nullptr),
    basicRender_(nullptr),
    isRunning_(true),
    fpsCounter_(15)
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
  basicRender_.reset(new VoidRenderer);
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
  log_.i("Inititalizing resources.");
  if (basicRender_)
    basicRender_->initialize();
}


void MainManager::finalize()
{
  log_.i("Cleaning up resources.");
  if (basicRender_)
    basicRender_->finalize();
}

void MainManager::run() {
  assert(basicRender_ && "You need a renderer, fool!");

  SDL_Event event;
  bool isDirty = true;
  uint frameNumber = 0;
  while (isRunning_) {
    while (SDL_PollEvent(&event)) {
      handleEvent(event);
      isDirty |= basicRender_->handleEvent(event);
    }

    isDirty = true;
    ++frameNumber;
    if (isDirty | (frameNumber % 20 == 0)) {
      basicRender_->render(runtime_->getSeconds());
      graphics_->swapBuffers();
    }
    isDirty = false;


    fpsCounter_.tic();
    if (isDirty | (frameNumber % 100 == 0)) {
      log_.d() << "Fps: " << fpsCounter_.getFps() << Log::end;
    }
  }
}


void MainManager::handleEvent(const SDL_Event& event)
{
  switch (event.type) {
  case SDL_WINDOWEVENT:
    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
      log_.d() << "Window resized to "
               << event.window.data1 << " x " << event.window.data2 << Log::end;
      basicRender_->handleResize(event.window.data1, event.window.data2);
    }
  case SDL_MOUSEBUTTONDOWN:
    break;
  case SDL_KEYDOWN:
    switch (event.key.keysym.sym) {
    case SDLK_q: case SDLK_ESCAPE: isRunning_ = false; break;
    case SDLK_f: graphics_->toggleFullScreen(); break;
    case SDLK_v: graphics_->toggleVSync(); break;
    case SDLK_m: graphics_->toggleMouseGrab(); break;
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
  log_.i("Initializing SDL");
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    throw log_.exception("Failed to initialize SDL", SDL_GetError);
  atexit(SDL_Quit);
}

void MainManager::initSDLimg()
{
  log_.d("Initializing SDL_img");
  int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
  int imgFlagsInit = IMG_Init(imgFlags);
  if ((imgFlagsInit & imgFlags) != imgFlags)
    throw log_.exception("Failed to initialize SDL_img", SDL_GetError);
  atexit(IMG_Quit);
}

void MainManager::initSDLttf()
{
  log_.d("Initializing SDL_ttf");
  if ( TTF_Init() != 0)
    throw log_.exception("Failed to initialize SDL_ttf", SDL_GetError);
  atexit(TTF_Quit);
}

void MainManager::initSDLmixer()
{

  log_.d("Initializing SDL_mixer");
  int mixFlags = MIX_INIT_FLAC;
  int mixFlagsInit = Mix_Init(mixFlags);
  if ((mixFlagsInit & mixFlags) != mixFlags)
    throw log_.exception("Failed to initialize SDL_mixer", Mix_GetError);
  if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 1024 ) == -1 )
    throw log_.exception("Failed to aquire sound device", Mix_GetError);
  atexit(Mix_CloseAudio);
}
