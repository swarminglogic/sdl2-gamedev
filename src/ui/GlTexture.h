#ifndef UI_GLTEXTURE_H
#define UI_GLTEXTURE_H

#include <string>

#include <math/Rect.h>
#include <ui/SDL.h>
#include <ui/SDL_opengl.h>
#include <util/Log.h>


/**
 * GlTexture class, represents a SDL_GlTexture, and provides
 * helper functions to interact with OpenGL
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class GlTexture
{
public:
  GlTexture();

  virtual ~GlTexture();

  // void setGlTextureId(GLuint textureId, Size size);
  void setId(GLuint textureId);

  GLuint glBind() const;
  void prepareForGl();

  GLuint getTextureId() const;

  /**
   * If set true, it enables linear mipmapping and anisotropic filtering.
   * If set false, sets nearest filter, with clamp to edge wrapping.
   */
  void setIsMaxFiltering(bool isMaxFiltering);
  bool isMaxFiltering() const;

private:
  void releaseResources();

  Log log_;
  GLuint textureId_;
  // Rect imageRect_;
  bool isMaxFiltering_;

  // NonCopyable
  GlTexture(const GlTexture& c);
  GlTexture& operator=(const GlTexture& c);
};

#endif
