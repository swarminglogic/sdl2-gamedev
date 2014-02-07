#ifndef UI_TEXTUREDQUADRENDER_H
#define UI_TEXTUREDQUADRENDER_H

#include <glm/glm.hpp>
#include <math/Size.h>
#include <ui/BasicRender.h>
#include <ui/SDL_opengl.h>
#include <ui/ShaderProgram.h>
#include <ui/Surface.h>
#include <util/Asset.h>
#include <util/Log.h>


/**
 * TexturedQuadRender class.
 * Renders pixel perfect onto a quad, filling the whole screen.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class TexturedQuadRender : public BasicRender
{
public:
  TexturedQuadRender();
  virtual ~TexturedQuadRender();

  virtual void initialize() override;
  virtual void render(float currentTime) const override;
  virtual void refresh() override;
  virtual void handleResize(int width, int height) override;

  // Three ways to set up image data:
  //  1. loadImage: load an image from file, create Surface
  //  2. setSurface: use an SDL_Surface object
  //  3. setGlTextureId: use a pre-prepared OpenGL texture.

  void loadImage(const AssetImage& imagefile);
  void setSurface(SDL_Surface& surface);
  void setGlTextureId(GLuint textureId, Size size);

private:
  void updateShader();

  Log log_;
  ShaderProgram program_;
  Size viewport_;
  Rect rect_;
  Surface surface_;

  GLuint vertexBuffer_;
  std::vector<GLfloat> vertices_;
  GLint texParamId_;
  GLint timeParamId_;

  // NonCopyable
  TexturedQuadRender(const TexturedQuadRender& c);
  TexturedQuadRender& operator=(const TexturedQuadRender& c);
};

#endif
