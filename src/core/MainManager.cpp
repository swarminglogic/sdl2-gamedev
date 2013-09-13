#include <core/MainManager.h>

#include <cstdlib>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include <util/Exception.h>
#include <util/Log.h>


MainManager& MainManager::instance()
{
  static MainManager instance;
  return instance;
}


MainManager::MainManager()
{
  logger_.reset(new Log("MainManager"));

  initSDL();
  initSDLimg();
  initSDLttf();
  initSDLmixer();

  logger_->debug("Testing deletion of nullptr usin sdl destructors");
  TTF_CloseFont(nullptr);
  logger_->debug("it went ok");

  graphicsManager_.reset(new GraphicsManager);
}

MainManager::~MainManager()
{
}

void MainManager::initSDL()
{
  logger_->debug("Initializing SDL");
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    logger_->error("Failed to initialize SDL");
    logger_->error(SDL_GetError());
    throw Exception("MainManager SDL failure");
  }
  atexit(SDL_Quit);
}

void MainManager::initSDLimg()
{
  logger_->debug("Initializing SDL_img");
  int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
  int imgFlagsInit = IMG_Init(imgFlags);
  if ((imgFlagsInit & imgFlags) != imgFlags) {
    logger_->error("Failed to initialize SDL_img");
    logger_->error(IMG_GetError());
    throw Exception("MainManager SDL img failure");
  }
  atexit(IMG_Quit);
}

void MainManager::initSDLttf()
{
  logger_->debug("Initializing SDL_ttf");
  if ( TTF_Init() != 0) {
    logger_->error("Failed to initialize SDL_ttf");
    logger_->error(TTF_GetError());
    throw Exception("MainManager SDL ttf failure");
  }
  atexit(TTF_Quit);
}

void MainManager::initSDLmixer()
{
  logger_->debug("Initializing SDL_mixer");
  int mixFlags = MIX_INIT_FLAC;
  int mixFlagsInit = Mix_Init(mixFlags);
  if ((mixFlagsInit & mixFlags) != mixFlags) {
    logger_->error("Failed to initialize SDL_mixer");
    logger_->error(Mix_GetError());
    throw Exception("MainManager SDL mixer failure");
  }
  if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 1024 ) == -1 ) {
    logger_->error(Mix_GetError());
    throw Exception("MainManager SDL mixer (on open) failure");
  }
  atexit(Mix_CloseAudio);
}
