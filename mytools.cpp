//
//  mytools.cpp
//  paper2
//
//  Created by 朱瑞 on 19/4/22.
//  Copyright © 2019年 朱瑞. All rights reserved.
//

#include <stdio.h>
#include "hash.hpp"
std::string imgin="/Users/zrain/Desktop/scshot/";
std::string imgout="/Users/zrain/Desktop/scshot/temp/";


WebSegment::WebSegment(){
    //    imgSrc=img.clone();
    //    imgDest=imgToRidge(imgSrc);
    //    imgDraw=showWrinkleCommonLine(imgSrc);
    //    cv::imwrite(imgout+"output.png", imgDraw);
    
    
}

WebSegment::~WebSegment(){
}

std::string WebSegment::getOutImg(){
    std::string res=imgout+"output.png";
    return res;
    
}

cv::Mat WebSegment::getAnsImg(cv::Mat &img){
    cv::Mat imgSrc=img.clone();
    cv::Mat imgDest=imgToRidge(imgSrc);
    cv::Mat imgAns=showWrinkleCommonLine(imgDest);
    return imgAns;
    
}


cv::Mat WebSegment::imgDeleteWords(cv::Mat &img){
    cv::Mat dest(img.size(),CV_8UC1);
    
    return dest;
}

cv::Mat WebSegment::imgToRidge(cv::Mat img){
    //    cv::resize(img, img, cv::Size(640, 360), (0, 0), (0, 0), cv::INTER_CUBIC);
    cv::Mat matSrc1=img.clone();
    //    cv::cvtColor(matSrc, matSrc, CV_BGR2GRAY);
    cv::Mat matDst=img.clone();
    //    std::vector<cv::Mat> labimg;
    //    cv::split(matSrc1, labimg);
    //    matSrc1=labimg[0];
    //    cv::imwrite(imgout+"gray0.png", matSrc1);
    //    cv::cvtColor(matSrc1, matDst, CV_BGR2GRAY);
    cv::imwrite(imgout+"gray1.png", matSrc1);
    
    int threshold = 0;
    float amount = 6;
    cv::Mat imgblurred;
    cv::GaussianBlur(matSrc1, imgblurred, cv::Size(3, 3), 0, 0);
    cv::Mat lowcontrastmask = abs(matSrc1 - imgblurred)<threshold;
    matDst = matSrc1*(1 + amount) + imgblurred*(-amount);
    matSrc1.copyTo(matDst, lowcontrastmask);
    
    float alpha = 1.1;
    float beta = -16;
    for (int y = 0; y < matDst.rows; y++)
    {
        for (int x = 0; x < matDst.cols; x++)
        {
            matDst.at<uchar>(y, x)= cv::saturate_cast<uchar>(alpha*(matDst.at<uchar>(y, x)) + beta);
        }
    }
    
    //        cv::equalizeHist(matDst, matDst);
    //    cv::Mat diamond = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    //    cv::morphologyEx(matDst, matDst, cv::MORPH_OPEN, diamond);
    cv::imwrite(imgout+"gray2.png", matSrc1);
    
    //end gray
    
    
    int scale = 1;
    int delta = 0;
    if( !matDst.data )
    {
        std::cout<<"load gray image error!"<<std::endl;
        return matDst;
    }
    
    cv::Mat grad,grad_temp;;
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    
    
    cv::Sobel( matDst, grad_x, CV_16S, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );
    
    cv::Sobel( matDst, grad_y, CV_16S, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );
    
    
    grad.setTo(0);
    cv::Mat check=cv::abs(abs_grad_x)<cv::abs(abs_grad_y);
    abs_grad_x.copyTo(grad);
    abs_grad_y.copyTo(grad,check);
    //    cv::imwrite(imgout+"graytidu00.png", grad);
    //    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    //    cv::imwrite(imgout+"graytidu001.png", grad);
    
    
    //    grad.convertTo(grad, CV_32FC1);
    //    abs_grad_x.convertTo(abs_grad_x, CV_32FC1);
    //    abs_grad_y.convertTo(abs_grad_y, CV_32FC1);
    //    cv::Mat angle;
    //    cv::cartToPolar(abs_grad_x, abs_grad_y, grad, angle);
    //    cv::imwrite(imgout+"graytidu.png", grad);
    
    //end grident
    
    frangi2d_opts_t opt;
    opt.sigma_start=1;
    opt.sigma_end=3;
    opt.sigma_step=1;
    opt.BetaOne=0.5;
    opt.BetaTwo=15;
    opt.BlackWhite=true;
    
    
    cv::Mat scales(grad.size(),CV_32FC1);
    cv::Mat directions(grad.size(),CV_32FC1);
    cv::Mat src(grad.size(),CV_32FC1);
    
    grad.convertTo(src, CV_32FC1);
    cv::Mat outfrangi;
    frangi2d(src,outfrangi,scales,directions,opt);
    outfrangi.convertTo(outfrangi, CV_8UC1);
    
    //end frangi
    cv::Mat out,out_x(grad.size(),CV_8U),out_y(grad.size(),CV_8U);
    //    outfrangi.convertTo(outfrangi, CV_8UC1);
    //    cv::imwrite(imgout+"xfrange1.png", outfrangi);
    
    cv::threshold(outfrangi, out, 0, 255,CV_THRESH_BINARY);
    //    cv::imwrite(imgout+"xfrange2.png", out);
    //        cv::medianBlur(out, out, 3);
    
    return out;
    
    
}

