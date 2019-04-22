//
//  hash.hpp
//  paper2
//
//  Created by 朱瑞 on 19/4/22.
//  Copyright © 2019年 朱瑞. All rights reserved.
//

#ifndef hash_h
#define hash_h
#include<opencv2/opencv.hpp>
#include<string>
#include<iostream>
#include<vector>
//options for the filter
typedef struct{
    //vessel scales
    int sigma_start;
    int sigma_end;
    int sigma_step;
    
    //BetaOne: suppression of blob-like structures.
    //BetaTwo: background suppression. (See Frangi1998...)
    float BetaOne;
    float BetaTwo;
    
    bool BlackWhite; //enhance black structures if true, otherwise enhance white structures
} frangi2d_opts_t;

#define DEFAULT_SIGMA_START 3
#define DEFAULT_SIGMA_END 7
#define DEFAULT_SIGMA_STEP 1
#define DEFAULT_BETA_ONE 1.6
#define DEFAULT_BETA_TWO 0.08
#define DEFAULT_BLACKWHITE true


/////////////////
//Frangi filter//
/////////////////

//apply full Frangi filter to src. Vesselness is saved in J, scale is saved to scale, vessel angle is saved to directions.
void frangi2d(const cv::Mat &src, cv::Mat &J, cv::Mat &scale, cv::Mat &directions, frangi2d_opts_t opts);
void pore_2d(const cv::Mat &src, cv::Mat &J, cv::Mat &scale, cv::Mat &directions, frangi2d_opts_t opts);



////////////////////
//Helper functions//
////////////////////

//run 2D Hessian filter with parameter sigma on src, save to Dxx, Dxy and Dyy.
void frangi2d_hessian(const cv::Mat &src, cv::Mat &Dxx, cv::Mat &Dxy, cv::Mat &Dyy, float sigma);

//set opts to default options (sigma_start = 3, sigma_end = 7, sigma_step = 1, BetaOne = 1.6, BetaTwo 0.08)
void frangi2d_createopts(frangi2d_opts_t *opts);

//estimate eigenvalues from Dxx, Dxy, Dyy. Save results to lambda1, lambda2, Ix, Iy.
void frangi2_eig2image(const cv::Mat &Dxx, const cv::Mat &Dxy, const cv::Mat &Dyy, cv::Mat &lambda1, cv::Mat &lambda2, cv::Mat &Ix, cv::Mat &Iy);


class WebSegment {
private:
    //    cv::Mat imgSrc;
    //    cv::Mat imgDest;
    //    cv::Mat imgDraw;
    //    cv::Mat imgNoWords;
    cv::Mat imgToRidge(cv::Mat mat);
    cv::Mat imgDeleteWords(cv::Mat &mat);
    cv::Mat showWrinkleCommonLine(cv::Mat mat);
public:
    WebSegment();
    ~WebSegment();
    std::string getOutImg();
    cv::Mat getAnsImg(cv::Mat &img);
    
    
    
};//class

//rgb2rbx
cv::Mat findline(cv::Mat &img,int lenhigh,int minlengthwidth,int misspot);

//hash
class Hash{
private:
    std::string outlinehash_row(cv::Mat src,int num);
    std::string outlinehash_col(cv::Mat src,int num);

    std::string outline_hash(cv::Mat &img,int r,int c);
 
    
public:
    std::string get_outline_hash(cv::Mat &img,int r,int c);
    

};



#endif /* hash_h */
