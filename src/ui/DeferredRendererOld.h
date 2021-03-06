#ifndef UI_DEFERREDRENDEREROLD_H
#define UI_DEFERREDRENDEREROLD_H

#include <array>
#include <memory>

#include <glm/glm.hpp>
#include <math/Size.h>
#include <ui/BasicRender.h>
#include <ui/DeferredQuadRender.h>
#include <ui/MeshRender.h>
#include <ui/SDL_opengl.h>
#include <ui/ShaderProgram.h>
#include <ui/Surface.h>
#include <util/Averager.h>
#include <util/Log.h>


class BaseCamera;

/**
 * DeferredRendererOld class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class DeferredRendererOld : public BasicRender
{
public:
  DeferredRendererOld();
  virtual ~DeferredRendererOld();

  virtual void initialize() override;
  virtual void render(float currentTime) const override;
  //  virtual void render(float currentTime) override;
  virtual bool handleEvent(const SDL_Event& event) override;
  virtual void handleResize(int width, int height) override;
  virtual bool prefersMouseGrab() const override;

  virtual void refresh() override;

private:
  void updateShader();
  void updateViewMatrix();
  void updateToTextureRenderFBO();
  void setTextureInterp();

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
  SurfaceShPtr texture_;
  MeshRender mesh_;

  SurfaceShPtr sceneBoxTexture_;
  MeshRender sceneBox_;
  ShaderProgramShPtr shader_;
  GLint mvpID_;
  GLint modelViewMatID_;
  GLint projectionMatID_;
  GLint normalMatID_;
  GLint textureId_;
  GLint textureRepeatID_;

  // To texture members
  GLuint fboHandle_;
  GLuint positionTexture_;
  GLuint normalTexture_;
  GLuint colorTexture_;
  GLuint depthBuffer_;
  DeferredQuadRender quadRender_;

  // Camera motion smoother
  std::array<Averager, 6> cameraMotion_;

  // NonCopyable
  DeferredRendererOld(const DeferredRendererOld& c);
  DeferredRendererOld& operator=(const DeferredRendererOld& c);
};

#endif
