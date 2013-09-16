#include <util/Log.h>

#include <cassert>

#include <config/ConfigManager.h>
#include <util/Exception.h>
#include <util/LogManager.h>


Log::Log(const std::string& loggerName)
  : loggerName_(loggerName)
{
}

void Log::debug(const std::string& message) const
{
  log(LogManager::LEVEL_DEBUG, message);
}

void Log::info(const std::string& message) const
{
  log(LogManager::LEVEL_INFO, message);
}

void Log::warning(const std::string& message) const
{
  log(LogManager::LEVEL_WARNING, message);
}

void Log::error(const std::string& message) const
{
  log(LogManager::LEVEL_ERROR, message);
}

const Log& Log::d() const {
  assert(!streamLog_.rdbuf()->in_avail());
  this->streamLogLevel_ = LogManager::LEVEL_DEBUG;
  return *this;
}

const Log& Log::i() const {
  assert(!streamLog_.rdbuf()->in_avail());
  this->streamLogLevel_ = LogManager::LEVEL_INFO;
  return *this;
}

const Log& Log::w() const {
  assert(!streamLog_.rdbuf()->in_avail());
  this->streamLogLevel_ = LogManager::LEVEL_WARNING;
  return *this;
}

const Log& Log::e() const {
  assert(!streamLog_.rdbuf()->in_avail());
  this->streamLogLevel_ = LogManager::LEVEL_ERROR;
  return *this;
}


Exception Log::exception(const std::string& error,
                           FunctionPtr func) const
{
#ifndef LOGGING_DISABLED
  this->error(error);
  if (func)
    this->error(func());
#endif
  return Exception(error);
}


void Log::log(LogManager::LogLevel level,
              const std::string& message) const
{
#ifndef LOGGING_DISABLED
  ConfigManager::instance().getLogManager().log(level,
                                                loggerName_,
                                                message);
#endif
}


Log::~Log()
{
}
