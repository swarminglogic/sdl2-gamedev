#ifndef UI_SCENEINTERFACE_H
#define UI_SCENEINTERFACE_H

/**
 * SceneInterface class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class SceneInterface
{
public:
  

private:
  SceneInterface();
  virtual ~SceneInterface();


  // NonCopyable
  SceneInterface(const SceneInterface& c);
  SceneInterface& operator=(const SceneInterface& c);
};

#endif
