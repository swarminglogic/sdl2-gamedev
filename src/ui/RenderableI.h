#ifndef UI_RENDERABLEI_H
#define UI_RENDERABLEI_H

#include <glm/fwd.hpp>
#include <ui/MeshRender.h>
#include <ui/ShaderProgram.h>
#include <ui/Surface.h>


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
class RenderableI
{
public:
  // Pure virtual functions
  virtual glm::mat4 getModelTransform() const  = 0;
  virtual const SurfaceShPtr getDecalTexture() const = 0;
  // virtual ShaderProgramShPtr getShader()  = 0;
  virtual const MeshRender& getMeshRender() const = 0;

  virtual void refresh() = 0;
private:
};

#endif
