#ifndef CORE_CONFIGMANAGER_H
#define CORE_CONFIGMANAGER_H

#include <memory>


class LogManager;

/**
 * ConfigManager class.
 *
 * Singleton class for managing global application settings.
 * Should be made thread safe.
 *
 * @author SwarmingLogic
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

  std::unique_ptr<LogManager> logManager_;

  // NonCopyable
  ConfigManager(const ConfigManager& c);
  ConfigManager& operator=(const ConfigManager& c);
};

#endif
