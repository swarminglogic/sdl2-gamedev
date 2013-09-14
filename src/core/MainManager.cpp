#include <core/MainManager.h>

#include <cstdlib>

#include <ui/SDL.h>
#include <ui/SDL_image.h>
#include <ui/SDL_mixer.h>
#include <ui/SDL_ttf.h>
#include <util/Exception.h>
#include <util/Log.h>


MainManager& MainManager::instance()
{
  static MainManager instance;
  return instance;
}


MainManager::MainManager()
  : logger_("MainManager")
{
  initSDL();
  initSDLimg();
  initSDLttf();
  initSDLmixer();

  graphicsManager_.reset(new GraphicsManager);
}

MainManager::~MainManager()
{
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
