#pragma once

#include "matrix.h"
#include "EasyBMP.h"

#include <tuple>


#include <vector>
#include "io.h"

int test1(uint par);

uint test2(uint par);

std::vector<Image>  triple_img(const Image &im);

Image consolidation(std::vector<Image> src_im);

std::vector<int> searching_the_best_shift(const Image &base,  const Image &test);

int calc_MSE_metric(const Image &base,  const Image &test, int n, int m);

int calc_Cross_Corr_metric(const Image &base,  const Image &test, int n, int m);

Image consolidation_with_shift_using_mse(const Image &base,  const Image &test, std::vector<int> maxresult, int chnl_number_to_add, int metric);

long double calc_color(const Image & image, int channel);

Image change_color(const Image& image, double blue, double green, double red);

Image unsharp_image(const Image & image);

Image autocontr(const Image & image, double fraction);