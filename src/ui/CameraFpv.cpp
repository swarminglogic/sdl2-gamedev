#include <ui/CameraFpv.h>

#include <glm/gtc/matrix_transform.hpp>
#include <io/Keyboard.h>
#include <ui/SDL.h>


CameraFpv::CameraFpv()
  :  CameraFpv(glm::vec3(1.0f), glm::vec3(0.0f))
{
}

CameraFpv::CameraFpv(const glm::vec3& pos,
                     const glm::vec3& target)
  : log_("CameraFpv"),
    pos_(pos),
    target_(target),
    vel_(0.0f),
    localTime_(),
    angles_(0.0f, 0.0f),
    targetDistanceOffset_(0.0f),
    cameraAngle_(new MouseRelMoveInput(0.2f,
                                       Pointf(0.0f, 0.0f)))
{
  localTime_.start();
}



CameraFpv::~CameraFpv()
{
}


glm::vec3 CameraFpv::getPosition() const
{
  return pos_;
}


glm::vec3 CameraFpv::getTargetPosition() const
{
  return target_;
}


bool CameraFpv::handleEvent(const SDL_Event& event)
{
  if (!SDL_GetRelativeMouseMode())
    return false;

  const bool angleChanged = cameraAngle_->handleEvent(event);
  if (angleChanged) {
    angles_ = cameraAngle_->getScaled();
    cameraAngle_->reset();
  }

  // TODO swarminglogic, 2013-10-01: Set up some kind of user
  // input to modify targetDistanceOffset_. It used to be this:
  // // if (targetChanged) {
  // //   targetDistanceOffset_ += cameraTarget_->getDragScaled().y();
  // //   cameraTarget_->reset();
  // // }
  // But with relative mouse movement, it's not feasible to use MouseDrag.

  if (angleChanged) {
    glm::vec3 deltaPos = target_ - pos_;
    const glm::vec3 rightAxis =
      glm::normalize(glm::cross(deltaPos, glm::vec3(0.0f, 1.0f, 0.0f)));
    const glm::vec3 upAxis = glm::normalize(glm::cross(rightAxis, deltaPos));

    // TODO swarminglogic, 2013-10-01: Part of above todo.
    // const glm::vec3 inAxis = glm::normalize(deltaPos);
    // deltaPos += inAxis * targetDistanceOffset_;

    const float delta = -angles_.x();
    const float phi   = -angles_.y();

    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f),
                                   delta, upAxis);
    rotate = glm::rotate(rotate, phi, rightAxis);
    glm::vec4 finalPosition = rotate * glm::vec4(deltaPos, 1.0f);
    target_ = pos_ + glm::vec3(finalPosition.x, finalPosition.y, finalPosition.z);
    angles_ = Pointf();
    return true;
  }
  return false;
}


void CameraFpv::setPosition(const glm::vec3& position)
{
  pos_ = position;
}

void CameraFpv::setTargetPosition(const glm::vec3& target)
{
  target_ = target;
}


float updateVelocity(float vel, float dt, bool isIncr, bool isDecr) {
  const float incrPrSec = 12.0f;
  const float maxVel = 10.0f;

  // Increment speed
  if (isIncr && !isDecr)
    vel += incrPrSec * dt * (vel < 0.0f ? 2 : 1);
  // Decrement speed
  else if (!isIncr && isDecr)
    vel -= incrPrSec * dt * (vel > 0.0f ? 2 : 1);
  // NEITHER, TOWARDS 0
  else if (!isIncr && !isDecr) {
    float diff = 2 * incrPrSec * dt;
    if (std::fabs(vel) < diff)
      vel = 0.0f;
    else
      vel += (vel < 0.0f) ? diff : -diff;
  }

  // Axis-bound speed
  vel = std::max(-maxVel, std::min(vel, maxVel));
  return vel;
}

bool CameraFpv::update()
{
  const Keyboard& kb = Keyboard::instance();

  const float dt = static_cast<float>(localTime_.reset()) / 1000.0f;
  localTime_.start();

  vel_.x = updateVelocity(vel_.x, dt,
                          kb.isDown(SDL_SCANCODE_D),
                          kb.isDown(SDL_SCANCODE_A));
  vel_.y = updateVelocity(vel_.y, dt,
                          kb.isDown(SDL_SCANCODE_E),
                          kb.isDown(SDL_SCANCODE_Q));
  vel_.z = updateVelocity(vel_.z, dt,
                          kb.isDown(SDL_SCANCODE_W),
                          kb.isDown(SDL_SCANCODE_S));

  const float maxVel = 10.0f;

  float velocity = std::max(0.0f, std::min(glm::length(vel_), maxVel));
  if (velocity > 0.1f ) {
    // Space-magnitude bound speed.
    glm::vec3 adjustedVelocity = velocity * glm::normalize(vel_);

    glm::vec3 move = adjustedVelocity * dt;

    // TODO swarminglogic, 2013-09-30: Can be optimized.
    glm::vec3 right = glm::normalize(glm::cross(target_ - pos_,
                                                glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 up = glm::normalize(glm::cross(right, target_ - pos_));
    glm::vec3 in = glm::normalize(target_ - pos_);

    glm::vec3 offset = (right * move.x +
                        up    * move.y +
                        in    * move.z);
    pos_    += offset;
    target_ += offset;
    return true;
  }
  else
    return false;
}
