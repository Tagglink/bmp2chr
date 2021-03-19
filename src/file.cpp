#include "file.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <locale>
#include <iterator>

#include <unicode/unistr.h>
#include <unicode/ustdio.h>

namespace tagg::file {

std::string ReadFile(const std::string& filename)
{
  std::string ret = "";
  std::ifstream file(filename.c_str());

  file.seekg(0, std::ios::end);
  ret.reserve(file.tellg());
  file.seekg(0, std::ios::beg);

  ret.assign((std::istreambuf_iterator<char>(file)),
    std::istreambuf_iterator<char>());

  file.close();

  return ret;
}

void WriteFile(const std::string& filename, const std::string& contents)
{
  std::ofstream file(filename.c_str());

  file << contents;

  file.close();
}

icu::UnicodeString ReadFileUTF16(const std::string& filename)
{
  UFILE* f = u_fopen(filename.c_str(), "r", NULL, NULL);
  UChar block[256];
  icu::UnicodeString ret;

  while (!u_feof(f)) {
    int read = u_file_read(block, 256, f);
    ret.append(block, read);
  }
  
  u_fclose(f);

  return ret;
}

} // namespace tagg::file
