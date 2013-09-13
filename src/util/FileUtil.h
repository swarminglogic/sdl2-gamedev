#ifndef UTIL_FILEUTIL_H
#define UTIL_FILEUTIL_H

#include <string>


/**
 * Static utility class for managing files
 *
 * @author Roald Fernandez
 */
class FileUtil
{
public:
  /**
   * Reads the whole content of a file, returned as a string.
   *
   * Check if file exists before trying to read it.
   *
   * @throw Exception, if I/O error occurs.
   * @return Content of file.
   */
  static std::string read(const std::string& fileName);

  /**
   * Appends content to a file (creates one if non-existent).
   *
   * @throw Exception, if failed to open and also failed to create file.
   *
   * @param fileName
   * @param content
   */
  static void append(const std::string& fileName,
                     const std::string& content);


  /**
   * Writes content to a file (creates one if non-existent).
   *
   * @throw Exception, if failed to create file.
   * @note Warning: Overrides file if it exists!
   * @param fileName
   * @param content
   */
  static void write(const std::string& fileName,
                    const std::string& content);


private:
  FileUtil(); // Static class
  // Not implemented!
  FileUtil(const FileUtil& c);
  FileUtil& operator=(const FileUtil& c);
};

#endif
