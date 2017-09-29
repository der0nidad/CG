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
inline int check_border(int a, int  b, int border){
    return ((a+b )>  border )? (border - 2) : (a + b);
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
    uint rownum  = src_im.at(0).n_rows;
    uint high = src_im.at(0).n_cols;//high на самом деле ширина вродеs
    Image result = Image(rownum,high);
    for (uint i = 0; i < rownum  ; ++i) {
        for (uint j = 0; j < high; ++j) {      
            result(i,j) = std::make_tuple(get<0>(src_im.at(0)(i,j)), get<1>( src_im.at(1)(i,j)), get<2>(src_im.at(2)(i,j)));
        }
        }
    return result;


}
/*
std::vector<int> try_one_shift(const Image &base,  const Image &test, int n, int m)
{
    unsigned int rownum  = base.n_rows;
    unsigned int high = base.n_cols;//high на самом деле ширина вродеs
    Image result = Image(rownum,high);
    for (uint i = 0; i < rownum  ; ++i) {
        for (uint j = 0; j < high; ++j) {      
            // здесь будем считать 2 метрики для конкретного сдвига изображение на n по щирине и на m по высоте(+)

        }
        }
    return ;


}
*/
std::vector<int> searching_the_best_shift(const Image &base,  const Image &test)
{
        vector<int> maxresult(6);
        maxresult.at(0) = 2000000000;
        // maxresult содержит две тройки - для каждой метрики. В 1 (и 4) элементах значения метрик, а в 2 и 3 (соотв. 5 и 6 ) - значения сдвигов
        //maxresult.reserve(6);
        int mse_temp = 0, cross_corr_temp = 0;
        int width_mse = 0, height_mse = 0, width_cross_corr = 0, height_cross_corr = 0;

        for (int i = -15; i < 16; ++i)
        {
            for (int j = -15; j < 16; ++j)
            {
        
                mse_temp = calc_MSE_metric(base, test, i ,j);
                
                if(mse_temp < maxresult.at(0)){
                    maxresult.at(0) = mse_temp;
                    width_mse = i;
                    height_mse = j;
                }
                cross_corr_temp = calc_Cross_Corr_metric(base, test, i ,j);
                if( cross_corr_temp> maxresult.at(3)){
                    maxresult.at(3) = cross_corr_temp;
                    width_cross_corr = i;
                    height_cross_corr = j;
                }
                
            }
        }
        maxresult.at(1) = width_mse;
        maxresult.at(2) = height_mse;
        maxresult.at(4) = width_cross_corr;
        maxresult.at(5) = height_cross_corr;
        cout << " Зе бест шит \n";
        return maxresult;
}

Image consolidation_with_shift_using_mse(const Image &base,  const Image &test, std::vector<int> maxresult){

    uint width = base.n_cols;
    uint height = base.n_rows;
    cout << "Обоссаный деед\n";
    Image result = Image(width,height);
    for (uint i = maxresult.at(1); i < height - maxresult.at(1)  ; ++i) {
        for (uint j = maxresult.at(1); j < width - maxresult.at(2); ++j) {      
            result(i,j) = std::make_tuple( get<0>(base(i,j)) + get<0>(test(check_border(i,maxresult.at(1), width),check_border(j, maxresult.at(2), height))),  
                get<1>( base(i,j)) + get<1>(test(check_border(i,maxresult.at(1), width),check_border(j, maxresult.at(2), height))),  get<2>(base(i,j)) + get<2>(test(check_border(i,maxresult.at(1), width),check_border(j, maxresult.at(2), height))));
        }
        }
    return result;



}




//нужно взять мимнум
int calc_MSE_metric(const Image &base,  const Image &test, int n, int m) //base - нееподвижна, test - свдигаем
{
    int width = base.n_cols;
    int height = base.n_rows;
    int mse = 0;
    int sum = 0;
    Image result = Image(width, height);
    auto preSquared = make_tuple(0,0,0);
    for(int i = abs(n); i < height - abs(n); ++i){ 
        for (int j = abs(m); j < width - abs(m); ++j)
        {
            //берем по пикселю из каждого изображения, считаем вектор разности, скалярно возводим его в квадрат. Прибавляем к int summa. ПОздравляем, вы великолепны
            preSquared = std::make_tuple( abs(get<0>(base(i,j)) - get<0>(test(i+n,j+m))),  abs(get<1>( base(i,j)) - get<1>(test(i+n,j+m))),  abs(get<2>(base(i,j)) - get<2>(test(i+n,j+m))));
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
int calc_Cross_Corr_metric(const Image &base,  const Image &test, int n, int m) 
//base - нееподвижна, test - свдигаем
{
    int width = base.n_cols;
    int height = base.n_rows;
    int mse = 0;
    int sum = 0;
    Image result = Image(width, height);
    for(int i = abs(n); i < height - abs(n); ++i){ 
        for (int j = abs(m); j < width - abs(m); ++j)
        {
            sum = get<0>(base(i,j)) * get<0>(test(i+n,j+m)) + get<1>(base(i,j)) * get<1>(test(i+n,j+m)) + get<2>(base(i,j)) * get<2>(test(i+n,j+m)) ;
            mse += sum;
        }
    }
    return mse;
}
