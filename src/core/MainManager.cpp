#include <core/MainManager.h>

#include <cassert>
#include <sstream>

#include <io/Keyboard.h>
#include <io/TextBoxText.h>
#include <model/PhysicsWorld.h>
#include <ui/DeferredRenderer.h>
#include <ui/DeferredRendererOld.h>
#include <ui/SDL.h>
#include <ui/SDL_image.h>
#include <ui/SDL_mixer.h>
#include <ui/SDL_opengl.h>
#include <ui/SDL_ttf.h>
#include <ui/SceneBasic.h>
#include <ui/ShaderUtil.h>
#include <ui/VoidRenderer.h>
#include <util/Asset.h>
#include <util/CharMap.h>
#include <util/Exception.h>
#include <util/Log.h>
#include <util/Timer.h>


MainManager::MainManager()
  : log_("MainManager"),
    graphics_(nullptr),
    runtime_(nullptr),
    basicRender_(nullptr),
    physics_(nullptr),
    resourceManager_(new ResourceManager),
    textRenderer_(),
    font_(nullptr),
    fontColor_{180u, 190u, 200u, 255u},
    isRunning_(true),
    fpsCounter_(30),
    currentTimeDelta_(1.0f/60.0f)
{
  initSDL();
  initSDLimg();
  initSDLttf();
  initSDLmixer();
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

  font_.reset(TTF_OpenFont(AssetFont("Minecraftia.ttf").path().c_str(), 8));

  graphics_.reset(new GraphicsManager);
  runtime_.reset(new Timer);
  runtime_->start();
  basicRender_.reset(new DeferredRenderer);

  if (dynamic_cast<DeferredRenderer*>(basicRender_.get())) {
    scene_.reset(new SceneBasic);
    dynamic_cast<DeferredRenderer*>(basicRender_.get())->setScene(scene_);
  }

  physics_.reset(new PhysicsWorld),

  log_.i("Inititalizing resources.");

  basicRender_->initialize();
  basicRender_->handleResize(graphics_->getScreenSize().w(),
                             graphics_->getScreenSize().h());


  // Set mouse grab based on main renderer preference.
  graphics_->setIsMouseGrab(basicRender_->prefersMouseGrab());

  // Text font renderer
  textRenderer_.loadImage(AssetImage("gnsh-bitmapfont.png"));
  TextBoxText tbt;
  tbt.setHeight(12u);
  tbt.setWidthFixed(5u);
  textRenderer_.setTextBoxText(tbt);
  CharMap cm(Size(5, 12));
  std::vector<CharMap::Trait> traits = {
    CharMap::C_GOLDEN,
    CharMap::C_CYAN,
    CharMap::C_RED,
    CharMap::C_MAGENTA,
    CharMap::C_GREY,
    CharMap::C_WHITE,
    CharMap::C_GREEN,
    CharMap::C_BLUE,
    CharMap::C_OLIVE };
  cm.setTraits(traits);
  textRenderer_.setCharMap(cm);
  textRenderer_.handleResize(graphics_->getScreenSize().w(),
                             graphics_->getScreenSize().h());
  textRenderer_.initialize();
  textRenderer_.setPosition(Point(40, 40));
  textRenderer_.setZoomFactor(2);
  textRenderer_.postConfigureInitialize();
  updateFpsText(0.0);
}


void MainManager::finalize()
{
  log_.i("Cleaning up resources.");
  basicRender_->finalize();
}


Uint32 MainManager::getRuntimeTics() const
{
  return runtime_->getTicks();
}

float MainManager::getRuntimeSecs() const
{
  return runtime_->getSeconds();
}

float MainManager::getCurrentTimeDelta() const
{
  return currentTimeDelta_;
}


PhysicsWorld& MainManager::getPhysicsWorld()
{
  assert(physics_);
  return *physics_.get();
}

ResourceManager& MainManager::getResourceManager()
{
  assert(resourceManager_);
  return *resourceManager_;

}

ResourceManager& MainManager::resources()
{
  return instance().getResourceManager();
}




