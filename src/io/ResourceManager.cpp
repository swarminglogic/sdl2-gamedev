#include <io/ResourceManager.h>

#include <ui/Mesh.h>
#include <ui/Surface.h>
#include <util/Asset.h>
#include <util/FileUtil.h>


ResourceManager::ResourceManager()
  : log_("ResouceManager")
{
}


ResourceManager::~ResourceManager()
{
}


SurfaceShPtr
ResourceManager::load(const AssetImage& image)
{
  SurfaceShPtr resource = loadedImages_[image];
  if (!resource) {
    resource.reset(new Surface);
    resource->loadImage(image);

    // Check that load went ok, and store in table if so
    if (resource->getSurface())
      loadedImages_[image] = resource;
    else
      resource.reset();
  }
  return resource;
}


MeshShPtr ResourceManager::load(const AssetMesh& mesh)
{
  MeshShPtr resource = loadedMeshes_[mesh];
  if (!resource) {
    try {
      resource.reset(new Mesh(mesh));
      resource->load();
      if (!resource->isEmpty())
        loadedMeshes_[mesh] = resource;
      else
        resource.reset();
    } catch (const Exception& e) {
      resource.reset();
    }
  }
  return resource;
}


ShaderProgramShPtr ResourceManager::load(const ShaderKey& shaders)
{
  ShaderProgramShPtr resource = loadedShaders_[shaders];
  if (!resource) {
    // Simple check to see that all files exist:
    for (auto& shader : shaders) {
      if (!FileUtil::exists(shader.second.path())) {
        resource.reset();
        return resource;
      }
    }
    resource.reset(new ShaderProgram(shaders));
    loadedShaders_[shaders] = resource;
  }
  return resource;
}
