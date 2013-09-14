#ifndef UI_GRAPHICSMANAGER_H
#define UI_GRAPHICSMANAGER_H

/**
 * GraphicsManager class.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class GraphicsManager
{
public:
  GraphicsManager();
  virtual ~GraphicsManager();

private:

  // NonCopyable
  GraphicsManager(const GraphicsManager& c);
  GraphicsManager& operator=(const GraphicsManager& c);
};

#endif
