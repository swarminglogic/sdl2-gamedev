#ifndef UI_DEFERREDQUADRENDER_H
#define UI_DEFERREDQUADRENDER_H

#include <array>

#include <glm/glm.hpp>
#include <ui/BasicRender.h>
#include <ui/GlTexture.h>
#include <ui/SDL_opengl.h>
#include <ui/ShaderProgram.h>
#include <ui/Surface.h>
#include <util/Log.h>


/**
 * DeferredQuadRender class.
 * Renders pixel perfect onto a quad, filling the whole screen.
 * It makes use of N textures, denoted by the enum.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class DeferredQuadRender : public BasicRender
{
public:
  enum GBufferTarget{
    GBUFFERTARGET_POSITION,
    GBUFFERTARGET_NORMAL,
    GBUFFERTARGET_COLOR,
    N_GBUFFERTARGETS
  };
  DeferredQuadRender();
  virtual ~DeferredQuadRender();

  virtual void initialize() override;
  virtual void render(float currentTime) override;
  virtual void handleResize(int width, int height) override;

  // Three ways to set up image data:
  //  1. loadImage: load an image from file, create Surface
  //  2. setSurface: use an SDL_Surface object
  //  3. setGlTextureId: use a pre-prepared OpenGL texture.

  void setGlTextureId(GBufferTarget target, GLuint textureId);

  void setModelViewMat(const glm::mat4 modelViewMat);
  void setProjectionMat(const glm::mat4 projectionMat);

private:
  std::string getUniformName(GBufferTarget target);
  void updateShader();


  Log log_;
  ShaderProgram program_;
  Size viewport_;
  std::array<GlTexture, N_GBUFFERTARGETS> textures_;
  std::array<GLint, N_GBUFFERTARGETS> uniformTextureIds_;

  GLuint vertexBuffer_;
  std::vector<GLfloat> vertices_;
  GLint timeParamId_;

  GLint projectionMatID_;
  GLint modelViewMatID_;
  glm::mat4 modelViewMat_;
  glm::mat4 projectionMat_;
  GLint lightPosID_;
  glm::vec4 lightPos_;
  GLint viewportID_;
  GLint timeID_;

  struct SsaoData {
    glm::vec4 pos[64];
    glm::vec4 vec[64];
  };

  SsaoData ssaoData_;
  GLuint ssaoDataBuffer_;

  // NonCopyable
  DeferredQuadRender(const DeferredQuadRender& c);
  DeferredQuadRender& operator=(const DeferredQuadRender& c);
};

#endif
