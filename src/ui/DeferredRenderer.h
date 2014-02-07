#ifndef UI_DEFERREDRENDERER_H
#define UI_DEFERREDRENDERER_H

#include <array>
#include <memory>

#include <ui/BasicRender.h>
#include <ui/DeferredQuadRender.h>
#include <ui/SDL_opengl.h>
#include <util/Log.h>

class SceneI;

/**
 * DeferredRenderer class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class DeferredRenderer : public BasicRender
{
public:
  DeferredRenderer();
  virtual ~DeferredRenderer();

  virtual void initialize() override;
  virtual void render(float currentTime) const override;
  virtual void refresh() override;
  virtual bool handleEvent(const SDL_Event& event) override;
  virtual void handleResize(int width, int height) override;
  virtual bool prefersMouseGrab() const override;

  virtual void setScene(std::shared_ptr<const SceneI> scene);

private:
  void updateShader();
  void updateDeferredRendererFBO();
  void setTextureInterp();

  Log log_;
  std::shared_ptr<const SceneI> scene_;

  // Window & camera
  Size viewport_;
  glm::mat4 projectionMat_;

  // Deferred renderer
  GLuint vertexArrayObject_;
  GLuint frameBuffer_;
  GLuint fboHandle_;

  // Shader
  ShaderProgramShPtr shader_;
  GLint mvpID_;
  GLint modelViewMatID_;
  GLint normalMatID_;
  GLint textureId_;
  GLint textureRepeatID_;

  GLuint positionTexture_;
  GLuint normalTexture_;
  GLuint colorTexture_;
  GLuint depthBuffer_;
  DeferredQuadRender quadRender_;

  // NonCopyable
  DeferredRenderer(const DeferredRenderer& c);
  DeferredRenderer& operator=(const DeferredRenderer& c);
};

#endif
