#ifndef CORE_MAINMANAGER_H
#define CORE_MAINMANAGER_H

#include <memory>

#include <ui/GraphicsManager.h>
#include <ui/SDL_opengl.h>


class BasicRender;
class Log;
class Timer;

/**
 * MainManager class for managing application internals
 * Initializes all core systems.
 * Errors and failures are reported to logger.
 *
 * Singleton class, as only one should ever be created.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class MainManager
{
public:
  static MainManager& instance();

  virtual ~MainManager();

  /**
   * Program main loop
   */
  void run();

  /**
   * Called prior to run()
   */
  void initialize();

  /**
   * Called after finalizing run() (exiting main loop)
   */
  void finalize();

private:
  MainManager();
  void handleEvent(const SDL_Event& event);

  void initSDL();
  void initSDLimg();
  void initSDLttf();
  void initSDLmixer();

  // NonCopyable
  MainManager(const MainManager& c);
  MainManager& operator=(const MainManager& c);

  Log logger_;
  std::unique_ptr<GraphicsManager> graphics_;
  std::unique_ptr<Timer> runtime_;
  std::unique_ptr<BasicRender> basicRender_;
  bool isRunning_;
};

#endif
