#include <core/ConfigManager.h>

#include <iostream>

#include <ui/GraphicsManager.h>
#include <util/LogManager.h>


ConfigManager& ConfigManager::instance()
{
  static ConfigManager instance;
  return instance;
}


const LogManager& ConfigManager::getLogManager() const
{
  return *logManager_;
}


ConfigManager::ConfigManager()
  : logManager_(nullptr)
{
  logManager_.reset(new LogManager(LogManager::LEVEL_DEBUG,
                                   LogManager::LEVEL_DEBUG));
  logManager_->setLogfilePath("./log.txt");
}


ConfigManager::~ConfigManager()
{
}

