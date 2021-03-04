#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/types.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <ISCSDKLib.h>

using namespace cv;
using namespace std;

#include "libbasic.h"

ISCSDKLib	*sdk;
int	maxcm;
uchar	*red, *green, *blue;

void show_rgb_tbl(int max_cm, uchar *redp, uchar *greenp, uchar *bluep)
{
	Mat	mat(50,500,CV_8UC3);
	int	cm, x;
	char	title[256], center[64];
	mat = 0;
	for (cm = 0 ; cm < max_cm ; cm++) {
		x = (int)((double)(cm*mat.cols)/(double)max_cm);
		line(mat, Point(x, mat.rows/2), Point(x, mat.rows),
			Scalar(blue[cm], green[cm], red[cm]), 1);	
		if (cm == max_cm/2) {
			line(mat, Point(x, 0), Point(x, mat.rows),
				Scalar(255, 255, 255), 1);	
			sprintf(center, "%d", max_cm/2);
			putText(mat,			// Mat
			       	center,			// string
				Point(x+4,20),		// position
				FONT_HERSHEY_PLAIN,	// font
				1.0,			// font scale
            			Scalar(255,255,255),	// color
				1);			// thicklness
		}
	}
	sprintf(title, "color chart 0cm - %dcm", max_cm);
	imshow(title, mat);
}

int main(int argc, char **argv)
{
	maxcm = allocate_rgb_mem(1000, &red, &green, &blue);
	show_rgb_tbl(maxcm, red, green, blue);
	waitKey(0);
	destroyAllWindows();
	return 0;
}
