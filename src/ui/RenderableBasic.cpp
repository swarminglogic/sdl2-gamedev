#include <ui/RenderableBasic.h>

#include <core/MainManager.h>


RenderableBasic::RenderableBasic() :
  log_("RenderableBasic"),
  modelTransform_(1.0f)
{
}


RenderableBasic::RenderableBasic(const std::string& meshfile,
                                 const std::string& decalfile,
                                 float decalRepeat)
  : RenderableBasic()
{
  auto mesh = MainManager::resources().loadMesh(meshfile);
  setMeshRender(MeshRenderShPtr{new MeshRender(mesh)});

  auto decal = MainManager::resources().loadImage(decalfile);
  decal->setIsMaxFiltering(true);
  decal->prepareForGl();
  decal->setRepeatFactorHint(decalRepeat);
  setDecalTexture(decal);
}


RenderableBasic::~RenderableBasic()
{
}


glm::mat4 RenderableBasic::getModelTransform() const
{
  return modelTransform_;
}

const SurfaceShPtr RenderableBasic::getDecalTexture() const
{
  return decalTexture_;
}

const MeshRender& RenderableBasic::getMeshRender() const
{
  assert(meshRenderer_);
  return *meshRenderer_;
}

void RenderableBasic::refresh()
{
  meshRenderer_->refresh();
}


void RenderableBasic::setDecalTexture(SurfaceShPtr decalTexture)
{
  decalTexture_ = decalTexture;
}

void RenderableBasic::setMeshRender(MeshRenderShPtr meshRenderer)
{
  meshRenderer_ = meshRenderer;
}

void RenderableBasic::setModelTransform(glm::mat4 modelTransform)
{
  modelTransform_ = modelTransform;
}
