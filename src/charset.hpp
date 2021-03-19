#ifndef BMP2CHR_CHARSET_H
#define BMP2CHR_CHARSET_H

#include <opencv2/opencv.hpp>
#include <unicode/unistr.h>
#include <string>

namespace bmp2chr {

class Charset final {
private:
	void ParseLine(const icu::UnicodeString& line, int num_line);
public:
	static constexpr int MAX_LINES = 256;

  int width_in_chars_;
  int height_in_chars_;
  cv::Mat image_;
	icu::UnicodeString unicode_mapping_;

	Charset(const std::string& path_charfile);
};

}

#endif // !BMP2CHR_CHARSET_H
