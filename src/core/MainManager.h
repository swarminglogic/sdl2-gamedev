#ifndef CORE_MAINMANAGER_H
#define CORE_MAINMANAGER_H

#include <memory>

#include <ui/GraphicsManager.h>
#include <ui/ImageRender.h>
#include <ui/SDL_opengl.h>
#include <ui/SDL_ttf.h>
#include <ui/TextBoxTextRender.h>
#include <util/FpsCounter.h>


class BasicRender;
class Log;
class PhysicsWorld;
class ResourceManager;
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

  Uint32 getRuntimeTics() const;
  float getRuntimeSecs() const;
  float getCurrentTimeDelta() const;

  PhysicsWorld& getPhysicsWorld();

  ResourceManager& getResourceManager();

private:
  MainManager();
  void handleEvent(const SDL_Event& event);

  void initSDL();
  void initSDLimg();
  void initSDLttf();
  void initSDLmixer();

  void updateFpsText(double fps);

  // NonCopyable
  MainManager(const MainManager& c);
  MainManager& operator=(const MainManager& c);

  Log log_;
  std::unique_ptr<GraphicsManager> graphics_;
  std::unique_ptr<Timer> runtime_;
  std::unique_ptr<BasicRender> basicRender_;
  std::unique_ptr<PhysicsWorld> physics_;
  std::unique_ptr<ResourceManager> resouceManager_;

  // ImageRender fpsRender_;
  TextBoxTextRender textRenderer_;

  FontPtr font_;
  SDL_Color fontColor_;

  bool isRunning_;
  FpsCounter fpsCounter_;

  float currentTimeDelta_;
};

#endif
