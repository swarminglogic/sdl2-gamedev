#ifndef UI_VOIDRENDERER_H
#define UI_VOIDRENDERER_H

#include <ui/BasicRender.h>


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

  virtual void initialize();
  virtual void render(float currentTime);
  virtual void finalize();

private:
  // NonCopyable
  VoidRenderer(const VoidRenderer& c);
  VoidRenderer& operator=(const VoidRenderer& c);
};

#endif
