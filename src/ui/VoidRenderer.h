#ifndef UI_VOIDRENDERER_H
#define UI_VOIDRENDERER_H

#include <ui/BasicRender.h>
#include <ui/SDL_opengl.h>


/**
 * VoidRenderer class.
 * Most basic renderer possible. Draws the opengl window with a solid color.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class VoidRenderer : public BasicRender
{
public:
  VoidRenderer();
  virtual ~VoidRenderer();

  virtual void initialize() override;
  virtual void render(float currentTime) const override;
  virtual void finalize() override;
  virtual void refresh() override;

private:
  GLuint vao_;

  // NonCopyable
  VoidRenderer(const VoidRenderer& c);
  VoidRenderer& operator=(const VoidRenderer& c);
};

#endif
