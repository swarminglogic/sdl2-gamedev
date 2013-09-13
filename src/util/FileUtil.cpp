#include <util/FileUtil.h>

#include <fstream>

#include <util/Exception.h>


std::string FileUtil::read(const std::string& fileName)
{
  std::ifstream file(fileName, std::ios::in | std::ios::binary);
  if (file) {
    std::string text;
    file.seekg(0, std::ios::end);
    text.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&text[0], text.size());
    file.close();
    return(text);
  }

  throw Exception(std::string("Failed to open file: ") + fileName);
}


void FileUtil::write(const std::string& fileName,
                     const std::string& content)
{
  std::ofstream outfile(fileName);
  if (!outfile.good())
    throw Exception(std::string("Failed to write to file: ") + fileName);
  outfile << content;
}


void FileUtil::append(const std::string& fileName,
                      const std::string& content)
{
  std::ofstream outfile(fileName, std::ios_base::app);
  if (!outfile.good())
    throw Exception(std::string("Failed to append to file: ") + fileName);
  outfile << content;
}
