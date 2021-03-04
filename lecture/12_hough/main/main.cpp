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

typedef struct {
	int	index;
	double	slope;
	int	intercept;
	int	xaty0;
} EDGE;

void detect_line(Mat edge, Mat line_mat, unsigned nlines)
{
	vector<Vec4i> linesP;
	HoughLinesP(edge, linesP, 1, CV_PI/180,  64, 100, 50);		// SAKUDAIRA
	cvtColor(edge, line_mat, COLOR_GRAY2BGR);
	for(size_t i = 0; i < linesP.size(); i++) {
        	Vec4i l = linesP[i];
        	line(line_mat, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,200), 1, LINE_AA);
	}
	if (nlines > linesP.size()) nlines = linesP.size();
	for(size_t i = 0; i < nlines ; i++) {
        	Vec4i l = linesP[i];
        	line(line_mat, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,255,0), 2, LINE_AA);
	}
	imshow("lines", line_mat);
}

const char *help =
" m   | change mode (high/low)\n"
" +   | increment thresh value\n"
" -   | decrement thresh value\n"
" l   | detect lines\n"
" i   | increment lines\n"
" d   | decrement lines\n"
" ?   | show this help message\n"
" ESC | exit application\n";

int main(int argc, char **argv)
{
	int	n, *val_ptr, edit_low = 1, thresh_low = 128, thresh_high =200;
	unsigned	nlines = 2;
	char	ch;

	val_ptr = &thresh_low;

	remove_ftdi_sio();
	sdk = (ISCSDKLib*)new ISCSDKLib();
	n = sdk->OpenISC();
	check_if_0("OpenISC", n);
	atexit(bye);

	read_camera_param();
	get_size(&width, &height);
	maxcm = 1000;

	n = sdk->SetShutterControlMode(true);	// auto gain/exposure
	check_if_0("SetShutterControlMode", n);

	n = sdk->SetAutoCalibration(1);		// auto mode
	check_if_0("SetAutoCalibration", n);

	n = sdk->StartGrab(2);	// 2:depth , 3:calibrated 4: original 
	check_if_0("StartGrab", n);

	Mat lmat = Mat::zeros(height, width, CV_8UC1);
	Mat rmat = Mat::zeros(height, width, CV_8UC1);
	Mat colmat = Mat::zeros(height, width, CV_8UC3);
	Mat edge = Mat::zeros(height, width, CV_8UC3);
	Mat line_mat = Mat::zeros(height, width, CV_8UC3);
#if 1
	namedWindow("color");
	moveWindow("color", 0, 0);
	namedWindow("original");
	moveWindow("original", rmat.cols+10, 0);
#endif
	do {
		n = sdk->GetImage((unsigned char*)lmat.data, (unsigned char*)rmat.data, 1); 
	} while (n != 0)
		;
loop:
	Canny(rmat, edge, thresh_low, thresh_high);	
	cvtColor(edge, colmat, cv::COLOR_GRAY2RGB);
	mat_printf(colmat, 4, 24, "low: %3d, high: %3d, lines: %d",
		      thresh_low, thresh_high, nlines);
	imshow("color", colmat);
	imshow("original", rmat);
	ch = waitKey(30);
	if (ch == 0x1b) {
		goto end;
	} else if (ch == 'm') {
		edit_low ^= 1;
		if (edit_low)
			val_ptr = &thresh_low;
		else
			val_ptr = &thresh_high;
	} else if (ch == 'i') {
		++nlines;
	} else if (ch == 'd') {
		if (nlines > 2) --nlines;
	} else if (ch == '+') {
		if (*val_ptr < 255) ++*val_ptr;
	} else if (ch == '-') {
		if (*val_ptr) --*val_ptr;
	} else if (ch == 'l') {
		memset(line_mat.data, 0, (size_t)(line_mat.cols*line_mat.rows));
		detect_line(edge, line_mat, nlines);
	} else if (ch == '?' || ch == 'h') {
		printf("%s", help);
	}
	if (thresh_high < thresh_low) thresh_high = thresh_low;
	goto loop;
end:	
	return 0;
}
