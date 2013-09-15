#ifndef UI_SDL_OPENGL_H
#define UI_SDL_OPENGL_H

// Required definition call for OpenGL 3.0+ functions
#define GL_GLEXT_PROTOTYPES 1

// SDL comes with built in glext.h, uncomment to use local one
#define NO_SDL_GLEXT


// Regular includs
#include <SDL2/SDL_opengl.h>

#ifdef NO_SDL_GLEXT
#include <GL/glext.h>
#endif


#endif
