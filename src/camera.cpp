#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    namedWindow("edges",1);
    Mat frame;
    Mat hsv;
    Mat threshed;
    Mat redA;
    Mat redB;
    for(;;)
    {
        cap >> frame; // get a new frame from camera

        Mat hsv(frame.size(), frame.depth());
        cvtColor(frame, hsv, CV_BGR2HSV_FULL);

        int rotation = 128 - 255; // 255 = red
        add(hsv, Scalar(rotation, 0, 0), hsv);

        cvtColor(frame, redA, CV_BGR2GRAY);
        cvtColor(frame, redB, CV_BGR2GRAY);
        cvtColor(frame, threshed, CV_BGR2GRAY);

        inRange(hsv, Scalar(0, 80, 80), Scalar(20, 255, 255), redA);
        inRange(hsv, Scalar(159, 80, 80), Scalar(179, 255, 255), redB);

        bitwise_or(redA, redB, threshed);
        imshow("edges", threshed);
        if(waitKey(30) == 27) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
