#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "red_finder/red_finder.h"

int main(int argc, char **argv)
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
        return -1;

    cv::Mat *frame;
    cv::Mat *clear;

    cv::namedWindow("Red colors", 1);

    for (;;)
    {
        frame = new cv::Mat();
        clear = new cv::Mat();

        cap >> *frame;
        findRedColors(frame, clear);

        cv::imshow("Red colors", *clear);

        frame->release();
        clear->release();
        if(cv::waitKey(30) == 27) break;
    }

    return 0;
}
