#ifndef UI_SDL_H
#define UI_SDL_H

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

CREATE_RAII2(SDL_GLContext,   SDL_GL_DeleteContext)   GLContextPtr;

#endif
