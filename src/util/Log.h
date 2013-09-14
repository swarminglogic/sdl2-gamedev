#ifndef UTIL_LOG_H
#define UTIL_LOG_H

#include <string>

#include <util/Exception.h>
#include <util/LogManager.h>


/**
 * Log class.
 * Used for writing log calls.
 *
 * @author SwarmingLogic (Roald Fernandez)
 */
class Log
{
public:
  virtual ~Log();
  Log(const std::string& loggerName);

  // Log functions for the various log levels
  void debug(const std::string& message) const;
  void info(const std::string& message) const;
  void warning(const std::string& message) const;
  void error(const std::string& message) const;

  using FunctionPtr = const char*(*)();

  /**
   * Logs an exception.
   * It optionally calls and logs result of a function.
   * Typically, it does not itself throw exception.
   *
   * @param error   Error message to log.
   * @param func    optional function pointer call and log.
   */
  Exception exception(const std::string& error,
                      FunctionPtr func = nullptr) const;

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
