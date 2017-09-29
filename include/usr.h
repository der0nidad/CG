#pragma once

#include "matrix.h"
#include "EasyBMP.h"

#include <tuple>


#include <vector>
#include "io.h"
std::vector<Image>  triple_img(const Image &im);

Image consolidation(std::vector<Image> src_im);

std::vector<int> searching_the_best_shift(const Image &base,  const Image &test);

int calc_MSE_metric(const Image &base,  const Image &test, int n, int m);

int calc_Cross_Corr_metric(const Image &base,  const Image &test, int n, int m);

Image consolidation_with_shift_using_mse(const Image &base,  const Image &test, std::vector<int> maxresult);