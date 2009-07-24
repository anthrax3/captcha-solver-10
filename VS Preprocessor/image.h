#include "CImg.h"
using namespace cimg_library;

class Image : public CImg<unsigned char> {
public:
	Image(char *name);
};
