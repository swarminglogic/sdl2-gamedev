#include <cstdlib>
#include <iostream>

#include <core/ConfigManager.h>
#include <core/MainManager.h>
#include <util/Exception.h>
#include <util/Log.h>


int main(int argc, char *argv[])
{
  Log log("main");
  try {
    ConfigManager::instance();
    MainManager::instance();
  }
  catch (Exception e) {
    log.error(std::string("Terminating early due to exception: ") + e.what());
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
