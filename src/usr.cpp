#include "usr.h"
#include <vector>
using std::tuple;
using std::make_tuple;
using std::get;
using std::tie;
using std::cout;
using std::vector;
std::vector<Image>  triple_img(const Image &im){

//typedef Matrix<std::tuple<uint, uint, uint>> Image;	


		vector<Image> result;
		
		result.reserve(3);
		
		//Image res(im.TellHeight(), im.TellWidth());
		
		unsigned int rownum = im.n_rows /3;
		unsigned int yy = 0;
		
for (uint k = 0; k < 3; k++){
    Image channel = Image(rownum, im.n_cols);
	cout <<"ga "<< rownum << "  cols " << im.n_cols <<" rows " << im.n_rows;
    for (uint i = k * (rownum); i < rownum * (k+1) ; ++i) {
        cout << " Str  " << i  << "\n";
        for (uint j = 0; j < im.n_cols; ++j) {     	
        	if( (j < (unsigned int )im.n_cols))
            { //(i < (unsigned int )rownum) &&
        	channel((i % rownum),j) = im(i,j);
            yy++;
        }
        }
    }
cout << "3456";
    result.push_back(channel);
    cout << result.size();
}
    cout <<" Итак: " << yy << "   ..  ";
    return result;
	}

Image consolidation(std::vector<Image> src_im)
{
    unsigned int rownum  = src_im.at(0).n_rows;
    unsigned int high = src_im.at(0).n_cols;
    Image result = Image(rownum,high);
    for (uint i = 0; i < rownum  ; ++i) {
        //cout << " Str  " << i  << "\n";
        for (uint j = 0; j < high; ++j) {      
            result(i,j) = std::make_tuple(get<0>(src_im.at(0)(i,j)), get<1>( src_im.at(1)(i,j)), get<2>(src_im.at(2)(i,j)));
        }
        }


    
    

//    cout <<" Итак: " << yy << "   ..  ";
    return result;


}


/*
std::vector<Image>  load_image3 triple_img(const Image &im){

//typedef Matrix<std::tuple<uint, uint, uint>> Image;	

   BMP in;

    if (!in.ReadFromFile(path))
        throw string("Error reading file ") + string(path);
		vector<Image> result;
		result.reserve(3);
		
		//Image res(im.TellHeight(), im.TellWidth());
		
		int rownum = im.n_rows /3;
		Image channel(rownum, im.n_cols);
for (uint k = 0; k < 2; k++){
    for (uint i = k * (rownum); i < rownum * (k+1) ; ++i) {
        for (uint j = 0; j < im.n_cols; ++j) {
            RGBApixel *p = im(j, i);
            channel(i, j) = make_tuple(p->Red, p->Green, p->Blue);
        }
    }
    result.push_back(channel);
}
    return result;
	}



Image (const char *path)
{
    BMP in;

    if (!in.ReadFromFile(path))
        throw string("Error reading file ") + string(path);

    Image res(in.TellHeight(), in.TellWidth());

    for (uint i = 0; i < res.n_rows; ++i) {
        for (uint j = 0; j < res.n_cols; ++j) {
            RGBApixel *p = in(j, i);
            res(i, j) = make_tuple(p->Red, p->Green, p->Blue);
        }
    }

    return res;
}
*/