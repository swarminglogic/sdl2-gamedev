#include <util/FileUtil.h>

#include <cstdio>
#include <fstream>

#include <boost/filesystem.hpp>

#include <util/Exception.h>


namespace fs = boost::filesystem;

std::string FileUtil::read(const std::string& filename)
{
  std::ifstream file(filename, std::ios::in | std::ios::binary);
  if (file) {
    std::string text;
    file.seekg(0, std::ios::end);
    text.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&text[0], text.size());
    file.close();
    return(text);
  }

  throw Exception(std::string("Failed to open file: ") + filename);
}


void FileUtil::write(const std::string& filename,
                     const std::string& content)
{
  std::ofstream outfile(filename);
  if (!outfile.good())
    throw Exception(std::string("Failed to write to file: ") + filename);
  outfile << content;
}


bool FileUtil::exists(const std::string& filename)
{
  return fs::exists(filename);
}


void FileUtil::append(const std::string& filename,
                      const std::string& content)
{
  std::ofstream outfile(filename, std::ios_base::app);
  if (!outfile.good())
    throw Exception(std::string("Failed to append to file: ") + filename);
  outfile << content;
}


bool FileUtil::remove(const std::string& filename)
{
  return std::remove(filename.c_str()) == 0;
}


std::time_t FileUtil::getLastModifiedTime(const std::string& filename)
{
  return fs::last_write_time(filename.c_str());
}
