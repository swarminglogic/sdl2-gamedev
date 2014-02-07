#ifndef UI_SCENEI_H
#define UI_SCENEI_H

#include <vector>

#include <glm/fwd.hpp>
#include <util/SDL_Event_fwd.h>


class RenderableI;

/**
 * SceneInterface class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class SceneI
{
public:
  virtual std::vector<const RenderableI*> getSceneObjects() const = 0;
  virtual std::vector<RenderableI*> getSceneObjects() = 0;
  virtual glm::mat4 getViewMatrix() const = 0;
  virtual bool handleEvent(const SDL_Event& event) = 0;

  // TODO swarminglogic, 2014-02-04: What is this for?
  virtual glm::mat3 getNormalMatrix() const = 0;

  virtual void refresh() {
    for (auto& renderable : getSceneObjects())
      renderable->refresh();
  }
private:
};

#endif
