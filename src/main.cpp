#include <iostream>

#include <core/ConfigManager.h>
#include <util/Log.h>


int main(int argc, char *argv[])
{
  ConfigManager& manager = ConfigManager::instance();

  Log log("main");
  log.debug("Hello world");
  log.warning("You've been warned!");

  return 0;
}
