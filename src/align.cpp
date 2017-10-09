#include "align.h"
#include <string>

using std::string;
using std::cout;
using std::endl;
using std::tuple;
using std::make_tuple;
using std::get;
using std::tie;
using std::vector;

inline int check_color(Image input){
    for (uint i = 0; i < input.n_rows; ++i)
    {
        for (uint j = 0; j < input.n_cols; ++j)
        {
            if( (std::get<0>(input(i,j)) > 255 ) || (std::get<1>(input(i,j)) > 255 ) || (std::get<2>(input(i,j)) > 255)) cout << "  Подстава!!  " << i << " столбец " << j;
        }
    }
    return 0;
}

Image align(Image srcImage, bool isPostprocessing, std::string postprocessingType, double fraction, bool isMirror, 
            bool isInterp, bool isSubpixel, double subScale)
{

    vector<Image> chnls;
    Image dst_image, temp_image;
    vector<int> maxresult;
    vector<int> maxresult2;
    
            chnls.reserve(3);
            maxresult.reserve(6);
            chnls = triple_img(srcImage);
            check_color(chnls.at(1));
      // cout << "пидор";
            maxresult = searching_the_best_shift(chnls.at(2), chnls.at(1));
            for (uint i = 0; i < maxresult.size(); ++i)
            {
             
                cout << " вектора   "<< maxresult.at(i) << "\n";
            }
            // cout << "конец секции тестов\n";
      
            cout << "первый\n";
             temp_image = consolidation_with_shift_using_mse(chnls.at(2), chnls.at(1), maxresult, 1, 1); /*searching_the_best_shift(chnls.at(0), chnls.at(1)));*/
            cout << "второй\n";
            // cout << "размеры tremp img n_rows " << temp_image.n_rows << "  n_cols  " << temp_image.n_cols << "\n";
              /*maxresult2 = searching_the_best_shift(temp_image, chnls.at(2));
                  for (uint i = 0; i < maxresult.size(); ++i)
            {
             
                cout << " вектор 2 temp  "<< maxresult.at(i) << "\n";
            }*/
            maxresult = searching_the_best_shift(temp_image, chnls.at(0));
            temp_image = consolidation_with_shift_using_mse(temp_image, chnls.at(0), maxresult, 2, 1);
            cout << "третий\n";
            for (uint i = 0; i < maxresult.size(); ++i)
            {
             
                cout << "  new  вектора   "<< maxresult.at(i) << "\n";
            }
            /*maxresult = searching_the_best_shift(chnls.at(2), temp_image);
for (uint i = 0; i < maxresult.size(); ++i)
            {
             
                cout << "  newnew перестановки вектора   "<< maxresult.at(i) << "\n";
            }*/

            // cout << "конец секции тестов\n";
            check_color(temp_image);
            // cout << "Пикселы изображения 1 канал рэд" << std::get<0>(chnls.at(2)(50,50)) << " g: " << std::get<1>(chnls.at(2)(50,50)) << "  b: " << std::get<2>(chnls.at(2)(50,50)) ;
            // cout << "Пикселы изображения 1 канал рэд" << std::get<0>(temp_image(50,50)) << " g: " << std::get<1>(temp_image(50,50)) << "  b: " << std::get<2>(temp_image(50,50)) ;
            // dst_image = consolidation(chnls);
                        // cout << "Пикселы изображения 1 канал рэд" << std::get<0>(dst_image(5,5)) << " g: " << std::get<1>(dst_image(5,5)) << "  b: " << std::get<2>(dst_image(5,5)) ;

            // cout << "dst img rows, cols " << dst_image.n_rows << "  "<< dst_image.n_cols << "temp_image rows cols "  << temp_image.n_rows << "  " << temp_image.n_cols;
            // cout << "dst imag mse " << 
            //cout << "Ух нихуя ж себе!" << calc_MSE_metric(temp_image, src_image,0,0) << " Великолепно!!";
            //cout << "Ух нихуя ж себе!" << calc_Cross_Corr_metric (temp_image, src_image,0,0) << " Великолепно!!";
            /*chnls[0]  = align(src_image, isPostprocessing, postprocessingType, fraction, isMirror, 
                isInterp, isSubpixel, subScale);
           dst_image = align(src_image, isPostprocessing, postprocessingType, fraction, isMirror, 
                isInterp, isSubpixel, subScale); */
    
    if(isPostprocessing){
        if(postprocessingType == "--unsharp"){
            temp_image = unsharp(temp_image);
        }if(postprocessingType == "--gray-world"){
            temp_image = gray_world(temp_image);
            
        }if(postprocessingType == "--autocontrast"){
            temp_image = autocontrast(temp_image,fraction);

        }
    }
    return temp_image;
}

Image sobel_x(Image src_image) {
    Matrix<double> kernel = {{-1, 0, 1},
                             {-2, 0, 2},
                             {-1, 0, 1}};
    return custom(src_image, kernel);
}

Image sobel_y(Image src_image) {
    Matrix<double> kernel = {{ 1,  2,  1},
                             { 0,  0,  0},
                             {-1, -2, -1}};
    return custom(src_image, kernel);
}

Image unsharp(Image src_image) {

    Image result = Image(src_image.n_rows, src_image.n_cols);
    result = unsharp_image(src_image);
    check_color(result);
    return result;
}

Image gray_world(Image src_image) {
    Image result = Image(src_image.n_rows, src_image.n_cols);
    long double blue = 0.0, green = 0.0, red = 0.0, avg = 0.0;
    blue =   static_cast<long double>(calc_color(src_image, 0));
    green =  static_cast<long double>( calc_color(src_image, 1));

    red = static_cast<long double>( calc_color(src_image, 2));
    cout << "\n старые  цвета r g b " <<   static_cast<int> (red) << " " <<   static_cast<int> (green) << " " <<  static_cast<int> (blue) << "\n";
    cout << "размер   "<< src_image.n_rows * src_image.n_cols * 255 << "\n ";
    cout << " тест 1 " << red;
    avg = (red + green + blue) / 3;
    cout << " тест avg " << avg;
    blue = avg / blue;
    green = avg / green;
    red = avg / red;
    cout << " тест 1 " << red;
    cout << " тест 2 " << blue;
    cout << "\n новые цвета r g b " << red << " " << green << " " << blue ;
    result = change_color(src_image, blue, green, red );
    // check_color(result);
    return result;
}

Image resize(Image src_image, double scale) {
    return src_image;
}

Image custom(Image src_image, Matrix<double> kernel) {
    // Function custom is useful for making concrete linear filtrations
    // like gaussian or sobel. So, we assume that you implement custom
    // and then implement other filtrations using this function.
    // sobel_x and sobel_y are given as an example.
    return src_image;
}

Image autocontrast(Image src_image, double fraction) {
    Image result = Image(src_image.n_rows, src_image.n_cols);
    result = autocontr(src_image, fraction);
    return result;
}

Image gaussian(Image src_image, double sigma, int radius)  {
    return src_image;
}

Image gaussian_separable(Image src_image, double sigma, int radius) {
    return src_image;
}

Image median(Image src_image, int radius) {
    return src_image;
}

Image median_linear(Image src_image, int radius) {
    return src_image;
}

Image median_const(Image src_image, int radius) {
    return src_image;
}

Image canny(Image src_image, int threshold1, int threshold2) {
    return src_image;
}
