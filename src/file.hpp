#ifndef TAGG_FILE_H
#define TAGG_FILE_H

#include <string>
#include <vector>

#include <unicode/unistr.h>

namespace tagg {
namespace file {

// Returns a string reading the given path
extern std::string ReadFile(const std::string& filename);
extern void WriteFile(const std::string& filename,
  const std::string& contents);

extern std::string ReadFileUTF8(const std::string& filename);
extern void WriteFileUTF8(const std::string& filename,
  const std::string& contents);

extern icu::UnicodeString ReadFileUTF16(const std::string& filename);
extern void WriteFileUTF16(const std::string& filename,
  const icu::UnicodeString& contents);

extern std::string ReadFileBinary(const std::string& filename);
extern void WriteFileBinary(const std::string& filename, 
  const std::string& in);

} // namespace file
} // namespace tagg

#endif // !TAGG_FILE_H
