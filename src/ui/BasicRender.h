#ifndef WIP_BASICRENDER_H
#define WIP_BASICRENDER_H

#include <util/SDL_Event_fwd.h>


/**
 * BasicRender class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class BasicRender
{
public:
  BasicRender(){}
  virtual ~BasicRender(){}

  virtual void initialize() = 0;
  virtual void render(float currentTime) = 0;
  virtual void finalize() = 0;

  virtual bool handleEvent(const SDL_Event&) { return false; }

private:
  // NonCopyable
  BasicRender(const BasicRender& c);
  BasicRender& operator=(const BasicRender& c);
};

#endif
