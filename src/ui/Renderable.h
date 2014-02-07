#ifndef UI_RENDERABLE_H
#define UI_RENDERABLE_H

#include <glm/fwd.hpp>


/**
 * Renderable class.
 * Interface for a renderable object.
 * Provides:
 *   - Model transform
 *   - Decal texture image
 *   // TODO swarminglogic, 2014-01-23: - Normal texture image
 *   // TODO swarminglogic, 2014-01-23: - Specular texture image
 *   - Shader program
 *   - Mesh data
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class Renderable
{
public:
  // Pure virtual functions
  virtual glm::Mat4 getModelTransform()          const = 0;
  virtual const ImageResource& getDecalTexture() const = 0;
  virtual const ShaderResource& getShader()      const = 0;
  virtual const MeshResource& getMesh()          const = 0;

private:
};

#endif
