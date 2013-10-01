#ifndef UI_CAMERASPHERICAL_H
#define UI_CAMERASPHERICAL_H

#include <memory>

#include <glm/glm.hpp>
#include <io/MouseDragInput.h>
#include <ui/BaseCamera.h>
#include <util/Log.h>
#include <util/SDL_Event_fwd.h>


/**
 * CameraSpherical class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class CameraSpherical : public BaseCamera
{
public:
  CameraSpherical();
  CameraSpherical(float radius, Pointf angles);
  virtual ~CameraSpherical();

  virtual glm::vec3 getPosition() const override;
  virtual glm::vec3 getTargetPosition() const override;
  virtual bool handleEvent(const SDL_Event& event) override;

  virtual void setPosition(const glm::vec3& position) override;
  virtual void setTargetPosition(const glm::vec3& target) override;

  virtual bool prefersMouseGrab() const override { return false; };

  // For direct input, these are independent of mouse/keyboard camera
  // positioning.
  // TODO swarminglogic, 2013-09-29: consider adding resetter to MouseDragInput
  void setAngles(Pointf angles);
  void setRadius(float r);


private:
  Log log_;

  Pointf angles_;
  float r_;
  glm::vec3 target_;
  std::unique_ptr<MouseDragInput> cameraAngle_;
  std::unique_ptr<MouseDragInput> cameraZoom_;

  // NonCopyable
  CameraSpherical(const CameraSpherical& c);
  CameraSpherical& operator=(const CameraSpherical& c);
};

#endif
