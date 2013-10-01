#ifndef UI_CAMERAFPV_H
#define UI_CAMERAFPV_H

#include <memory>

#include <glm/glm.hpp>
#include <io/MouseRelMoveInput.h>
#include <math/Pointf.h>
#include <ui/BaseCamera.h>
#include <util/Log.h>
#include <util/Timer.h>


/**
 * CameraFpv class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class CameraFpv : public BaseCamera
{
public:
  CameraFpv();
  CameraFpv(const glm::vec3& pos,
            const glm::vec3& target);
  virtual ~CameraFpv();

  virtual glm::vec3 getPosition() const override;
  virtual glm::vec3 getTargetPosition() const override;
  virtual bool handleEvent(const SDL_Event& event) override;
  virtual void setPosition(const glm::vec3& position) override;
  virtual void setTargetPosition(const glm::vec3& target) override;
  virtual bool update() override;

  virtual bool prefersMouseGrab() const override { return true; };

private:
  Log log_;
  glm::vec3 pos_;
  glm::vec3 target_;
  glm::vec3 vel_;
  Timer localTime_;

  Pointf angles_;
  float targetDistanceOffset_;
  std::unique_ptr<MouseRelMoveInput> cameraAngle_;

  // NonCopyable
  CameraFpv(const CameraFpv& c);
  CameraFpv& operator=(const CameraFpv& c);
};

#endif
