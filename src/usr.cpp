#include "usr.h"
#include <vector>
using std::tuple;
using std::make_tuple;
using std::get;
using std::tie;
using std::cout;
using std::vector;
inline int abs(int a){
    return a > 0 ? a : -a;
}

std::vector<Image>  triple_img(const Image &im){

	vector<Image> result;
		result.reserve(3);	
		unsigned int rownum = im.n_rows /3;
		unsigned int yy = 0;
		
for (uint k = 0; k < 3; k++){
    Image channel = Image(rownum, im.n_cols);
    for (uint i = k * (rownum); i < rownum * (k+1) ; ++i) {
        for (uint j = 0; j < im.n_cols; ++j) {     	
        	if( (j < (unsigned int )im.n_cols))
            { //(i < (unsigned int )rownum) &&
        	channel((i % rownum),j) = im(i,j);
            yy++;
        }
        }
    }
    result.push_back(channel);
    cout << result.size();
}
    return result;
	}

Image consolidation(std::vector<Image> src_im)
{
    unsigned int rownum  = src_im.at(0).n_rows;
    unsigned int high = src_im.at(0).n_cols;//high на самом деле ширина вродеs
    Image result = Image(rownum,high);
    for (uint i = 0; i < rownum  ; ++i) {
        for (uint j = 0; j < high; ++j) {      
            result(i,j) = std::make_tuple(get<0>(src_im.at(0)(i,j)), get<1>( src_im.at(1)(i,j)), get<2>(src_im.at(2)(i,j)));
        }
        }
    return result;


}

std::vector<int> searching_the_best_shift(const Image &base,  const Image &test)
{
        vector<int> result = {0 ,0};
        for (int i = -15; i < 16; ++i)
        {
            for (int j = -15; j < 16; ++j)
            {
          
            }
        }

        return result;
}






//нужно взять мимнум
int calc_MSE_metric(const Image &base,  const Image &test) //base - нееподвижна, test - свдигаем
{
    int width = base.n_cols;
    int height = base.n_rows;
    int mse = 0;
    int sum = 0;
    Image result = Image(width, height);
    auto preSquared = make_tuple(0,0,0);
    for(int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j)
        {
            //берем по пикселю из каждого изображения, считаем вектор разности, скалярно возводим его в квадрат. Прибавляем к int summa. ПОздравляем, вы великолепны
            preSquared = std::make_tuple( abs(get<0>(base(i,j)) - get<0>(test(i,j))),  abs(get<1>( base(i,j)) - get<1>(test(i,j))),  abs(get<2>(base(i,j)) - get<2>(test(i,j))));
        //что хзздесь происходит????
            sum = get<0>(preSquared) * get<0>(preSquared) + get<1>(preSquared) * get<1>(preSquared) + get<2>(preSquared) * get<2>(preSquared) ;
            mse += sum;
        }
    }
    mse /= width;
    mse /= height;
    return mse;
}

//нужно взять максимум
int calc_Cross_Corr_metric(const Image &base,  const Image &test) //base - нееподвижна, test - свдигаем
{
    int width = base.n_cols;
    int height = base.n_rows;
    int mse = 0;
    int sum = 0;
    Image result = Image(width, height);
    for(int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j)
        {
            sum = get<0>(base(i,j)) * get<0>(test(i,j)) + get<1>(base(i,j)) * get<1>(test(i,j)) + get<2>(base(i,j)) * get<2>(test(i,j)) ;
            mse += sum;
        }
    }
    return mse;
}
