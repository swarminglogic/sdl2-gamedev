#ifndef UI_IMAGERENDER_H
#define UI_IMAGERENDER_H

#include <glm/glm.hpp>
#include <math/Size.h>
#include <ui/BasicRender.h>
#include <ui/SDL_opengl.h>
#include <ui/ShaderProgram.h>
#include <ui/Surface.h>
#include <util/Log.h>


/**
 * ImageRender class.
 * Renders surfaces pixel perfect onto a quad, on screen.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class ImageRender : public BasicRender
{
public:
  ImageRender();
  virtual ~ImageRender();

  virtual void initialize() override;
  virtual void render(float currentTime) override;
  virtual void handleResize(int width, int height) override;

  void loadImage(const std::string& filename);
  void setSurface(SDL_Surface& surface);

  // Position: screen location in pixels, (0, 0) is top left.
  Point getPosition() const;
  void setPosition(Point position);
  void setPosition(int x, int y);

  /**
   * Sets zoom factor.
   *
   * Needs to be a power of two for pixel perfect rendering.
   *
   * @param zoomFactor
   */
  void setZoomFactor(unsigned char zoomFactor);

private:
  void prepareVertices();
  void prepareTexcoords();
  void updateQuad();
  void updateTex();
  void updateShader();

  Log log_;
  ShaderProgram program_;
  Size viewport_;
  Point position_;
  Surface surface_;
  unsigned char zoomFactor_;

  GLuint vertexBuffer_;
  GLuint textureBuffer_;
  std::vector<GLfloat> vertices_;
  std::vector<GLfloat> texcoords_;
  GLint viewportParamId_;
  GLint texParamId_;
  // NonCopyable
  ImageRender(const ImageRender& c);
  ImageRender& operator=(const ImageRender& c);
};

#endif
