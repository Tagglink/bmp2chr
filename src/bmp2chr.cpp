#include <opencv2/core/core_c.h>
#include <unicode/unistr.h>

#include <string>
#include <vector>

#include "charset.hpp"
#include "converter.hpp"
#include "file.hpp"

using bmp2chr::Charset;
using bmp2chr::ConvertToUTF8;
using tagg::file::WriteFile;

int main(int argc, char** argv) {
	if (argc < 4) {
		std::cerr <<
			"bmp2chr usage: " << std::endl 
			<< "bmp2chr mycharset.char dumpfile.txt imgfile1"
			<< std::endl;
		return 1;
	}

	cv::Mat source_image;
	const Charset charset(argv[1]);

	if (cv::haveImageReader(argv[3])) {
		source_image = cv::imread(argv[3], cv::IMREAD_GRAYSCALE);
	}

	if (source_image.empty()) {
		std::cerr << "BMP2CHR: could not read image " << argv[3] << " correctly."
			<< std::endl;
		return 1;
	}

	std::string out_chars = ConvertToUTF8(charset, source_image, 160);
	WriteFile(argv[2], out_chars);

	return 0;
}
