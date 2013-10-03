#include <wip/InstancedCubeRenderer.h>

#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>

#include <SDL2/SDL_test_common.h>

#include <io/MouseDragInput.h>
#include <math/Size.h>
#include <ui/CameraFpv.h>
#include <ui/CameraSpherical.h>
#include <ui/GlUtil.h>
#include <ui/SDL.h>
#include <ui/SDL_ttf.h>
#include <ui/ShaderUtil.h>
#include <util/Asset.h>


InstancedCubeRenderer::InstancedCubeRenderer()
  : log_("InstancedCubeRenderer"),
    vao_(0),
    camera_(nullptr),
    timer_(),
    mvpMat_(),
    unit_(1.0f),
    projectionMat_(glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f)),
    viewMat_(),
    modelMat_(1.0f),
    viewport_(0, 0),
    clearColor_(),
    cube_()
{
  // camera_.reset(new CameraSpherical(3.3f, Pointf(0.47f, 0.45f)));
  camera_.reset(new CameraFpv(glm::vec3(3.3f), glm::vec3(0.0f)));
  updateViewMatrix();
  timer_.start();
}


InstancedCubeRenderer::~InstancedCubeRenderer()
{
}


void InstancedCubeRenderer::checkForLogError() {
  const GLenum err = glGetError();
  if (err == GL_NO_ERROR)
    return;

  if (err == GL_INVALID_ENUM)
    log_.e("OpenGL error: GL_INVALID_ENUM");
  else if (err == GL_INVALID_VALUE)
    log_.e("OpenGL error: GL_INVALID_VALUE");
  else
    log_.e() << "OpenGL error: " << static_cast<int>(err) << Log::end;
}



void InstancedCubeRenderer::initialize()
{
  log_.d("Initializing");
  glGenVertexArrays(1, &vao_);
  glBindVertexArray(vao_);

  // Prepare vertices
  clearColor_[0] = 0.1f;
  clearColor_[1] = 0.2f;
  clearColor_[2] = 0.25f;
  clearColor_[3] = 1.0f;

  cube_.initialize();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


void InstancedCubeRenderer::render(float time)
{
  (void)time;
  if (camera_->update())
    updateViewMatrix();

  glClearBufferfv(GL_COLOR, 0, clearColor_);
  glClear(GL_DEPTH_BUFFER_BIT);
  // glClear(GL_ACCUM_BUFFER_BIT);

  // TODO swarminglogic, 2013-09-30: WHO DOES GLBLEND FAIL HERE?
  // glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  // std::vector<glm::vec3> bokehs = camera_->getBokehPositions(10);
  // for (size_t i = 0 ; i < bokehs.size() ; ++i) {
  //   viewMat_ = glm::lookAt(camera_->getPosition() + bokehs[i],
  //                          camera_->getTargetPosition(),
  //                          glm::vec3(0.0f, 1.0f, 0.0f));
  //   mvpMat_ = projectionMat_ * viewMat_;
  //   cube_.setPvPointer(&mvpMat_[0][0]);
  //   cube_.render(time);
  //   glAccum(i ? GL_ACCUM : GL_LOAD,
  //           1.0f / (float)bokehs.size());
  // }
  // glAccum(GL_RETURN, 1);


  mvpMat_ = projectionMat_ * viewMat_;
  cube_.setPvPointer(&mvpMat_[0][0]);
  cube_.render(time);
}


bool InstancedCubeRenderer::handleEvent(const SDL_Event& event)
{
  const bool cameraChanged = camera_->handleEvent(event);
  if (cameraChanged)
    updateViewMatrix();
  return cameraChanged;
}


void InstancedCubeRenderer::handleResize(int width, int height)
{
  BasicRender::handleResize(width, height);
  float aspect = static_cast<float>(width) / static_cast<float>(height);
  projectionMat_ = glm::perspective(45.0f, aspect, 0.1f, 100.0f);
  viewport_ = Size(width, height);
}


bool InstancedCubeRenderer::prefersMouseGrab() const
{
  return camera_->prefersMouseGrab();
}


void InstancedCubeRenderer::updateViewMatrix()
{
  viewMat_ = glm::lookAt(camera_->getPosition(),
                         camera_->getTargetPosition(),
                         glm::vec3(0.0f, 1.0f, 0.0f));
}
