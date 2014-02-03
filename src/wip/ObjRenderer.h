#ifndef WIP_OBJRENDERER_H
#define WIP_OBJRENDERER_H

#include <memory>

#include <glm/glm.hpp>
#include <math/Size.h>
#include <ui/BasicRender.h>
#include <ui/Mesh.h>
#include <ui/SDL_opengl.h>
#include <ui/ShaderProgram.h>
#include <ui/Surface.h>


class BaseCamera;

/**
 * ObjRenderer class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class ObjRenderer : public BasicRender
{
public:
  ObjRenderer();
  virtual ~ObjRenderer();

  virtual void initialize() override;
  virtual void render(float currentTime) override;
  virtual bool handleEvent(const SDL_Event& event) override;
  virtual void handleResize(int width, int height) override;
  virtual bool prefersMouseGrab() const override;
private:
  void updateShader();
  void updateModel();

  void updateViewMatrix();
  GLuint vao_;
  GLuint frameBuffer_;

  std::unique_ptr<BaseCamera> camera_;
  glm::mat4 mvpMat_;
  glm::mat4 unit_;
  glm::mat4 projectionMat_, viewMat_, modelMat_;
  Size viewport_;

  Surface texture_;
  Mesh mesh_;
  ShaderProgram shader_;
  GLint mvpID_;
  GLint textureId_;
  GLint diffuseColorId_;

  // NonCopyable
  ObjRenderer(const ObjRenderer& c);
  ObjRenderer& operator=(const ObjRenderer& c);
};

#endif
