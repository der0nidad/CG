#pragma once

#include "matrix.h"
#include "EasyBMP.h"

#include <tuple>

typedef Matrix<std::tuple<uint, uint, uint>> Image;

Image load_image(const char*);
void save_image(const Image&, const char*);
void save_image2(const Image &im,  char *path, int ijk);

void itoa(int n, char s[]);
 void reverse(char s[]);
