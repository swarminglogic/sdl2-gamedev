#ifndef WIP_INSTANCEDCUBERENDERER_H
#define WIP_INSTANCEDCUBERENDERER_H

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math/Pointf.h>
#include <ui/BaseCamera.h>
#include <ui/BasicRender.h>
#include <ui/ImageRender.h>
#include <ui/SDL_opengl.h>
#include <ui/ShaderProgram.h>
#include <ui/Surface.h>
#include <util/Log.h>
#include <util/Timer.h>
#include <wip/CubeInstanced.h>


class MouseDragInput;
class Size;

/**
 * InstancedCubeRenderer class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class InstancedCubeRenderer : public BasicRender
{
public:
  InstancedCubeRenderer();
  virtual ~InstancedCubeRenderer();

  virtual void initialize() override;
  virtual void render(float currentTime) override;
  virtual bool handleEvent(const SDL_Event& event) override;
  virtual void handleResize(int width, int height) override;

  virtual bool prefersMouseGrab() const override;
private:
  void updateViewMatrix();
  void checkForLogError();

  Log log_;
  GLuint vao_;

  std::unique_ptr<BaseCamera> camera_;
  Timer timer_;

  glm::mat4 mvpMat_;
  glm::mat4 unit_;
  glm::mat4 projectionMat_, viewMat_, modelMat_;
  Size viewport_;

  GLfloat clearColor_[4];
  CubeInstanced cube_;

  // NonCopyable
  InstancedCubeRenderer(const InstancedCubeRenderer& c);
  InstancedCubeRenderer& operator=(const InstancedCubeRenderer& c);
};

#endif
