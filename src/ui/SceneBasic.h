#ifndef UI_SCENEBASIC_H
#define UI_SCENEBASIC_H

#include <array>
#include <memory>
#include <vector>

#include <glm/fwd.hpp>
#include <ui/RenderableBasic.h>
#include <ui/SceneI.h>
#include <util/Averager.h>
#include <util/Log.h>
#include <util/SDL_Event_fwd.h>


class BaseCamera;
class RenderableI;


/**
 * SceneBasic class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class SceneBasic : public SceneI
{
public:
  SceneBasic();
  virtual ~SceneBasic();

  virtual std::vector<const RenderableI*> getSceneObjects() const override;
  virtual std::vector<RenderableI*> getSceneObjects() override;
  virtual glm::mat4 getViewMatrix() const override;
  virtual bool handleEvent(const SDL_Event& event) override;
  virtual glm::mat3 getNormalMatrix() const override;
  virtual void refresh() override;

private:
  void updateViewMatrix();
  Log log_;

  std::unique_ptr<RenderableBasic> sceneBox_;
  std::unique_ptr<RenderableBasic> sceneObjects_;
  std::unique_ptr<BaseCamera> camera_;
  glm::mat4 viewMat_;
  glm::mat3 normalMat_;
  std::array<Averager, 6> cameraMotion_;

  // NonCopyable
  SceneBasic(const SceneBasic& c);
  SceneBasic& operator=(const SceneBasic& c);
};

#endif
