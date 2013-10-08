#ifndef UI_SURFACE_H
#define UI_SURFACE_H

#include <string>

#include <math/Rect.h>
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

  /**
   * Create based on SDL_Surface.
   * @note Takes ownership!
   */
  Surface(SDL_Surface& surface);

  virtual ~Surface();

  /**
   * @param filename Just the filename, it prepends image directory.
   */
  void loadImage(const std::string& filename);

  /**
   * Fill using a SDL_Surface.
   * @note Takes ownership!
   */
  void setSurface(SDL_Surface& surface);

  GLuint glBind();
  void prepareForGl();

  GLuint getTextureId() const;

  unsigned int getWidth() const;
  unsigned int getHeight() const;

  const Rect& getClip() const;
  void setClip(const Rect& clip);

  /**
   * Only use if image dimensions are not a power of two.
   *
   * If a clip is set, it assumes texcoord are passed to withing this clip.
   * I.e 0.0 correspond to the clip start. 1.0 to the clip end.
   *
   * The surface will extend to powers of two regardless, and then the texture
   * coordinates need to be adjusted. This function calculates that new
   * coordinate.
   */
  float glTexCoordX(float texcoord) const;
  float glTexCoordY(float texcoord) const;


  /**
   * If set true, it enables linear mipmapping and anisotropic filtering.
   * If set false, sets nearest filter, with clamp to edge wrapping.
   */
  void setIsMaxFiltering(bool isMaxFiltering);
  bool isMaxFiltering() const;

private:
  void releaseResources();

  Log log_;
  SurfacePtr surface_;
  std::string filename_;
  GLuint textureId_;
  Rect clip_;
  bool isMaxFiltering_;

  // NonCopyable
  Surface(const Surface& c);
  Surface& operator=(const Surface& c);
};

#endif
