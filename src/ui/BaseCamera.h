#ifndef UI_BASECAMERA_H
#define UI_BASECAMERA_H

#include <vector>

#include <glm/glm.hpp>
#include <util/SDL_Event_fwd.h>


/**
 * BaseCamera class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class BaseCamera
{
public:
  BaseCamera(){}
  virtual ~BaseCamera(){}

  virtual glm::vec3 getPosition() const = 0;
  virtual glm::vec3 getTargetPosition() const = 0;
  virtual bool handleEvent(const SDL_Event&) { return false; }

  virtual void setPosition(const glm::vec3& position) = 0;
  virtual void setTargetPosition(const glm::vec3& target) = 0;

  /**
   * For handling event-asynchronous update.
   * E.g. when a key is down, no even't will be triggered, but it might still
   * require an update.
   */
  virtual bool update() { return false; }

  virtual bool prefersMouseGrab() const = 0;

private:
  // NonCopyable
  BaseCamera(const BaseCamera& c);
  BaseCamera& operator=(const BaseCamera& c);
};

#endif
