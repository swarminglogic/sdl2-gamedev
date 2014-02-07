#ifndef UI_RENDERABLEBASIC_H
#define UI_RENDERABLEBASIC_H

#include <glm/glm.hpp>
#include <ui/RenderableI.h>
#include <ui/SceneI.h>


/**
 * RenderableBasic class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class RenderableBasic : public RenderableI
{
public:
  RenderableBasic();
  RenderableBasic(const std::string& meshfile,
                  const std::string& decalfile,
                  float decalRepeat);

  virtual ~RenderableBasic();

  virtual glm::mat4 getModelTransform() const  override;
  virtual const SurfaceShPtr getDecalTexture() const override;
  virtual const MeshRender& getMeshRender() const override;

  // virtual glm::mat4 getModelTransform()   override;
  // virtual SurfaceShPtr getDecalTexture()  override;
  // virtual MeshRender& getMeshRender() override;
  virtual void refresh() override;

  void setDecalTexture(SurfaceShPtr decalTexture);
  void setMeshRender(MeshRenderShPtr meshRenderer);
  void setModelTransform(glm::mat4 modelTransform);

private:
  Log log_;

  SurfaceShPtr decalTexture_;
  MeshRenderShPtr meshRenderer_;
  glm::mat4 modelTransform_;

  // NonCopyable
  RenderableBasic(const RenderableBasic& c);
  RenderableBasic& operator=(const RenderableBasic& c);
};

#endif
