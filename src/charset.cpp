#include "charset.hpp"

#include <string>
#include <iostream>

#include <opencv2/core/core_c.h>
#include <unicode/unistr.h>
#include <unicode/schriter.h>
#include <unicode/regex.h>
#include "file.hpp"

using tagg::file::ReadFileUTF16;

namespace bmp2chr {

void Charset::ParseLine(const icu::UnicodeString& line, int num_line) {
	std::string utf8_line;
	line.toUTF8String(utf8_line);
	// first line should be the filename of the charset image
	if (num_line == 1) {
		if (cv::haveImageReader(utf8_line)) {
			image_ = cv::imread(utf8_line, cv::IMREAD_GRAYSCALE);
		} else {
			std::cerr << "could not read charset image: "
				<< utf8_line << std::endl;
		}
	// second line should be the width and height in code points
	} else if (num_line == 2) {
		width_in_chars_ = std::stoi(utf8_line.substr(
					0, utf8_line.find(',')));
		height_in_chars_ = std::stoi(utf8_line.substr(
					utf8_line.find(',') + 1));
	// third line and onwards are encodings for the mapping
	} else {
		unicode_mapping_ += line;
	}
}

Charset::Charset(const std::string& path_charfile) {
	icu::UnicodeString content_charfile;
	content_charfile = ReadFileUTF16(path_charfile);

	UErrorCode status = U_ZERO_ERROR;
	// match newline characters (crossplatform)
	// see: http://userguide.icu-project.org/strings/regexp
	icu::RegexMatcher matcher("\\R", 0, status);

	int num_lines;
	icu::UnicodeString lines[MAX_LINES];
	num_lines = matcher.split(content_charfile, lines, MAX_LINES, status);

	for (int i = 0; i < num_lines; i++) {
		ParseLine(lines[i], i + 1);
	}
}

} // namespace bmp2chr