bool cmp(std::vector<cv::Point> a,std::vector<cv::Point> b){
    return (cv::contourArea(a)>cv::contourArea(b));
}

cv::Mat WebSegment::showWrinkleCommonLine(cv::Mat mat){
    //    cv::Mat src=mat.clone();
    cv::Mat mark=mat.clone();
    
    //****************************
    
    std::vector<std::vector<cv::Point> > contours_small;
    cv::threshold(mark, mark, 100, 255,CV_THRESH_BINARY );
    
    //filter
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::Mat mark2=mark.clone();
    cv::morphologyEx(mark2,mark2,cv::MORPH_OPEN,getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
    //        cv::imwrite(imgout+"resultfushi.png", mark2);
    
    
    cv::findContours(mark2, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    cv::Mat resultMask = cv::Mat ::zeros(mark.size(),CV_8U);
    resultMask.setTo(0);
    cv::Mat resultImage = cv::Mat ::zeros(mark.size(),CV_8U);
    
    sort(contours.begin(),contours.end(),cmp);
    
    for(int i=0;i<contours.size();i++)
    {
        std::vector<std::vector<cv::Point> >::iterator it=contours.begin()+i;
        //        cv::Point2f center;
        //        float radius;
        //        minEnclosingCircle(contours[i],center,radius);
        //        float cirArea=3.1415926*radius*radius;
        //        float div=contourArea(contours[i])/cirArea;
        
        if(cv::contourArea(contours[i])<300 )
        {
            contours.erase(it);
            i--;
        }
        
    }
    
    //    contours_small.clear();
    //    sort(contours.begin(),contours.end(),cmp);
    //    for(int i=0;i<contours.size();i++)
    //    {
    //                if(i>10)break;
    //        contours_small.push_back(contours[i]);
    //
    //
    //    }
    
    drawContours(resultMask, contours, -1, cv::Scalar(255),CV_FILLED);
    //    cv::threshold(resultMask, resultMask, 0, 255, CV_THRESH_BINARY);
    cv::Mat resultImage1;
    
    //    cv::imwrite(imgout+"resultImagemark.png", resultMask);
    
    mark.copyTo(resultImage, resultMask);
    
    //    cv::morphologyEx(resultImage,resultImage,cv::MORPH_OPEN,getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
    cv::imwrite(imgout+"resultImage.png", resultImage);
    
    //    mark2=resultImage.clone();
    //    contours.clear();
    //    hierarchy.clear();
    //    contours_small.clear();
    //    resultMask.setTo(0);
    //    findContours(mark2, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    //    sort(contours.begin(),contours.end(),cmp);
    //    for(int i=0;i<contours.size();i++)
    //    {
    ////        if(i>60)break;
    //        contours_small.push_back(contours[i]);
    //
    //
    //    }
    //    drawContours(resultMask, contours_small, -1, cv::Scalar(255),CV_FILLED);
    //    resultImage.copyTo(resultImage1, resultMask);
    //
    //
    //
    //    cv::imwrite("/Users/zrain/Desktop/pic/wrinkle_mid/wrinkle_resultImage1.bmp", resultImage1);
    
    
    
    
    //*********************************
    //    cv::Mat mark3(resultImage.size(),CV_8UC1);
    //    mark3.setTo(0);
    //    vector<int> ans_area;
    //    vector<float> ans_len;
    
    //    int num=0;
    //    for(int r=resultImage.rows-1;r>=0;r--)
    //        for(int c=0;c<resultImage.cols;c++)
    //        {
    //            if((int)resultImage1.at<uchar>(r,c)>250 && (int)mark3.at<uchar>(r,c)==0)
    //            {
    //                num++;
    //                int area=0,len=0;
    //                int rr=r,cc=c;
    //                countWrinkle(resultImage1, mark3, r, c, num,area,len);
    //                //float tlen=sqrt((rr-r)*(rr-r)+(cc-c)*(cc-c));
    //                ans_len.push_back(len);
    //                ans_area.push_back(area);
    //
    //            }
    //
    //        }//for
    //
    //    sort(ans_len.begin(),ans_len.end());
    //    sort(ans_area.begin(),ans_area.end());
    //
    //    for(int i=0;i<ans_len.size();i++){
    //        cout<<"第"<<i<<"根皱纹： 长度"<<ans_len[i]<<"(像素)-----  面积："<<ans_area[i]<<"像素。"<<endl;
    //
    //    }
    
    
    
    
    
    //begin to draw
    //    for(int r=0;r<src.rows;r++)
    //        for(int c=0;c<src.cols;c++)
    //        {
    //            if((int)resultImage.at<uchar>(r,c)>225){
    //                src.at<cv::Vec3b>(r,c)[0]=25;
    //                src.at<cv::Vec3b>(r,c)[1]=25;
    //                src.at<cv::Vec3b>(r,c)[2]=255;
    //                
    //                
    //            }
    //            
    //        }
    
    return resultImage;
    
}

bool cmp1(std::vector<cv::Point> a,std::vector<cv::Point> b){
    return (cv::contourArea(a)>cv::contourArea(b));
}

cv::Mat findline(cv::Mat &img,int lenHigh,int lenWidth,int miss){
    cv::Mat dest=cv::Mat::zeros(img.size(), CV_8UC1);
    //    cv::Mat dest(img.size(),CV_8UC1);
    int nr= img.rows;
    int nc= img.cols ;
    for (int j=0; j<nr;j++) {
        for (int i=0; i<nc; i+=2) {
            int leng=i+lenWidth;
            if (leng>=nc){
                break;
            }
            int sum=0;
            for(int k=i;k<leng;k++){
                if(img.at<uchar>(j,k)<200){
                    sum++;
                }
            }
            if(sum<miss){
                for(int k=0;k<nc;k++){
                    dest.at<uchar>(j,k)=255;
                }
                break;
            }
            //            i=leng;
        }//for
    }//for
    //stright line
    for (int i=0; i<nc;i++) {
        for (int j=0; j<nr; j+=2) {
            int leng=j+lenHigh;
            if (leng>=nr){
                break;
            }
            int sum=0;
            for(int k=j;k<leng;k++){
                if(img.at<uchar>(k,i)<200){
                    sum++;
                }
            }
            if(sum<miss){
                for(int k=0;k<nr;k++){
                    dest.at<uchar>(k,i)=255;
                }
                break;
            }
            //            j=leng;
            
        }//for
    }//for
    //    cv::imwrite(imgout11+"imgdest.png", dest);
    
    cv::dilate(dest, dest, getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
    cv::Mat ans=cv::Mat::zeros(img.size(),img.type());
    img.copyTo(ans, dest);
    //    cv::imwrite(imgout11+"imgans1.png", ans);
    
    std::vector<std::vector<cv::Point> > contours_small;
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::Mat mark=cv::Mat::zeros(img.size(),img.type());
    ans.copyTo(mark);
    //    cv::imwrite(imgout11+"imgmark.png", mark);
    cv::findContours(mark, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
    cv::Mat resultMask = cv::Mat ::zeros(img.size(),CV_8U);
    resultMask.setTo(0);
    cv::Mat resultImage = cv::Mat ::zeros(img.size(),CV_8U);
    
    std::sort(contours.begin(),contours.end(),cmp1);
    
    for(int i=0;i<contours.size();i++)
    {
        std::vector<std::vector<cv::Point> >::iterator it=contours.begin()+i;
        if(cv::contourArea(contours[i])<200 )
        {
            contours.erase(it);
            i--;
        }
    }
    //    cv::imwrite(imgout11+"imgmark2.png", mark);
    drawContours(resultMask, contours, -1, cv::Scalar(255),CV_FILLED);
    //    cv::imwrite(imgout11+"imgresultMask.png", resultMask);
    cv::Mat ans2=cv::Mat::zeros(img.size(),img.type());
    ans.copyTo(ans2, resultMask);
    
    return ans2;
    //    cv::imwrite(imgout11+"imgling2.png", ans);
}




