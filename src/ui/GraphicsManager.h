#ifndef UI_GRAPHICSMANAGER_H
#define UI_GRAPHICSMANAGER_H

#include <math/Size.h>
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

  bool isMouseGrab() const;
  void setIsMouseGrab(bool isMouseGrabEnabled);
  void toggleMouseGrab();

  Size getScreenSize() const;
  void setScreenSize(Size windowSize);
private:
  void initalizeOpenGL(const ViewConfig& viewConfig);

  Log log_;
  SdlWindowPtr window_;
  Size screenSize_;
  SdlGLContextPtr context_;
  bool isFullScreen_;
  bool isVSync_;
  bool isOpenGlDebugEnabled_;
  bool isMouseGrab_;

  // NonCopyable
  GraphicsManager(const GraphicsManager& c);
  GraphicsManager& operator=(const GraphicsManager& c);
};

#endif
