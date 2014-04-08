#include <iostream>
#include <cv.h>
#include <highgui.h>

int main(int argc, char **argv)
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
        return -1;

    cv::Mat *frame;
    cv::Mat *clear;

    cv::namedWindow("Camera", 1);

    for (;;)
    {
        frame = new cv::Mat();

        cap >> *frame;

        cv::imshow("Camera", *frame);

        frame->release();
        if(cv::waitKey(30) == 27) break;
    }

    return 0;
}

