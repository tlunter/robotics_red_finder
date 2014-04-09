#include <cv.h>
#include <highgui.h>
#include "red_capture/red_finder.h"

void findRedColors(cv::Mat *src, cv::Mat *dest)
{
    cv::Mat *hsv = new cv::Mat();
    cv::Mat *redA = new cv::Mat();
    cv::Mat *redB = new cv::Mat();
    cv::Mat *threshed = new cv::Mat();

    cv::cvtColor(*src, *hsv, CV_BGR2HSV_FULL);

    cv::cvtColor(*src, *redA, CV_BGR2GRAY);
    cv::cvtColor(*src, *redB, CV_BGR2GRAY);
    cv::cvtColor(*src, *threshed, CV_BGR2GRAY);
    cv::cvtColor(*src, *dest, CV_BGR2GRAY);

    cv::inRange(*hsv, cv::Scalar(0, 70, 70), cv::Scalar(10, 255, 255), *redA);
    cv::inRange(*hsv, cv::Scalar(169, 70, 70), cv::Scalar(179, 255, 255), *redB);

    cv::bitwise_or(*redA, *redB, *threshed);

    cv::GaussianBlur(*threshed, *dest, cv::Size(3, 3), 0);

    hsv->release();
    redA->release();
    redB->release();
    threshed->release();
}
