#include <util/LogManager.h>

#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <io/BashColor.h>
#include <util/Clock.h>
#include <util/FileUtil.h>


LogManager::LogManager()
  : fileLogLevel_(LEVEL_NONE),
    streamLogLevel_(LEVEL_NONE),
    streamColorMode_(COLORMODE_NONE),
    logfilePath_("")
{
}


LogManager::LogManager(LogLevel fileLogLevel,
                       LogLevel streamLogLevel)
  : fileLogLevel_(fileLogLevel),
    streamLogLevel_(streamLogLevel),
    streamColorMode_(COLORMODE_NONE),
    logfilePath_("")
{
}


LogManager::~LogManager()
{
}


void LogManager::log(LogLevel level,
                     const std::string& loggerName,
                     const std::string& message) const
{
#ifndef LOGGING_DISABLED
  if (level < streamLogLevel_ &&
      level < fileLogLevel_)
    return;

  Clock clock;
  std::stringstream ss;

  // TODO swarminglogic, 2013-09-13: Add short build checksum (through scons)

  ss << "  "
     << logLevelAsString(level) << clock.getTimeStamp() << "  "
     << std::left << std::setw(18)
     << loggerName  << "  " << message << "\n";
  const std::string formatted(ss.str());

  if (level >= fileLogLevel_)
    log2File(formatted);

  if (level >= streamLogLevel_) {
    if (streamColorMode_ == COLORMODE_BASH ) {
      const std::string bashFormatted =
        BashColor::setColor(formatted, BashColor::getLogLevelColor(level));
      log2Stream(bashFormatted);
    }
    else
      log2Stream(formatted);
  }

#endif
}


void LogManager::log2Stream(const std::string& formatted) const
{
#ifndef LOG2STREAM_DISABLED
  std::cout << formatted;
#endif
}


void LogManager::log2File(const std::string& formatted) const
{
#ifndef LOG2FILE_DISABLED
  if (logfilePath_.empty())
    return;
  FileUtil::append(logfilePath_, formatted);
#endif
}


LogManager::LogLevel LogManager::getFileLogLevel() const
{
  return fileLogLevel_;
}


void LogManager::setFileLogLevel(LogManager::LogLevel fileLogLevel)
{
  fileLogLevel_ = fileLogLevel;
}


LogManager::LogLevel LogManager::getStreamLogLevel() const
{
  return streamLogLevel_;
}


void LogManager::setStreamLogLevel(LogLevel streamLogLevel)
{
  streamLogLevel_ = streamLogLevel;
}


const std::string& LogManager::getLogfilePath() const
{
  return logfilePath_;
}


void LogManager::setLogfilePath(std::string logfilePath)
{
  FileUtil::write(logfilePath, "");
  logfilePath_ = logfilePath;
}


LogManager::ColorMode LogManager::getStreamColorMode() const
{
  return streamColorMode_;
}


void LogManager::setStreamColorMode(LogManager::ColorMode streamColorMode)
{
  streamColorMode_ = streamColorMode;
}


std::string LogManager::logLevelAsString(LogLevel level)
{
  switch (level) {
  case LEVEL_DEBUG:
    return std::string("DEBUG   ");
    break;
  case LEVEL_INFO:
    return std::string("INFO    ");
    break;
  case LEVEL_WARNING:
    return std::string("WARNING ");
    break;
  case LEVEL_ERROR:
    return std::string("ERROR   ");
    break;
  case LEVEL_NONE:
    assert(false && "Should never be queried");
    return "";
    break;
  default:
    return "";
  }
}
