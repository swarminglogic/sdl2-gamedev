#include <ui/CameraSpherical.h>



CameraSpherical::CameraSpherical()
  : CameraSpherical(3.3f, Pointf(0.47f, 0.45f))
{
}


CameraSpherical::CameraSpherical(float radius, Pointf angles)
  : log_("CameraSpherical"),
    angles_(angles),
    r_(radius),
    target_(0.0f),
    cameraAngle_(new MouseDragInput(MouseDragInput::MB_LEFT,
                                    0.005f,
                                    Pointf(0.0f, 0.0f))),
    cameraZoom_(new MouseDragInput(MouseDragInput::MB_RIGHT,
                                   0.03f,
                                   Pointf(0.0f, 0.0f)))
{
}


CameraSpherical::~CameraSpherical()
{
}


glm::vec3 CameraSpherical::getPosition() const
{
  const float theta = angles_.x() + cameraAngle_->getDragScaled().x();
  const float phi   = angles_.y() + cameraAngle_->getDragScaled().y();
  const float r     = r_ + cameraZoom_->getDragScaled().y();

  const float t = r * std::cos(phi);
  return glm::vec3(t * std::cos(theta),
                   r * std::sin(phi),
                   t * std::sin(theta));
}


glm::vec3 CameraSpherical::getTargetPosition() const
{
  return target_;
}


bool CameraSpherical::handleEvent(const SDL_Event& event)
{
  const bool angleChanged = cameraAngle_->handleEvent(event);
  const bool zoomChanged  = cameraZoom_->handleEvent(event);
  return angleChanged || zoomChanged;
}


void CameraSpherical::setPosition(const glm::vec3& pos)
{
  r_ = glm::length(pos);
  angles_ = Pointf(std::asin(pos.y / r_),
                   std::atan2(pos.z, pos.x));
}


void CameraSpherical::setTargetPosition(const glm::vec3& target)
{
  target_ = target;
}


void CameraSpherical::setAngles(Pointf angles)
{angles_ = angles;}
void CameraSpherical::setRadius(float r)
{r_ = r;}

