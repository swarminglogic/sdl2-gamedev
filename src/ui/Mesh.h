#ifndef UI_OBJMESH_H
#define UI_OBJMESH_H

#include <array>
#include <string>
#include <vector>

#include <memory>
#include <extern/tiny_obj_loader.h>
#include <ui/SDL_opengl.h>
#include <util/Asset.h>
#include <util/FileMonitor.h>
#include <util/Log.h>
#include <util/TimedExecution.h>


/**
 * Mesh class
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class Mesh
{
public:
  Mesh(const AssetMesh& meshfile);
  virtual ~Mesh();

  bool isEmpty() const;
  size_t getShapeCount() const;
  GLsizei getIndicesCount(size_t shapeIndex) const;
  GLuint getIndexBuffer(size_t shapeIndex) const;
  GLuint getVertexBuffer(size_t shapeIndex) const;
  GLuint getUVBuffer(size_t shapeIndex) const;
  GLuint getNormalBuffer(size_t shapeIndex) const;
  const tinyobj::material_t& getMaterial(size_t shapeIndex) const;
  tinyobj::shape_t& getShape(size_t shapeIndex) const;

  bool load();
  bool isUpdated();

  bool hasUVdata(size_t shapeIndex) const;
  bool hasNormalData(size_t shapeIndex) const;

private:
  void clear();

  Log log_;

  std::string filename_;

  std::vector<tinyobj::shape_t> shapes_;

  struct BufferGroup
  {
    GLuint index;
    GLuint vertex;
    GLuint uv;
    GLuint normal;
  };

  // size_t shapeIndex_;
  std::vector<BufferGroup> bufferIds_;
  FileMonitor fileMonitor_;

  // Using TimedExecution as local guard.  Since it only accepts void(void), I'm
  // caching the result in member variable 'cached_isShadersModified_'.
  void guard_isObjModified();
  bool cached_isObjModified_;
  TimedExecution guard_isObjModified_;

  Timer timer_;

  // NonCopyable
  Mesh(const Mesh& c);
  Mesh& operator=(const Mesh& c);
};

typedef std::shared_ptr<Mesh> MeshShPtr;

#endif
