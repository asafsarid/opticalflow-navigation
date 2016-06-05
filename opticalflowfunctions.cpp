
#include "opticalflowfunctions.h"
#include <iostream>

using namespace std;

/* create custom grid for each section */
void createGrid(vector<cv::Point2f> &grid, int16_t wRes, int16_t hRes, int step){
    for (int i= wRes * 0.09; i < wRes * 0.91; i+=step)
        for (int j= hRes * 0.09; j < wRes * 0.91; j+=step)
            grid.push_back(cv::Point2f(i,j));
}

/* this function finds the right location according yaw angle*/
locationStruct calculateNewLocationByYaw(locationStruct lastFlowStep){
    float yYaw = eulerFromSensors.yaw - M_PI/2;
    locationStruct outputLocation;

    outputLocation.x = lastFlowStep.x * cos(eulerFromSensors.yaw) + lastFlowStep.y * cos(yYaw);
    outputLocation.y = -(lastFlowStep.x * sin(eulerFromSensors.yaw) + lastFlowStep.y * sin(yYaw));

    return outputLocation;
}

/* rotates the image - applying perspective transformation*/
void rotateImage(const Mat &input, UMat &output, double roll, double pitch, double yaw,
                 double dx, double dy, double dz, double f, double cx, double cy)
  {
    // Camera Calibration Intrinsics Matrix
    Mat A2 = (Mat_<double>(3,4) <<
              f, 0, cx, 0,
              0, f, cy, 0,
              0, 0, 1,  0);
    // Inverted Camera Calibration Intrinsics Matrix
    Mat A1 = (Mat_<double>(4,3) <<
              1/f, 0,   -cx/f,
              0,   1/f, -cy/f,
              0,   0,   0,
              0,   0,   1);
    // Rotation matrices around the X, Y, and Z axis
    Mat RX = (Mat_<double>(4, 4) <<
              1, 0,         0,          0,
              0, cos(roll), -sin(roll), 0,
              0, sin(roll), cos(roll),  0,
              0, 0,         0,          1);
    Mat RY = (Mat_<double>(4, 4) <<
              cos(pitch),  0, sin(pitch), 0,
              0,           1, 0,          0,
              -sin(pitch), 0, cos(pitch), 0,
              0,           0, 0,          1);
    Mat RZ = (Mat_<double>(4, 4) <<
              cos(yaw), -sin(yaw), 0, 0,
              sin(yaw),  cos(yaw), 0, 0,
              0,          0,       1, 0,
              0,          0,       0, 1);
    // Translation matrix
    Mat T = (Mat_<double>(4, 4) <<
             1, 0, 0, dx,
             0, 1, 0, dy,
             0, 0, 1, dz,
             0, 0, 0, 1);
    // Compose rotation matrix with (RX, RY, RZ)
    Mat R = RZ * RY * RX;
    // Final transformation matrix
    Mat H = A2 * (T * (R * A1));
    // Apply matrix transformation
    warpPerspective(input, output, H, input.size(), INTER_LANCZOS4);
}

/* Calculate optical flow per section */
void *OpticalFlowPerSection(void *currSectionInfo)
{
    // 1. cast the input pointer to the desired format
    sectionInfo *currSection = (sectionInfo *)currSectionInfo;
    vector<Point2f> pyrLKOutput;
    std::vector<unsigned char> status;
    std::vector<float> error;

    // 2. send to optical flow algorithm
    cv::calcOpticalFlowPyrLK(
                currSection->prevFrameSection, currSection->frameSection, // 2 consecutive images
                currSection->grid, // input point positions in first im
                pyrLKOutput, // output point positions in the 2nd
                status,    // tracking success
                error,      // tracking error
                Size(21,21),
                3,
                TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 5, 0.01)
                    );

    int counter = 0;
    float distPixelx = 0;
    float distPixely = 0;

    //get delta x,y for every point in grid
    for(size_t i = 0; i < pyrLKOutput.size(); ++i) {
        if(!status[i]) {
          continue;
        }
        const Point2f& currPoint = pyrLKOutput[i];
        const Point2f& prevPoint = currSection->grid[i];
//		line(currSection->prevFrameSection, currSection->grid[i], pyrLKOutput[i], Scalar(0, 255, 0));
//		circle(currSection->prevFrameSection, currSection->grid[i], 2, Scalar(0, 255, 0), -1);
        distPixelx += currPoint.x - prevPoint.x;
        distPixely += currPoint.y - prevPoint.y;
        counter++;
    }

    // average
    if (counter != 0){
        lastFlowStepSections[currSection->index].x = distPixelx/((float)counter);
        lastFlowStepSections[currSection->index].y = distPixely/((float)counter);
    }
    else {
        lastFlowStepSections[currSection->index].x = 0;
        lastFlowStepSections[currSection->index].y = 0;
    }

    return NULL;
}
