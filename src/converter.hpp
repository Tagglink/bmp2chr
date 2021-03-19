#ifndef BMP2CHR_CONVERTER_H
#define BMP2CHR_CONVERTER_H

#include <string>

#include <opencv2/opencv.hpp>
#include "charset.hpp"

namespace bmp2chr {

// Get a UTF-8 encoded string graphically alike to the image in 'src',
// using the characters provided in 'charset'
extern std::string ConvertToUTF8(const Charset& charset,
  const cv::Mat& src, int max_char_width);

} // namespace bmp2chr

#endif // !BMP2CHR_CONVERTER_H
