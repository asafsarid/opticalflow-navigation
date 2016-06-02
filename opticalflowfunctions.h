#ifndef OPTICALFLOWFUNCTIONS_H
#define OPTICALFLOWFUNCTIONS_H

#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
#include "globals.h"

using namespace cv;
using namespace std;

void createGrid(vector<cv::Point2f> &grid, int16_t wRes, int16_t hRes, int step);
locationStruct calculateNewLocationByYaw(locationStruct lastFlowStep);
void rotateImage(const Mat &input, UMat &output, double roll, double pitch, double yaw,
                 double dx, double dy, double dz, double f, double cx, double cy);
void *OpticalFlowPerSection(void *currSectionInfo);

typedef struct{
    UMat frameSection;
    UMat prevFrameSection;
    vector<Point2f> grid;
    int32_t index;
}sectionInfo;

#endif // OPTICALFLOWFUNCTIONS_H
