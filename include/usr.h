#pragma once

#include "matrix.h"
#include "EasyBMP.h"

#include <tuple>


#include <vector>
#include "io.h"
std::vector<Image>  triple_img(const Image &im);


Image consolidation(std::vector<Image> src_im);