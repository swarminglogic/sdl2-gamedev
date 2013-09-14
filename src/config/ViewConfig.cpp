#include <config/ViewConfig.h>


ViewConfig::ViewConfig()
  : screenSize_(800, 600),
    windowTitle_(""),
    isFullScreen_(false)
{
}

ViewConfig::ViewConfig(Size screenSize,
                       const std::string& windowTitle,
                       bool isFullScreen)
  : screenSize_(screenSize),
    windowTitle_(windowTitle),
    isFullScreen_(isFullScreen)
{
}

ViewConfig::~ViewConfig()
{
}

const Size& ViewConfig::getScreenSize() const
{
  return screenSize_;
}

void ViewConfig::setScreenSize(Size screenSize)
{
  screenSize_ = screenSize;
}

int ViewConfig::getScreenWidth() const
{
  return screenSize_.width();
}

int ViewConfig::getScreenHeight() const
{
  return screenSize_.width();
}


const std::string& ViewConfig::getWindowTitle() const
{
  return windowTitle_;
}

void ViewConfig::setWindowTitle(const std::string& windowTitle)
{
  windowTitle_ = windowTitle;
}

bool ViewConfig::isFullScreen() const
{
  return isFullScreen_;
}

void ViewConfig::setIsFullScreen(bool isFullScreenSet)
{
  isFullScreen_ = isFullScreenSet;
}

