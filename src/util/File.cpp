#include <util/File.h>

#include <cassert>
#include <functional>

#include <util/Clock.h>
#include <util/FileUtil.h>


File::File(const std::string& filename,
           bool keepLocalCopy)
  : filename_(filename),
    isLocalCopyEnabled_(keepLocalCopy),
    contentReadHash_(0)
{
}


File::~File()
{
}


bool File::exists() const
{
  return FileUtil::exists(filename_);
}


std::string File::read()
{
  timeLastRead_ = Clock::now();
  const std::string content = FileUtil::read(filename_);
  assert(timeLastRead_ >= getLastModifiedTime());

  std::size_t contentHash = std::hash<std::string>()(content);
  const bool isHashModified = contentHash != contentReadHash_;

  if (isHashModified && isLocalCopyEnabled_)
    localCopy_ = content;
  contentReadHash_ = contentHash;
  return content;
}


std::string File::read() const
{
  timeLastRead_ = Clock::now();
  return FileUtil::read(filename_);
}


const std::string& File::readCopy()
{
  assert(isLocalCopyEnabled_ && "Bad programmer, bad!");
  contentReadHash_ = localCopyHash_;
  return localCopy_;
}

bool File::isModified()
{
  timeLastRead_ = Clock::now();
  const std::string content = FileUtil::read(filename_);
  assert(timeLastRead_ >= getLastModifiedTime());

  std::size_t contentHash = std::hash<std::string>()(content);
  const bool isHashModified = contentHash != contentReadHash_;

  if (isHashModified && isLocalCopyEnabled_) {
    const bool actualStoredContentDiffers =
      contentHash != localCopyHash_;
    if (actualStoredContentDiffers)
      localCopy_ = content;
  }

  // Don't update local hash, even if you mantain a copy of the file!
  localCopyHash_ = contentHash;
  return isHashModified;
}


bool File::isUpdated() const
{
  return timeLastRead_ < getLastModifiedTime();
}


std::time_t File::getLastModifiedTime() const
{
  return FileUtil::getLastModifiedTime(filename_);
}


std::time_t File::getLastReadTime() const
{
  return timeLastRead_;
}


bool File::isLocalCopyEnabled() const
{
  return isLocalCopyEnabled_;
}


const std::string& File::File::getFilename() const
{
  return filename_;
}
