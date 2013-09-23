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

  /**
   * Swaps the OpenGL buffers.
   */
  void swapBuffers();

  // FullScreen toggle
  void toggleFullScreen();
  void setFullScreen(bool isFullScreenEnabled);
  bool isFullScreen() const;

  // VSync toggle
  void toggleVSync();
  bool isVSync() const;
  void setIsVSync(bool isVSyncEnabled);

  // Debug function for logging graphics driver information
  void logGraphicsDriverInfo() const;
  void logOpenGLContextInfo() const;
  void logStaticOpenGLInfo() const;

private:
  void initalizeOpenGL(const ViewConfig& viewConfig);

  Log log_;
  WindowPtr window_;
  GLContextPtr context_;
  bool isFullScreen_;
  bool isVSync_;
  bool isOpenGlDebugEnabled_;

  // NonCopyable
  GraphicsManager(const GraphicsManager& c);
  GraphicsManager& operator=(const GraphicsManager& c);
};

#endif
