#ifndef WIP_BASICRENDER_H
#define WIP_BASICRENDER_H

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

private:
  // NonCopyable
  BasicRender(const BasicRender& c);
  BasicRender& operator=(const BasicRender& c);
};

#endif
