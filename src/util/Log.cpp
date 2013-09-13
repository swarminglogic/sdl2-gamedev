#include <util/Log.h>

#include <core/ConfigManager.h>
#include <util/LogManager.h>


Log::Log(const std::string& loggerName)
  : loggerName_(loggerName)
{
}

void Log::debug(const std::string& message) const
{
#ifndef LOGGING_DISABLED
  ConfigManager::instance().getLogManager().log(LogManager::LEVEL_DEBUG,
                                                loggerName_,
                                                message);
#endif
}

void Log::info(const std::string& message) const
{
#ifndef LOGGING_DISABLED
  ConfigManager::instance().getLogManager().log(LogManager::LEVEL_INFO,
                                                loggerName_,
                                                message);
#endif
}

void Log::warning(const std::string& message) const
{
#ifndef LOGGING_DISABLED
  ConfigManager::instance().getLogManager().log(LogManager::LEVEL_WARNING,
                                                loggerName_,
                                                message);
#endif
}

void Log::error(const std::string& message) const
{
#ifndef LOGGING_DISABLED
  ConfigManager::instance().getLogManager().log(LogManager::LEVEL_ERROR,
                                                loggerName_,
                                                message);
#endif
}

Log::~Log()
{
}
