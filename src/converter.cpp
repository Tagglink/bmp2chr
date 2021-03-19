#include "converter.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <limits>

#include <opencv2/opencv.hpp>
#include <unicode/unistr.h>
#include "charset.hpp"
#include "file.hpp"

namespace {

std::vector<cv::Mat> BlockifyImage(const cv::Mat& src, const cv::Size& size)
{
  std::vector<cv::Mat> ret = {};

  CV_Assert(src.rows % size.height == 0);
  CV_Assert(src.cols % size.width == 0);

  for (int y = 0; y < src.rows / size.height; y++) {
    for (int x = 0; x < src.cols / size.width; x++) {
      cv::Rect r(x * size.width, y * size.height, size.width, size.height);
      cv::Mat block = src(r);
      ret.push_back(block);
    }
  }

  return ret;
}

// Mean Squared Error algorithm for grayscale images
double MSE(const cv::Mat& a, const cv::Mat& b)
{
  CV_Assert(a.rows == b.rows);
  CV_Assert(a.cols == b.cols);
  CV_Assert(a.type() == CV_8UC1);
  CV_Assert(b.type() == CV_8UC1);

  uchar p_a = 0;
  uchar p_b = 0;
  int diff = 0;
  unsigned long sum = 0;

  for (int i = 0; i < a.rows; i++) {
    for (int j = 0; j < a.cols; j++) {
      p_a = a.ptr<uchar>(i)[j];
      p_b = b.ptr<uchar>(i)[j];
      if (p_a > p_b) {
        diff = p_a - p_b;
      }
      else {
        diff = p_b - p_a;
      }

      sum += diff * diff;
    }
  }

  double mean = sum / (a.rows * a.cols);

  return mean;
}

// Resizes an image reference so that 
// its width and height are both divisible by block_size's
// width and height respectively
void FitBlockSize(cv::Mat& img, const cv::Size& block_size, int max_char_width)
{
  // aspect ratio
  double cols_per_row = (double)img.cols / (double)img.rows;

  int width = max_char_width * block_size.width;
  // if the image is smaller than max_char_width, use the original width
  if (width > img.cols) {
    width = round(
      img.cols / (double)block_size.width
    ) * block_size.width;
  }

  // adapt the height to fit the block_size as close as possible to the
  // original aspect ratio
  int height = round(
    (width / cols_per_row) / block_size.height
  ) * block_size.height;
  cv::Size new_size(width, height);

  cv::resize(img, img, new_size, 0.0, 0.0, cv::INTER_CUBIC);
}

} // namespace

namespace bmp2chr {

std::string ConvertToUTF8(const Charset& charset,
  const cv::Mat& src, int max_char_width)
{
  cv::Size block_size(
    charset.image_.cols / charset.width_in_chars_,
    charset.image_.rows / charset.height_in_chars_
  );

  cv::Mat source_clone = src.clone();
  cv::Mat charset_clone = charset.image_.clone();

  ::FitBlockSize(source_clone, block_size, max_char_width);

  //debug
  cv::imshow("gray image rescaled", source_clone);
  cv::waitKey();

  // the charset.image_ divided in a grid by block_size
  std::vector<cv::Mat> chars =
    ::BlockifyImage(charset_clone, block_size);
  // the source image divided in a grid by block_size
  std::vector<cv::Mat> source_blocks =
    ::BlockifyImage(source_clone, block_size);
	// the output text
	icu::UnicodeString out_str;

  // debug
  cv::Mat char_converted_img(source_clone.size(), CV_8UC1);
  std::vector<cv::Mat> blocks =
    ::BlockifyImage(char_converted_img, block_size);
  cv::Mat best_char;

  double best_diff = std::numeric_limits<double>::max();
  int best_char_index = 0;

  for (int i = 0; i < source_blocks.size(); i++) {
    best_diff = std::numeric_limits<double>::max();
    for (int j = 0; j < chars.size(); j++) {
      double result = ::MSE(source_blocks[i], chars[j]);
      if (result < best_diff) {
        best_diff = result;
        best_char_index = j;
        //debug
        best_char = chars[j];
      }
    }
    //debug
    best_char.copyTo(blocks[i]);

    if (i != 0 && i % (source_clone.cols / block_size.width) == 0) {
			out_str += u'\u000a';
    }
    out_str += charset.unicode_mapping_[best_char_index];
  }

  //debug
  cv::imshow("utf8 chars image", char_converted_img);
  cv::waitKey();

	std::string ret;
	return out_str.toUTF8String(ret);
}

} // namespace bmp2chr
