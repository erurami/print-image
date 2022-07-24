
#pragma once

#include "imageclass.hpp"

#define METHOD_BOX_SAMPLING 1
#define METHOD_BILINEAR     2

int ShrinkImage(Image* pImageOut, Image* pImageIn, int widthTo, int heightTo, int method = METHOD_BOX_SAMPLING);


