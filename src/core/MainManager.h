#ifndef CORE_MAINMANAGER_H
#define CORE_MAINMANAGER_H

#include <memory>

#include <ui/GraphicsManager.h>

class Log;

/**
 * MainManager class for managing application internals
 * Initializes all core systems.
 * Errors and failures are reported to logger.
 *
 * Singleton class, as only one should ever be created.
 *
 * @author SwarmingLogic
 */
class MainManager
{
public:
  static MainManager& instance();

  virtual ~MainManager();

private:
  MainManager();

  void initSDL();
  void initSDLimg();
  void initSDLttf();
  void initSDLmixer();

  // NonCopyable
  MainManager(const MainManager& c);
  MainManager& operator=(const MainManager& c);

  std::unique_ptr<GraphicsManager> graphicsManager_;
  std::unique_ptr<Log> logger_;
};

#endif
