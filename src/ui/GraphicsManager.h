#ifndef UI_GRAPHICSMANAGER_H
#define UI_GRAPHICSMANAGER_H

#include <ui/SDL.h>
#include <util/Log.h>


class ViewConfig;

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
  void initalizeOpenGL(const ViewConfig& viewConfig);

  Log logger_;
  WindowPtr window_;
  GLContextPtr context_;

  // NonCopyable
  GraphicsManager(const GraphicsManager& c);
  GraphicsManager& operator=(const GraphicsManager& c);
};

#endif
