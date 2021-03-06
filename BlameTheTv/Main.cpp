#include <iostream>
#include <stdint.h>
#include "opencv2/opencv.hpp"

#define KEY_ESC     (27)
#define KEY_RIGHT   (2555904)
#define KEY_LEFT    (2424832)
#define KEY_DEL     (3014656)
#define KEY_RET     (13)

using namespace cv;

// adjust rectangle size as needed
static const Rect LED_ROI(20, 180, 20, 20);
static bool detectLed(const Mat& frame);

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: BlameTheTv.exe video_file" << std::endl;
        return -1;
    }

    VideoCapture cap(argv[1]);
    if (!cap.isOpened())
    {
        std::cout << "Could not open input file " << argv[1] << std::endl;
        return -1;
    }

    std::string infile(argv[1]);
    std::string outfile = infile.substr(0, infile.find_last_of('.')) + ".csv";
    FILE* f;
    fopen_s(&f, outfile.c_str(), "w");
    if (!f)
    {
        std::cout << "Could not open output file " << outfile << std::endl;
        return -1;
    }
    std::cout << "Saving output to " << outfile << std::endl;

    Mat frame;
    uint64_t globalFrameNumber = 0;
    namedWindow("BlameTheTv", WINDOW_NORMAL);

    while (true)
    {
        std::cout << "-------------------------------------" << std::endl;

        uint64_t first;
        std::vector<Mat> hist;

        // Fast forward until LED is on
        first = globalFrameNumber;
        do
        {
            cap >> frame;
            if (frame.empty())
                goto QUIT;
            ++globalFrameNumber;
        } while (!detectLed(frame));

		// Fast forward until LED is off
        first = globalFrameNumber;
        do
        {
            cap >> frame;
            if (frame.empty())
                goto QUIT;
            ++globalFrameNumber;

            hist.push_back(frame.clone());
            while (hist.size() > 100)
                hist.erase(hist.begin());
        } while (detectLed(frame));

        // Go frame by frame
        const uint64_t timestamp = globalFrameNumber;
        const size_t startingFrames = hist.size();
        int relFrameNum = -2;
        bool slowMo = true;
        while (slowMo)
        {
            std::cout << timestamp << ": " << relFrameNum + 2 << std::endl;
            if (relFrameNum + startingFrames < hist.size())
            {
                hist[relFrameNum + startingFrames].copyTo(frame);
            }
            else
            {
                cap >> frame;
                if (frame.empty())
                    goto QUIT;
                ++globalFrameNumber;

                hist.push_back(frame.clone());
            }

            imshow("BlameTheTv", frame);
            switch (waitKey())
            {
				case KEY_RIGHT:
					++relFrameNum;
					break;

				case KEY_LEFT:
					if (relFrameNum + startingFrames > 0)
						--relFrameNum;
					break;

				case KEY_DEL:
					slowMo = false;
					break;

				case KEY_ESC:
					goto QUIT;

				case KEY_RET:
					fprintf(f, "%llu,%d\n", timestamp, relFrameNum + 2);
					slowMo = false;

				default:
					break;
            }
        }
    }

QUIT:
    fclose(f);

    return 0;
}

static bool detectLed(const Mat& frame)
{
	Mat color;
	// BLUE = 1, GREEN = 2, RED = 3
	extractChannel(frame(LED_ROI), color, 1);
	cvtColor(frame(LED_ROI), color, CV_BGR2GRAY);
	// adjust this threshold as needed
	threshold(color, color, 75, 255, THRESH_BINARY);
	std::cout << "green count: " << countNonZero(color) << std::endl;
	// adjust this threshold as needed
	return countNonZero(color) > 2;
}