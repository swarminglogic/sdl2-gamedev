#include <io/ResourceManager.h>

#include <audio/MusicTrack.h>
#include <audio/SoundChunk.h>
#include <ui/Mesh.h>
#include <ui/Surface.h>
#include <util/Asset.h>
#include <util/FileUtil.h>


ResourceManager::ResourceManager()
  : log_("ResouceManager"),
    loadedImages_(),
    loadedMeshes_(),
    loadedShaders_()
{
}


ResourceManager::~ResourceManager()
{
}


SurfaceShPtr
ResourceManager::load(const AssetImage& image)
{
  log_.d() << "Trying to load image: " << image.path() << Log::end;
  SurfaceShPtr resource = loadedImages_[image];
  if (!resource) {
    resource.reset(new Surface(image));

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
  log_.d() << "Trying to load mesh: " << mesh.path() << Log::end;
  MeshShPtr resource = loadedMeshes_[mesh];
  if (!resource) {
    if (!FileUtil::exists(mesh.path())) {
      resource.reset();
    } else {
      resource.reset(new Mesh(mesh));
      loadedMeshes_[mesh] = resource;
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


MusicTrackShPtr ResourceManager::load(const AssetMusic& music)
{
  MusicTrackShPtr resource = loadedMusic_[music];
  if (!resource) {
    if (!FileUtil::exists(music.path())) {
      resource.reset();
    } else {
      resource.reset(new MusicTrack(music));
      loadedMusic_[music] = resource;
    }
  }
  return resource;
}


SoundChunkShPtr ResourceManager::load(const AssetSound& sound)
{
  log_.d() << "Trying to load sound: " << sound.path() << Log::end;
  SoundChunkShPtr resource = loadedSounds_[sound];
  if (!resource) {
    if (!FileUtil::exists(sound.path())) {
      resource.reset();
    } else {
      resource.reset(new SoundChunk(sound));
      loadedSounds_[sound] = resource;
    }
  }
  return resource;
}


SoundChunkShPtr ResourceManager::loadSound(const std::string& soundfile)
{
  return load(AssetSound(soundfile));
}


MusicTrackShPtr ResourceManager::loadMusic(const std::string&  musicfile)
{
  return load(AssetMusic(musicfile));
}


ShaderProgramShPtr ResourceManager::loadShader(const ShaderfileKey& shaders)
{
  ShaderKey key;
  for (auto& shader : shaders)
    key[shader.first] = AssetShader(shader.second);
  return load(key);
}


SurfaceShPtr ResourceManager::loadImage(const std::string&  imagefile)
{
  return load(AssetImage(imagefile));
}


MeshShPtr ResourceManager::loadMesh(const std::string&  meshfile)
{
  return load(AssetMesh(meshfile));
}
