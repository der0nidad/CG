#include "usr.h"
#include <vector>
using std::tuple;
using std::array;
using std::make_tuple;
using std::get;
using std::tie;
using std::cout;
using std::vector;
inline int abs(int a){
    return a > 0 ? a : -a;
}

inline int  calc_new_brightness(int y, int ymin, int ymax){
    return static_cast<int>(((y - ymin) * 255 ) / (ymax - ymin));
}
inline double calc_brightness(uint b, uint g, uint r){
    double a  = r * 0.2125 + g * 0.7154 + b * 0.0721;
    // cout << " " << a << " ";
    return a;
}


inline uint sqrr(uint x){
    return x * x;
}
inline int sqrr2(uint x){
    return x * x;
}
inline int check_border(int a, int  b, int border){
    return ((a+b )>=  border )? (border - 1) : (a + b);
}

std::vector<Image>  triple_img(const Image &im){

	vector<Image> result;
		result.reserve(3);	
		unsigned int rownum = im.n_rows /3;
		// unsigned int yy = 0;
		
for (uint k = 0; k < 3; k++){
    Image channel = Image(rownum, im.n_cols);
    for (uint i = k * (rownum); i < rownum * (k+1) ; ++i) {
        for (uint j = 0; j < im.n_cols; ++j) {     	
        	if( (j < static_cast<unsigned int> (im.n_cols)))
            { //(i < (unsigned int )rownum) &&
        	channel((i % rownum),j) = im(i,j);
            // yy++;
        }
        }
    }
    result.push_back(channel);
    // cout << result.size();
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
        
        vector<int> maxresult(7);
        maxresult.at(0) = 2000000000;
        maxresult.at(3) = -200000000;
        // cout << "\n vector size " << maxresult.size();
        // for(uint i = 0; i < maxresult.size(); i++){
        // cout << " \n new vector " << maxresult.at(i) << " at " << i ; }
        // maxresult содержит две тройки - для каждой метрики. В 1 (и 4) элементах значения метрик, а в 2 и 3 (соотв. 5 и 6 ) - значения сдвигов
        //maxresult.reserve(6);
        int mse_temp = 0, cross_corr_temp = 0;
        int width_mse = 0, height_mse = 0, width_cross_corr = 0, height_cross_corr = 0;
        // int yyy = 0;

        for (int i = -15; i < 16; ++i)
        {
            for (int j = -15; j < 16; ++j)
            {
                 // cout << "  перед ";// << yyy;
                mse_temp = calc_MSE_metric(base, test, i ,j);
                 // yyy++;
                  // cout << " " << mse_temp;  
                
                if(mse_temp < maxresult.at(0)){
                    
                    maxresult.at(0) = mse_temp;
                    width_mse = i;
                    height_mse = j;
                }
                cross_corr_temp = calc_Cross_Corr_metric(base, test, i ,j);
                if( cross_corr_temp > maxresult.at(3)){
                    maxresult.at(3) = cross_corr_temp;
                    width_cross_corr = i;
                    height_cross_corr = j;
                }
                
            }
            // cout << "\n new line:";
        }

        maxresult.at(1) = width_mse;
        maxresult.at(2) = height_mse;
        maxresult.at(4) = width_cross_corr;
        maxresult.at(5) = height_cross_corr;
        // cout << " Зе бест шит \n";
        return maxresult;
}
int test1(uint par){
    return sqrr2(par);
}
uint test2(uint par){
    return sqrr2(par);
}
Image consolidation_with_shift_using_mse(const Image &base,  const Image &test, std::vector<int> maxresult, int chnl_number_to_add, int metric){//chnl_number_to_add  - какой канал добавлять

    uint width = base.n_cols;
    uint height = base.n_rows;
    vector<int> myvector(4);
    // cout << "Обоссаный деед\n";
    Image result = Image(height,width);
    if(metric == 1){//using mse
        // Эти два коммента ОЧЕНЬ ВАЖНЫ!!
        //Разберись с путаницей в myvector'ах - у тебя работает все, кроме этого
            myvector.at(2) = maxresult.at(1);
            myvector.at(1) = maxresult.at(2);
            cout << "Срыв покровов " << myvector.at(1) << "Дружко продолжает отжигать: " << myvector.at(2);
            // myvector.at(1) = 1;
            // myvector.at(2) = -4;
    }else if(metric == 2) {//using cross corr
            myvector.at(2) = maxresult.at(4);
            myvector.at(1) = maxresult.at(5);
        }else{ cout << "не та метрика";}
    for (uint i =abs( myvector.at(2)) + 1; i < height -abs( myvector.at(2)) -1  ; ++i) {
        for (uint j = abs( myvector.at(1)) +1 ; j < width - abs(myvector.at(1)) -1; ++j) {      
        if(chnl_number_to_add == 1){
        result(i,j) = std::make_tuple(get<0>(base(i, j)), get<1>(test(i + myvector.at(2), j + myvector.at(1))),get<2>(base(i, j) ));
            }
       if(chnl_number_to_add == 2){
        result(i,j) = std::make_tuple(get<0>(base(i, j)),get<1>(base(i, j) ), get<2 >(test(i + myvector.at(2), j + myvector.at(1))));
    }
        // а что если просто писать в соотв каналы?? и ничего не складывать???
       /*     result(i,j) = std::make_tuple( get<0>(base(i,j)) + get<0>(test(check_border(i,maxresult.at(1), height),check_border(j, maxresult.at(2), width))),  
                get<1>( base(i,j)) + get<1>(test(check_border(i,maxresult.at(1), height),check_border(j, maxresult.at(2), width))),  get<2>(base(i,j)) + get<2>(test(check_border(i,maxresult.at(1), height),check_border(j, maxresult.at(2), width))));
        */}
        }/*
        for (int i = 50; i < 100; ++i)
        {
            for (int j = 50; j < 100; ++j)
            {
                result(i,j) = std::make_tuple(255,255,255);
            }
        }*/
    return result;



}




//нужно взять мимнум
int calc_MSE_metric(const Image &base,  const Image &test, int n, int m) //base - нееподвижна, test - свдигаем
{
    int width = base.n_cols - 32;
    int height = base.n_rows - 32;
    uint mse = 0;
    
    Image result = Image(height, width);
    auto preSquared = make_tuple(0,0,0);
    for(int i = 16; i < height - 16; ++i){ 
        for (int j = 16; j < width - 16; ++j)
        {
            // cout << " mse ";
            //берем по пикселю из каждого изображения, считаем вектор разности, скалярно возводим его в квадрат. Прибавляем к int summa. ПОздравляем, вы великолепны
            // if((j != 4294967295) & (i != 391))
            preSquared = std::make_tuple( get<0>(base(i,j)) - get<0>(test(i + n,j + m)),  
                get<1>( base(i,j)) - get<1>(test(i + n, j + m)),  
                get<2>(base(i,j)) - get<2>(test(i + n , j + m)));
        //что хзздесь происходит????
            mse += get<0>(preSquared) * get<0>(preSquared) + get<1>(preSquared) * get<1>(preSquared) + get<2>(preSquared) * get<2>(preSquared) ;
            


/* чисто для сеёва
     preSquared = std::make_tuple( abs(get<0>(base(i,j)) - get<0>(test(check_border(i,n,height),check_border(j, m , width) ))),  
                abs(get<1>( base(i,j)) - get<1>(test(check_border(i, n, height) ,check_border(j, m, width) ))),  
                abs(get<2>(base(i,j)) - get<2>(test(check_border(i, n, height) ,check_border(j, m, width) ))));
        //что хзздесь происходит????
            sum = get<0>(preSquared) * get<0>(preSquared) + get<1>(preSquared) * get<1>(preSquared) + get<2>(preSquared) * get<2>(preSquared) ;
            mse += sum;
*/
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
    uint width = base.n_cols - 32;
    uint height = base.n_rows - 32;
    uint mse = 0;
    uint sum = 0;
    Image result = Image(height, width);
    for(uint i = 16; i < height - 16; ++i){ 
            for (uint j = 16; j < width - 16; ++j)
            {
                sum = get<0>(base(i,j)) * get<0>(test(i+n,j+m)) + get<1>(base(i,j)) * get<1>(test(i+n,j+m)) + get<2>(base(i,j)) * get<2>(test(i+n,j+m)) ;
                mse += sum;
            }
        }
        mse /= width;
    mse /= height;
    return mse;
}


// здесь начинаются функции небазовой части
// считаем опрделенный цвет на всем изображении
long double calc_color(const Image & image, const int channel)
{
         long double color_sum = 0;
        // cout << "интересно" << square << " rows   " << image.n_rows << " cols  " <<image.n_cols;
        if (channel == 0)
        {
            for (uint i = 0; i < image.n_rows ; ++i)
        {
            for (uint j = 0; j < image.n_cols ; ++j)
            {
                // cout << ";  "<<i  << "-" << j; 
                color_sum += get<0>(image(i,j));
            }
        }
            
        }
        else if (channel == 1)
        {
            for (uint i = 0; i < image.n_rows  ; ++i)
        {
            for (uint j = 0; j < image.n_cols ; ++j)
            {
                color_sum += get<1>(image(i,j));
            }
        }
        }else if (channel == 2){
            for (uint i = 0; i < image.n_rows ; ++i)
            {
            for (uint j = 0; j < image.n_cols ; ++j)
            {
                color_sum += get<2>(image(i,j));
            }
        }
        }else return -1;
        return color_sum ;
}
Image change_color(const Image& image, double blue, double green, double red)
{
    Image result(image.n_rows, image.n_cols);
    unsigned int ch1  = 0, ch2 = 0, ch3 = 0;

    for (uint i = 0; i < image.n_rows ; ++i)
        {
        for (uint j = 0; j < image.n_cols ; ++j)
            {
                
                std::tie(ch1,ch2,ch3) = image(i,j);
                ch1 = ch1 * blue;
                ch2 = ch2 * green;
                ch3 = ch3 * red;
                if(ch1 > 255 ){
                    ch1 = 255;
                }
                if(ch2 > 255 ){
                    ch2 = 255;
                }
                if(ch3 > 255 ){
                    ch3 = 255;
                }
                result(i,j) = make_tuple(ch1,ch2 , ch3);
            }
        }
    return result;   
}

Image unsharp_image(const Image & image){
    Image result(image.n_rows, image.n_cols);
    // Image kernel(3,3);
    int chnl = 0;
    std::vector<int> pixel(4);
    std::vector<int> kernel = {-1, -4, -1,/*вторая строка*/ -4, 26, -4,/*третья строка*/ -1, -4, -1} ;/* kernel(0,0) = make_tuple(-1, -1, -1);
    kernel(0,1) = make_tuple(-4, -4, -4);
    kernel(0,2) = make_tuple(-1, -1, -1);

    kernel(1,0) = make_tuple(-4, -4, -4);
    kernel(1,1) = make_tuple(26, 26, 26);
    kernel(1,2) = make_tuple(-4, -4, -4);

    kernel(2,0) = make_tuple(-1, -1, -1 );
    kernel(2,1) = make_tuple(-4, -4, -4);
    kernel(2,2) = make_tuple(-1, -1, -1);
     uint ch1 = 0, ch2 = 0, ch3 = 0;*/

    for (uint i = 1; i < image.n_rows - 1 ; ++i)
    {
        for (uint j= 1; j < image.n_cols - 1; ++j)
        {
            // std::tie(pixel.at(0), pixel.at(1), pixel.at(2)) = image(i,j);
            
            
                
            
            for (uint k = 0; k < 3; ++k)
            {
                for (uint l = 0; l < 3; ++l)
                {
                    pixel.at(chnl) += get<0> (image(i -1 + k,j - 1 + l)) * kernel.at(k * 3 + l);
                    pixel.at(chnl + 1) += get<1> (image(i -1 + k,j - 1 + l)) * kernel.at(k * 3 + l);
                    pixel.at(chnl + 2) += get<2> (image(i -1 + k,j - 1 + l)) * kernel.at(k * 3 + l);
                }
            }
            pixel.at(chnl) /= 6;
            pixel.at(chnl + 2) /= 6;
            pixel.at(chnl + 1) /= 6;
            
            if(pixel.at(chnl) > 255) {pixel.at(chnl) = 255;}
            if(pixel.at(chnl + 1) > 255) {pixel.at(chnl + 1) = 255;}
            if(pixel.at(chnl + 2) > 255) {pixel.at(chnl + 2) = 255;}

            if(pixel.at(chnl) < 0) {pixel.at(chnl) = 0;}
            if(pixel.at(chnl + 1) < 0 ) {pixel.at(chnl + 1) = 0;}
            if(pixel.at(chnl + 2) < 0) {pixel.at(chnl + 2) = 0;}

            result(i,j) = make_tuple(pixel.at(0), pixel.at(1), pixel.at(2));
        }
    }

return result;
}


Image autocontr(const Image & image, double fraction){
    Image result(image.n_rows, image.n_cols);
    array<int , 256> hist {{0}};
    int ch1  = 0, ch2 = 0, ch3 = 0;
    uint square = image.n_rows * image.n_cols;
    uint min = static_cast<int>(square * fraction);
    uint sum = 0, sum2 = 0, index_min = 0, index_max = 255;
    uint pixels = 0;

    for (uint i = 0; i < image.n_rows ; ++i)
        {
        for (uint j = 0; j < image.n_cols ; ++j)
            {
                
                std::tie(ch1,ch2,ch3) = image(i,j);
                
                hist.at(static_cast<int>( calc_brightness(ch1, ch2 , ch3)))++;
            }
        }
        for (int i = 0; (sum < min) && (i < 256)  ; ++i)
        {
            sum += hist.at(i);
            index_min = i;
            // cout << sum << " " << hist.at(i) << " " << i << "\n";
        }
       // cout << "смена цикла !!!\n\n\n\n\n";
        for (int i = 255; (sum2 < min) && (i > 0)  ; --i)
        {

            sum2 += hist.at(i);
            index_max = i;
            // cout << sum2 << " " << hist.at(i) << " " << i << "\n";
        }
        for (uint i = 0; i < image.n_rows ; ++i)
        {
            for (uint j = 0; j < image.n_cols ; ++j)
            {
                tie(ch1,ch2,ch3) = image(i,j);
                ch1 = calc_new_brightness(ch1, index_min, index_max);
                ch2 = calc_new_brightness(ch2, index_min, index_max);
                ch3 = calc_new_brightness(ch3, index_min, index_max);
                if(ch1 > 255) {ch1 = 255;}
                if(ch2 > 255) {ch2 = 255;}
                if(ch3 > 255) {ch3 = 255;}

                if(ch1 < 0) {ch1 = 0;}
                if(ch2 < 0 ) {ch2 = 0;}
                if(ch3 < 0) {ch3 = 0;}
                result(i,j) = make_tuple(ch1,ch2,ch3);

            }
        }
       cout << "Кол=во пикселей в шистошрамме  " << pixels << "  Площадб  " << square << " индекс мин  " << index_min << "  индекс макс  " << index_max << "\n";
    return result;
}
