#ifndef UI_SURFACE_H
#define UI_SURFACE_H

#include <string>

#include <ui/SDL.h>
#include <ui/SDL_opengl.h>
#include <util/Log.h>


/**
 * Surface class, represents a SDL_Surface, and provides
 * helper functions to interact with OpenGL
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class Surface
{
public:
  Surface();
  virtual ~Surface();

  /**
   * @param filename Just the filename, it prepends image directory.
   */
  void loadImage(const std::string& filename);

  GLuint glBind();
  void prepareForGl();

  GLuint getTextureId() const;

  unsigned int getWidth() const;
  unsigned int getHeight() const;
private:
  Log log_;
  SurfacePtr surface_;
  std::string filename_;
  GLuint textureId_;

  // NonCopyable
  Surface(const Surface& c);
  Surface& operator=(const Surface& c);
};

#endif
