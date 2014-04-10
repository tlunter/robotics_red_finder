#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <unistd.h>
#include "red_finder/red_finder.h"
#include "red_finder/uart_writer.h"

#define HISTORY_SIZE 3
#define POINTS_NEEDED 50

int main(int argc, char **argv)
{
    cv::VideoCapture cap(0);
    if (!cap.isOpened())
        return -1;

    cv::Mat *frame;
    cv::Mat *clear;

    char direction;

    char history[HISTORY_SIZE] = {0};
    unsigned int historyIndex = 0;

    // Initialize the midpoint of the frame
    frame = new cv::Mat();

    cap >> *frame;
    int midpoint = frame->cols / 2;
    frame->release();

    // Start UART connection
    int uart = open_uart();
    if (uart < 0)
        return -1;

    char lastAngleSent = -1;

    // Let Arduino reset before spamming serial connection
    sleep(5);

    for (;;)
    {
        frame = new cv::Mat();
        clear = new cv::Mat();

        cap >> *frame;
        findRedColors(frame, clear);

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

        if (points > POINTS_NEEDED)
        {
            xmean /= points;
        }
        else
        {
            xmean = -1;
        }

        //std::cout << "X: " << xmean << std::endl;

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
            for (unsigned int i = historyIndex - HISTORY_SIZE; i < historyIndex; i++)
            {
                switch (history[i % HISTORY_SIZE])
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
            {
                //std::cout << "None" << //std::endl;
                direction = 0x40;
            }
            else if (maxsDir == 1)
            {
                //std::cout << "Left" << //std::endl;
                direction = 0x41;
            }
            else if (maxsDir == 2)
            {
                //std::cout << "Center" << //std::endl;
                direction = 0x42;
            }
            else if (maxsDir == 3)
            {
                //std::cout << "Right" << //std::endl;
                direction = 0x43;
            }

            if (maxsDir != lastAngleSent)
            {
                if (!uart_write(uart, direction))
                    return -1;

                lastAngleSent = maxsDir;

                if (maxsDir == 0)
                {
                    std::cout << "None" << std::endl;
                }
                else if (maxsDir == 1)
                {
                    std::cout << "Left" << std::endl;
                }
                else if (maxsDir == 2)
                {
                    std::cout << "Center" << std::endl;
                }
                else if (maxsDir == 3)
                {
                    std::cout << "Right" << std::endl;
                }
            }
        }

        if (!uart_read(uart))
            return -1;

        frame->release();
        clear->release();

        //usleep(25000);
    }

    close(uart);

    return 0;
}
