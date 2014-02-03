#ifndef UI_SDL_MIXER_H
#define UI_SDL_MIXER_H

#include <SDL2/SDL_mixer.h>

#include <util/RAIIhelper.h>


/**
 * SDL mixer header wrapper
 * @author SwarmingLogic (Roald Fernandez)
 */
CREATE_RAII_UP(Mix_Music,    Mix_FreeMusic)       MusicPtr;
CREATE_RAII_UP(Mix_Chunk,    Mix_FreeChunk)       SoundPtr;

#endif
