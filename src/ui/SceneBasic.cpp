#include <ui/SceneBasic.h>

#include <core/MainManager.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ui/BaseCamera.h>
#include <ui/CameraFpv.h>
#include <ui/RenderableI.h>


SceneBasic::SceneBasic()
  : log_("SceneBasic"),
    sceneBox_(new RenderableBasic("scenebox.cobj", "uv_browngrid.png", 20.0f)),
    sceneObjects_(new RenderableBasic("sss.cobj", "uv_colorgrid.png", 1.0f)),
    viewMat_(1.0f),
    normalMat_(1.0f),
    cameraMotion_{8, 8, 8, 8, 8, 8}
{
  camera_.reset(new CameraFpv(glm::vec3(3.3f), glm::vec3(0.0f)));

  // Loading the objects in the scene:
}


SceneBasic::~SceneBasic()
{
}


std::vector<const RenderableI*> SceneBasic::getSceneObjects() const
{
  // return std::vector<const RenderableI*>{sceneObjects_.get(), sceneBox_.get()};
  return std::vector<const RenderableI*> {
    sceneBox_.get(),
    sceneObjects_.get()
  };
}


std::vector<RenderableI*> SceneBasic::getSceneObjects()
{
  return std::vector<RenderableI*> {
    sceneBox_.get(),
    sceneObjects_.get()
  };
}


glm::mat4 SceneBasic::getViewMatrix() const
{
  return viewMat_;
}


bool SceneBasic::handleEvent(const SDL_Event& event)
{
  const bool cameraChanged = camera_->handleEvent(event);
  return cameraChanged;
}


glm::mat3 SceneBasic::getNormalMatrix() const
{
  return normalMat_;
}

void SceneBasic::refresh()
{
  SceneI::refresh();
  camera_->update();
  updateViewMatrix();
}


void SceneBasic::updateViewMatrix() {
  const glm::vec3& pos    = camera_->getPosition();
  const glm::vec3& target = camera_->getTargetPosition();

  cameraMotion_[0].insertValue(pos.x);
  cameraMotion_[1].insertValue(pos.y);
  cameraMotion_[2].insertValue(pos.z);
  cameraMotion_[3].insertValue(target.x);
  cameraMotion_[4].insertValue(target.y);
  cameraMotion_[5].insertValue(target.z);

  viewMat_ = glm::lookAt(glm::vec3(cameraMotion_[0].getAverage(),
                                   cameraMotion_[1].getAverage(),
                                   cameraMotion_[2].getAverage()),
                         glm::vec3(cameraMotion_[3].getAverage(),
                                   cameraMotion_[4].getAverage(),
                                   cameraMotion_[5].getAverage()),
                         glm::vec3(0.0f, 1.0f, 0.0f));

  normalMat_ = glm::mat3(glm::vec3(viewMat_[0]),
                         glm::vec3(viewMat_[1]),
                         glm::vec3(viewMat_[2]));
}
