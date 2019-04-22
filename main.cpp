#include<iostream>
#include "hash.hpp"

int main(int argc,char* argv[]){
std::string imgpath=argv[1];
cv::Mat img=cv::imread(imgpath,CV_LOAD_IMAGE_GRAYSCALE);
Hash sh;
std::string res=sh.get_outline_hash(img,180,320);
std::cout<<res<<std::endl;
return 0;
}
