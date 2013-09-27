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
#include <util/Asset.h>
#include <util/Exception.h>
#include <util/Log.h>
#include <util/Timer.h>


MainManager::MainManager()
  : log_("MainManager"),
    graphics_(nullptr),
    runtime_(nullptr),
    basicRender_(nullptr),
    fpsRender_(),
    font_(nullptr),
    fontColor_{180u, 190u, 200u, 255u},
    isRunning_(true),
    fpsCounter_(15)
{
  initSDL();
  initSDLimg();
  initSDLttf();
  initSDLmixer();

  font_.reset(TTF_OpenFont(Asset::font("Minecraftia.ttf").c_str(), 8));

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
  basicRender_->initialize();
  basicRender_->handleResize(graphics_->getScreenSize().w(),
                             graphics_->getScreenSize().h());

  fpsRender_.initialize();
  updateFpsText(0.0);
  fpsRender_.setPosition(20, 20);

  fpsRender_.handleResize(graphics_->getScreenSize().w(),
                          graphics_->getScreenSize().h());
  fpsRender_.setZoomFactor(2);
  updateFpsText(0.0);
}


void MainManager::finalize()
{
  log_.i("Cleaning up resources.");
  basicRender_->finalize();
  fpsRender_.finalize();
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

    ++frameNumber;
    basicRender_->render(runtime_->getSeconds());
    fpsRender_.render(0);
    graphics_->swapBuffers();

    fpsCounter_.tic();
    if (frameNumber % 30 == 0)
      updateFpsText(fpsCounter_.getFps());
  }
}


void MainManager::handleEvent(const SDL_Event& event)
{
  switch (event.type) {
  case SDL_WINDOWEVENT:
    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
      const int width = event.window.data1;
      const int height = event.window.data2;
      log_.i() << "Window resized to " << width << " x " << height << Log::end;
      basicRender_->handleResize(width, height);
      fpsRender_.handleResize(width, height);
      graphics_->setScreenSize(Size(width, height));
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

void MainManager::updateFpsText(double fps)
{
  assert(font_ && "No font?");
  std::stringstream ss;
  ss.precision(2);
  ss << std::fixed <<  "FPS: " << fps;
  fpsRender_.setSurface(*TTF_RenderText_Solid(font_.get(),
                                              ss.str().c_str(),
                                              fontColor_));
}
