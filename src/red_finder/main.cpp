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

        std::cout << "Rows: " << clear->rows << " Cols: " << clear->cols << std::endl;

        float xmean = 0;
        float ymean = 0;
        int points = 0;

        for (int i = 0; i < clear->rows; i++)
        {
            for (int j = 0; j < clear->cols; j++)
            {
                if (clear->at<uchar>(i,j) > 127)
                {
                    xmean += j;
                    ymean += i;
                    points += 1;
                }
            }
        }

        if (points > 0)
        {
            xmean /= points;
            ymean /= points;
        }

        std::cout << "X: " << xmean << " Y: " << ymean << std::endl;

        int midpoint = clear->cols / 2;

        if (xmean < (midpoint - 20))
        {
            std::cout << "Left" << std::endl;
        }
        else if (xmean > (midpoint + 20))
        {
            std::cout << "Right" << std::endl;
        }
        else
        {
            std::cout << "Center" << std::endl;
        }

        frame->release();
        clear->release();
        if(cv::waitKey(30) == 27) break;
    }

    return 0;
}
