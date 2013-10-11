#ifndef WIP_TOTEXTURERENDERER_H
#define WIP_TOTEXTURERENDERER_H

#include <array>
#include <memory>

#include <glm/glm.hpp>
#include <math/Size.h>
#include <ui/BasicRender.h>
#include <ui/ObjMesh.h>
#include <ui/SDL_opengl.h>
#include <ui/ShaderProgram.h>
#include <ui/Surface.h>
#include <ui/TexturedQuadRender.h>
#include <util/Averager.h>
#include <util/Log.h>


class BaseCamera;

/**
 * ToTextureRenderer class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class ToTextureRenderer : public BasicRender
{
public:
  ToTextureRenderer();
  virtual ~ToTextureRenderer();

  virtual void initialize() override;
  virtual void render(float currentTime) override;
  virtual bool handleEvent(const SDL_Event& event) override;
  virtual void handleResize(int width, int height) override;
  virtual bool prefersMouseGrab() const override;
private:
  void updateShader();
  void updateModel();
  void updateViewMatrix();
  void updateToTextureRenderFBO();

  Log log_;
  GLuint vao_;
  GLuint frameBuffer_;

  std::unique_ptr<BaseCamera> camera_;
  glm::mat4 mvpMat_;
  glm::mat4 unit_;
  glm::mat4 projectionMat_, viewMat_, modelMat_;
  glm::mat3 normalMat_;
  Size viewport_;

  // Regular render members
  Surface texture_;
  ObjMesh mesh_;
  ShaderProgram shader_;
  GLint mvpID_;
  GLint modelViewMatID_;
  GLint normalMatID_;
  GLint textureId_;

  // To texture members
  GLuint fboHandle_;
  GLuint renderTexture_;
  GLuint depthBuffer_;
  TexturedQuadRender quadRender_;

  // Camera motion smoother
  std::array<Averager, 6> cameraMotion_;

  // NonCopyable
  ToTextureRenderer(const ToTextureRenderer& c);
  ToTextureRenderer& operator=(const ToTextureRenderer& c);
};

#endif
