#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <sys/time.h>
#include "red_finder/red_finder.h"
#include "red_finder/uart_writer.h"

int main(int argc, char **argv)
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
        return -1;

    cv::Mat *frame;
    cv::Mat *clear;

    timeval a;
    timeval b;

    long seconds;
    long useconds;
    double duration;
    int loops = 0;

    char direction;

    int uart = open_uart();

    gettimeofday(&a, 0);

    for (;;)
    {
        frame = new cv::Mat();
        clear = new cv::Mat();

        cap >> *frame;
        findRedColors(frame, clear);

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
        else
        {
            xmean = -1;
        }

        std::cout << "X: " << xmean << " Y: " << ymean << std::endl;

        int midpoint = clear->cols / 2;

        if (xmean < 0)
        {
            std::cout << "None" << std::endl;
            direction = 0x40;
        }
        else if (xmean < (midpoint - 20))
        {
            std::cout << "Left" << std::endl;
            direction = 0x41;
        }
        else if (xmean > (midpoint + 20))
        {
            std::cout << "Right" << std::endl;
            direction = 0x43;
        }
        else
        {
            std::cout << "Center" << std::endl;
            direction = 0x42;
        }

        uart_write(uart, direction);

        gettimeofday(&b, 0);

        seconds  = b.tv_sec  - a.tv_sec;
        useconds = b.tv_usec - a.tv_usec;

        loops += 1;
        duration = seconds + useconds/1000000.0;

        std::cout << loops/duration << " Loops/second" << std::endl << std::endl;

        frame->release();
        clear->release();
    }

    close(uart);

    return 0;
}
