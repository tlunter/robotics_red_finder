#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "red_finder/red_finder.h"

#define HISTORY_SIZE 10

int main(int argc, char **argv)
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
        return -1;

    cv::Mat *frame;
    cv::Mat *clear;

    cv::namedWindow("Red colors", 1);

    char history[HISTORY_SIZE] = {0};
    unsigned int historyIndex = 0;

    frame = new cv::Mat();

    cap >> *frame;
    int midpoint = frame->cols / 2;
    frame->release();

    for (;;)
    {
        frame = new cv::Mat();
        clear = new cv::Mat();

        cap >> *frame;
        findRedColors(frame, clear);

        cv::imshow("Red colors", *clear);

        float xmean = 0;
        int points = 0;

        for (int i = 0; i < clear->rows; i++)
        {
            for (int j = 0; j < clear->cols; j++)
            {
                if (clear->at<uchar>(i,j) > 127)
                {
                    xmean += j;
                    points += 1;
                }
            }
        }

        if (points > 150)
        {
            xmean /= points;
        }
        else
        {
            xmean = -1;
        }

        std::cout << "X: " << xmean << std::endl;

        if (xmean < 0)
        {
            history[historyIndex++ % HISTORY_SIZE] = 0;
        }
        else if (xmean < (midpoint - 30))
        {
            history[historyIndex++ % HISTORY_SIZE] = 1;
        }
        else if (xmean > (midpoint + 30))
        {
            history[historyIndex++ % HISTORY_SIZE] = 3;
        }
        else
        {
            history[historyIndex++ % HISTORY_SIZE] = 2;
        }

        if (historyIndex > HISTORY_SIZE)
        {
            char noneCount = 0;
            char leftCount = 0;
            char centerCount = 0;
            char rightCount = 0;
            char maxCount = 0;
            char maxsDir = -1;
            for (int i = 0; i < HISTORY_SIZE; i++)
            {
                switch (history[i])
                {
                    case 0:
                        noneCount++;
                        if (maxCount <= noneCount)
                        {
                            maxCount = noneCount;
                            maxsDir = 0;
                        }
                        break;
                    case 1:
                        leftCount++;
                        if (maxCount <= leftCount)
                        {
                            maxCount = leftCount;
                            maxsDir = 1;
                        }
                        break;
                    case 2:
                        centerCount++;
                        if (maxCount <= centerCount)
                        {
                            maxCount = centerCount;
                            maxsDir = 2;
                        }
                        break;
                    case 3:
                        rightCount++;
                        if (maxCount <= rightCount)
                        {
                            maxCount = rightCount;
                            maxsDir = 3;
                        }
                        break;
                }
            }

            if (maxsDir == 0)
                std::cout << "None" << std::endl;
            else if (maxsDir == 1)
                std::cout << "Left" << std::endl;
            else if (maxsDir == 2)
                std::cout << "Center" << std::endl;
            else if (maxsDir == 3)
                std::cout << "Right" << std::endl;
        }

        frame->release();
        clear->release();
        if(cv::waitKey(30) == 27) break;
    }

    return 0;
}
