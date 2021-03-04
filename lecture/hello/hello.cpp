#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
using namespace cv;
int main(int argc, char **argv)
{
	if (argc < 2) return 0;
	Mat img = imread(argv[1]);
	if(img.empty()) {
		printf("%s can't open\n", argv[1]);
        	return 1;
	}
	imshow("image", img);
	waitKey(0); // Wait for a keystroke in the window
	return 0;
}
