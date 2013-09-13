#ifndef UTIL_SDL_TTF_H
#define UTIL_SDL_TTF_H

#include <SDL2/SDL_ttf.h>

#include <util/RAIIhelper.h>


/**
 * SDL ttf header wrapper
 * @author SwarmingLogic
 */
CREATE_RAII(TTF_Font,     TTF_CloseFont)       FontPtr;

#endif
