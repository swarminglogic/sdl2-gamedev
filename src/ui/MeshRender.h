#ifndef UI_MESHRENDER_H
#define UI_MESHRENDER_H

#include <ui/BasicRender.h>
#include <ui/Mesh.h>
#include <util/Asset.h>


/**
 * MeshRender class.
 * Wrapper for rendering Mesh objects.
 * Has functions for assigning various textures.
 * Expects to be rendered with deferredobjrender.{vert|frag} shader.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class MeshRender : public BasicRender
{
public:
  MeshRender(MeshShPtr mesh);

  virtual ~MeshRender();
  virtual void render(float currentTime) override;

  void refresh();

private:
  MeshShPtr mesh_;

  // NonCopyable
  MeshRender(const MeshRender& c);
  MeshRender& operator=(const MeshRender& c);
};

#endif
