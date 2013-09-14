#ifndef UTIL_SDL_H
#define UTIL_SDL_H

#include <SDL2/SDL.h>

#include <util/RAIIhelper.h>


/**
 * SDL header wrapper
 * @author SwarmingLogic (Roald Fernandez)
 */

CREATE_RAII(SDL_Surface,  SDL_FreeSurface)     SurfacePtr;
CREATE_RAII(SDL_Texture,  SDL_DestroyTexture)  TexturePtr;
CREATE_RAII(SDL_Renderer, SDL_DestroyRenderer) RendererPtr;
CREATE_RAII(SDL_Window,   SDL_DestroyWindow)   WindowPtr;

#endif
