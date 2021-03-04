#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/fast_math.hpp>
#include <ISCSDKLib.h>

using namespace cv;
using namespace std;

#include "libbasic.h"

ISCSDKLib	*sdk = NULL;
uchar	*red, *green, *blue;
int	maxcm, width, height;

void close_sdk(void)
{
	if (sdk != NULL) {
		sdk->StopGrab();
		sdk->CloseISC();
	}
}

void bye(void)
{
	close_sdk();
	destroyAllWindows();
}

double put_horizontal_scale(Mat mat, float degree, int cm)
{
	double	fullw = 2.0 * (double)cm * tan(D2R(degree)/2.0);
	int	xcm, xpos, xpos2, h, thickness;
	Scalar	col;
	mat_printf_size(0.8);
	line(mat,
		Point(0, mat.rows/2),
		Point(mat.cols, mat.rows/2),
		Scalar(255,255, 0),
		1);
	for (xcm = 0 ; ; xcm += 10) {
		xpos = ((double)mat.cols*((double)xcm+fullw/2.0))/fullw;
		if (xpos >= mat.cols) break;
		xpos2 = xpos - mat.cols/2;
		xpos2 = mat.cols/2 - xpos2;
		if (!(xcm % 50)) {
			mat_printf(mat, xpos,  mat.rows/2-16, "%d", xcm);
			if (xcm) mat_printf(mat, xpos2, mat.rows/2-16, "-%d", xcm);
			h = 36;
			thickness = 2;
			col = Scalar(0, 0, 255);
		} else {
			h = 12;
			thickness = 2;
			col = Scalar(255, 255, 0);
		}
		line(mat,
			Point(xpos, mat.rows/2-h),
			Point(xpos, mat.rows/2),
			col,
			thickness);
		line(mat,
			Point(xpos2, mat.rows/2-h),
			Point(xpos2, mat.rows/2),
			col,
			thickness);
	}
	return fullw;
}

const char *help =
" f   | go far\n"
" n   | come near\n"
" ?   | show this help message\n"
" ESC | exit application\n";

int main(int argc, char **argv)
{
	int	n, cm = 200;
	char	ch;
	float	degree;		
	double	view;

	remove_ftdi_sio();
	sdk = (ISCSDKLib*)new ISCSDKLib();
	n = sdk->OpenISC();
	check_if_0("OpenISC", n);
	atexit(bye);

	read_camera_param();
	maxcm = 1000;
	get_size(&width, &height);
	printf("width: %d, height: %d\n", width, height);
	degree = get_view_angle();
	// adjust view angle for PP2
	degree = 93.082;		// PP2-041 only!!

	n = sdk->SetShutterControlMode(true);	// auto gain/exposure
	check_if_0("SetShutterControlMode", n);

	n = sdk->SetAutoCalibration(1);		// auto mode
	check_if_0("SetAutoCalibration", n);

	n = sdk->StartGrab(2);	// 2:depth , 3:calibrated 4: original 
	check_if_0("StartGrab", n);

	Mat lmat = Mat::zeros(height, width, CV_8UC1);
	Mat rmat = Mat::zeros(height, width, CV_8UC1);
	Mat colmat = Mat::zeros(height, width, CV_8UC3);
#if 1
	namedWindow("color");
	moveWindow("color", 0, 0);
#endif
loop:
	do {
		n = sdk->GetImage((unsigned char*)lmat.data, (unsigned char*)rmat.data, 1); 
	} while (n != 0)
		;
	cvtColor(rmat, colmat, cv::COLOR_GRAY2RGB);
	rectangle(colmat, Point(0, 0), Point(colmat.cols/2, 30), Scalar(0, 0, 0), -1);
	view = put_horizontal_scale(colmat, degree, cm);
	mat_printf(colmat, 4, 20, "distance:%4dcm, view:%dcm",
		     cm, (int)view);
	imshow("color", colmat);
	ch = waitKey(30);
	if (ch == 0x1b) {
		goto end;
	} else if (ch == 'f') {
		if ((cm += 10) > maxcm) cm = maxcm;
	} else if (ch == 'n') {
		if ((cm -= 10) < 10) cm = 10;
	} else if (ch == '?' || ch == 'h') {
		printf("%s", help);
	}
	goto loop;
end:	
	return 0;
}
