#include <config/ConfigManager.h>

#include <iostream>

#include <config/ViewConfig.h>
#include <math/Size.h>
#include <ui/GraphicsManager.h>
#include <util/LogManager.h>


ConfigManager& ConfigManager::instance()
{
  static ConfigManager instance;
  return instance;
}


const LogManager& ConfigManager::getLogManager() const
{ return *logManager_; }
LogManager& ConfigManager::getLogManager()
{ return *logManager_; }

const ViewConfig& ConfigManager::getViewConfig() const
{ return *viewConfig_; }
ViewConfig& ConfigManager::getViewConfig()
{ return *viewConfig_; }



ConfigManager::ConfigManager()
  : logManager_(nullptr)
{
  // TODO swarminglogic, 2013-09-14: Load configuration from file, and set
  // values accordingly. Separate function so that reused for updating.

  // View config
  viewConfig_.reset(new ViewConfig(Size(800, 600),
                                   "SDL Game"));
  viewConfig_->setIsFullScreen(false);
  viewConfig_->setIsResizeable(false);
  viewConfig_->setIsVSync(true);

  // Logging
  logManager_.reset(new LogManager(LogManager::LEVEL_DEBUG,
                                   LogManager::LEVEL_DEBUG));
  logManager_->setLogfilePath("./log.txt");
}


ConfigManager::~ConfigManager()
{
}
