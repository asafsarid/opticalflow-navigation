/*
 * perspective.cpp
 *
 *  Created on: Jan 18, 2016
 */
/* Includes */
// General
#include <math.h>
#include <iostream>
// OpenCV
#include "opencv2/video/tracking.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

/* Namespaces */
using namespace cv;
using namespace std;


//static double rad2Deg(double rad){return rad*(180/M_PI);}//Convert radians to degrees
static double deg2Rad(double deg){return deg*(M_PI/180);}//Convert degrees to radians

void warpMatrix(Size   sz,
                double theta,
                double phi,
                double gamma,
                double scale,
                double fovy,
                Mat&   M,
                vector<Point2f>* corners){
    double st=sin(deg2Rad(theta));
    double ct=cos(deg2Rad(theta));
    double sp=sin(deg2Rad(phi));
    double cp=cos(deg2Rad(phi));
    double sg=sin(deg2Rad(gamma));
    double cg=cos(deg2Rad(gamma));

    double halfFovy=fovy*0.5;
    double d=hypot(sz.width,sz.height);
    double sideLength=scale*d/cos(deg2Rad(halfFovy));
    double h=d/(2.0*sin(deg2Rad(halfFovy)));
    double n=h-(d/2.0);
    double f=h+(d/2.0);

    Mat F=Mat(4,4,CV_64FC1);//Allocate 4x4 transformation matrix F
    Mat Rtheta=Mat::eye(4,4,CV_64FC1);//Allocate 4x4 rotation matrix around Z-axis by theta degrees
    Mat Rphi=Mat::eye(4,4,CV_64FC1);//Allocate 4x4 rotation matrix around X-axis by phi degrees
    Mat Rgamma=Mat::eye(4,4,CV_64FC1);//Allocate 4x4 rotation matrix around Y-axis by gamma degrees

    Mat T=Mat::eye(4,4,CV_64FC1);//Allocate 4x4 translation matrix along Z-axis by -h units
    Mat P=Mat::zeros(4,4,CV_64FC1);//Allocate 4x4 projection matrix

    //Rtheta
    Rtheta.at<double>(0,0)=Rtheta.at<double>(1,1)=ct;
    Rtheta.at<double>(0,1)=-st;Rtheta.at<double>(1,0)=st;
    //Rphi
    Rphi.at<double>(1,1)=Rphi.at<double>(2,2)=cp;
    Rphi.at<double>(1,2)=-sp;Rphi.at<double>(2,1)=sp;
    //Rgamma
    Rgamma.at<double>(0,0)=Rgamma.at<double>(2,2)=cg;
    Rgamma.at<double>(0,2)=sg;Rgamma.at<double>(2,0)=sg;

    //T
    T.at<double>(2,3)=-h;
    //P
    P.at<double>(0,0)=P.at<double>(1,1)=1.0/tan(deg2Rad(halfFovy));
    P.at<double>(2,2)=-(f+n)/(f-n);
    P.at<double>(2,3)=-(2.0*f*n)/(f-n);
    P.at<double>(3,2)=-1.0;
    //Compose transformations
    F=P*T*Rphi*Rtheta*Rgamma;//Matrix-multiply to produce master matrix

    //Transform 4x4 points
    double ptsIn [4*3];
    double ptsOut[4*3];
    double halfW=sz.width/2, halfH=sz.height/2;

    ptsIn[0]=-halfW;ptsIn[ 1]= halfH;
    ptsIn[3]= halfW;ptsIn[ 4]= halfH;
    ptsIn[6]= halfW;ptsIn[ 7]=-halfH;
    ptsIn[9]=-halfW;ptsIn[10]=-halfH;
    ptsIn[2]=ptsIn[5]=ptsIn[8]=ptsIn[11]=0;//Set Z component to zero for all 4 components

    Mat ptsInMat(1,4,CV_64FC3,ptsIn);
    Mat ptsOutMat(1,4,CV_64FC3,ptsOut);

    perspectiveTransform(ptsInMat,ptsOutMat,F);//Transform points

    //Get 3x3 transform and warp image
    Point2f ptsInPt2f[4];
    Point2f ptsOutPt2f[4];

    for(int i=0;i<4;i++){
        Point2f ptIn (ptsIn [i*3+0], ptsIn [i*3+1]);
        Point2f ptOut(ptsOut[i*3+0], ptsOut[i*3+1]);
        ptsInPt2f[i]  = ptIn+Point2f(halfW,halfH);
        ptsOutPt2f[i] = (ptOut+Point2f(1,1))*(sideLength*0.5);
    }

    M=getPerspectiveTransform(ptsInPt2f,ptsOutPt2f);

    //Load corners vector
    if(corners){
        corners->clear();
        corners->push_back(ptsOutPt2f[0]);//Push Top Left corner
        corners->push_back(ptsOutPt2f[1]);//Push Top Right corner
        corners->push_back(ptsOutPt2f[2]);//Push Bottom Right corner
        corners->push_back(ptsOutPt2f[3]);//Push Bottom Left corner
    }
}


void warpImage(const Mat &src,
               double    theta,
               double    phi,
               double    gamma,
               double    scale,
               double    fovy,
               Mat&      dst,
               Mat&      M,
               vector<Point2f> &corners){
    double halfFovy=fovy*0.5;
    double d=hypot(src.cols,src.rows);
    double sideLength=scale*d/cos(deg2Rad(halfFovy));

    warpMatrix(src.size(),theta,phi,gamma, scale,fovy,M,&corners);//Compute warp matrix
//    warpMatrix(src.size(),0,0,0, scale,fovy,M,&corners);//Compute warp matrix
    warpPerspective(src,dst,M,Size(sideLength,sideLength));//Do actual image warp
}

