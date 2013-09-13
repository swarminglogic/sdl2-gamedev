#ifndef UTIL_LOG_H
#define UTIL_LOG_H

#include <string>

#include <util/LogManager.h>


/**
 * Log class.
 * Used for writing log calls.
 *
 * @author SwarmingLogic
 */
class Log
{
public:
  virtual ~Log();
  Log(const std::string& loggerName);

  void debug(const std::string& message) const;
  void info(const std::string& message) const;
  void warning(const std::string& message) const;
  void error(const std::string& message) const;

private:
  // Can only be instantiated through non-default constructor
  Log();

  // Denotes globally set log levels.
  const std::string loggerName_;

  // NonCopyable
  Log(const Log& c);
  Log& operator=(const Log& c);
};

#endif
