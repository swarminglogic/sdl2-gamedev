#ifndef IO_RESOURCEMANAGER_H
#define IO_RESOURCEMANAGER_H

#include <map>
#include <memory>
#include <set>
#include <string>

#include <ui/ShaderProgram.h>
#include <unordered_map>
#include <util/Asset.h>
#include <util/Log.h>


class Mesh;
class MusicTrack;
class SoundChunk;
class Surface;

/**
 * ResourceManager class.
 *
 * Manages resource files, making sure that only one
 * file is loaded to memory at one point.
 *
 * Future:
 * // TODO swarminglogic, 2014-02-08: Separate the resource manager into
 *    components 1:ui (image,mesh,shader), 2:audio {music/sound}
 *
 * // TODO swarminglogic, 2014-01-27:
 * // 1: Extend to asynchronously load files, in a non-blocking way.
 * // 2: Maintain a resource alive even though single user abandoned it.
 * // 3: Check if resource has been modified. Request file.
 * //    This does not alter current memory pointed to, unless all users have
 * //    abandoned old content. What is the GUID then? File path not enough.
 * //    Is this also only useful when developing?
 * //
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class ResourceManager
{
public:
  typedef std::shared_ptr<Surface> SurfaceShPtr;
  typedef std::shared_ptr<Mesh> MeshShPtr;
  typedef std::shared_ptr<MusicTrack> MusicTrackShPtr;
  typedef std::shared_ptr<SoundChunk> SoundChunkShPtr;
  typedef std::map<ShaderProgram::ShaderType, AssetShader> ShaderKey;
  typedef std::map<ShaderProgram::ShaderType, std::string> ShaderfileKey;

  ResourceManager();
  virtual ~ResourceManager();

  /**
   * Returns shared pointer to Surface with the image loaded.
   * If the image isn't found, it returns a nullptr.
   */
  SurfaceShPtr load(const AssetImage& image);
  SurfaceShPtr loadImage(const std::string&  imagefile);

  MeshShPtr load(const AssetMesh& mesh);
  MeshShPtr loadMesh(const std::string&  meshfile);

  MusicTrackShPtr load(const AssetMusic& music);
  MusicTrackShPtr loadMusic(const std::string&  musicfile);

  SoundChunkShPtr load(const AssetSound& sound);
  SoundChunkShPtr loadSound(const std::string&  soundfile);

  ShaderProgramShPtr load(const ShaderKey& shaders);
  ShaderProgramShPtr loadShader(const ShaderfileKey& shaders);

private:
  Log log_;

  std::map<AssetImage, SurfaceShPtr>       loadedImages_;
  std::map<AssetMesh,  MeshShPtr>          loadedMeshes_;
  std::map<ShaderKey,  ShaderProgramShPtr> loadedShaders_;
  std::map<AssetMusic, MusicTrackShPtr>    loadedMusic_;
  std::map<AssetSound, SoundChunkShPtr>    loadedSounds_;

  // NonCopyable
  ResourceManager(const ResourceManager& c);
  ResourceManager& operator=(const ResourceManager& c);
};

#endif
