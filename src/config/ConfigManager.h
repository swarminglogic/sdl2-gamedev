#ifndef CONFIG_CONFIGMANAGER_H
#define CONFIG_CONFIGMANAGER_H

#include <memory>


class LogManager;
class Timer;
class ViewConfig;

/**
 * ConfigManager class.
 *
 * Singleton class for managing global application settings.
 * Should be made thread safe.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class ConfigManager
{
public:
  static ConfigManager& instance();
  virtual ~ConfigManager();

  const LogManager& getLogManager() const;

private:
  // Singleton
  ConfigManager();

  std::unique_ptr<ViewConfig> viewConfig_;
  std::unique_ptr<LogManager> logManager_;
  std::unique_ptr<Timer> runtime_;

  // NonCopyable
  ConfigManager(const ConfigManager& c);
  ConfigManager& operator=(const ConfigManager& c);
};

#endif