void MainManager::run() {
  assert(basicRender_ && "You need a renderer, fool!");

  SDL_Event event;

  uint frameNumber = 0;
  float previousTime = runtime_->getSeconds();

  while (isRunning_) {
#ifndef NDEBUG
    const char* err = SDL_GetError();
    if (err[0] != '\0') {
      log_.e() << "SDL_GetError(): " << err << Log::end;
    }
#endif
    SDL_ClearError();

    while (SDL_PollEvent(&event)) {
      handleEvent(event);
      basicRender_->handleEvent(event);
      if (scene_) scene_->handleEvent(event);
    }

    ++frameNumber;

    if (scene_) scene_->refresh();

    basicRender_->refresh();
    textRenderer_.refresh();

    currentTimeDelta_ = runtime_->getSeconds() - previousTime;
    previousTime = runtime_->getSeconds();
    physics_->stepSimulation(currentTimeDelta_);

    basicRender_->render(runtime_->getSeconds());
    textRenderer_.render(0);
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
      textRenderer_.handleResize(width, height);
      graphics_->setScreenSize(Size(width, height));
    }
  case SDL_MOUSEBUTTONDOWN:
    break;
  case SDL_KEYUP:
    Keyboard::instance().handleEvent(event);
    break;
  case SDL_KEYDOWN:
    Keyboard::instance().handleEvent(event);
    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE: isRunning_ = false; break;
    case SDLK_f: graphics_->toggleFullScreen(); break;
    case SDLK_v: graphics_->toggleVSync(); break;
    case SDLK_m: graphics_->toggleMouseGrab(); break;
    case SDLK_SPACE: runtime_->togglePauseResume(); break;
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

  // Write version information to log
  SDL_version compiled;
  SDL_version linked;
  SDL_VERSION(&compiled);
  SDL_GetVersion(&linked);
  logSDLVersion("SDL", compiled, linked, SDL_GetRevision());
}

void MainManager::initSDLimg()
{
  log_.i("Initializing SDL_img");
  int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
  int imgFlagsInit = IMG_Init(imgFlags);
  if ((imgFlagsInit & imgFlags) != imgFlags)
    throw log_.exception("Failed to initialize SDL_img", SDL_GetError);
  atexit(IMG_Quit);

  // Write version information to log
  SDL_version compiled;
  SDL_IMAGE_VERSION(&compiled);
  logSDLVersion("SDL_image", compiled, *IMG_Linked_Version());
}

void MainManager::initSDLttf()
{
  log_.i("Initializing SDL_ttf");
  if ( TTF_Init() != 0)
    throw log_.exception("Failed to initialize SDL_ttf", SDL_GetError);
  atexit(TTF_Quit);

  // Write version information to log
  SDL_version compiled;
  SDL_TTF_VERSION(&compiled);
  logSDLVersion("SDL_ttf", compiled, *TTF_Linked_Version());
}

void MainManager::initSDLmixer()
{

  log_.i("Initializing SDL_mixer");
  int mixFlags = MIX_INIT_FLAC;
  int mixFlagsInit = Mix_Init(mixFlags);
  if ((mixFlagsInit & mixFlags) != mixFlags)
    throw log_.exception("Failed to initialize SDL_mixer", Mix_GetError);
  if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 1024 ) == -1 )
    throw log_.exception("Failed to aquire sound device", Mix_GetError);
  atexit(Mix_CloseAudio);


  // Write version information to log
  SDL_version compiled;
  SDL_MIXER_VERSION(&compiled);
  logSDLVersion("SDL_mixer", compiled, *Mix_Linked_Version());
}


void MainManager::logSDLVersion(const std::string& what,
                                const SDL_version& compiled,
                                const SDL_version& linked,
                                std::string revision)
{
  std::stringstream ss;
  ss << what << " Version (Compiled): "
     << (int)compiled.major << "."
     << (int)compiled.minor << "."
     << (int)compiled.patch;
  if (!revision.empty())
    ss << " (" << revision << ")";
  log_.d(ss.str());

  log_.d() << what << " Version (Runtime):  "
           << linked.major << "." << linked.minor << "." << linked.patch
           << Log::end;
}


void MainManager::updateFpsText(double fps)
{
  assert(font_ && "No font?");
  std::stringstream ss;
  ss.precision(2);
  ss << std::fixed <<  "FPS: " << fps;
  if (textRenderer_.isReady())
    textRenderer_.setText(ss.str());
  // fpsRender_.setSurface(*TTF_RenderText_Solid(font_.get(),
  //                                             ss.str().c_str(),
  //                                             fontColor_));
}
