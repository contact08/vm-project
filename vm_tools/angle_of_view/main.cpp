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
#include "libx3.h"

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

void put_marker(Mat mat, int x, int y, Scalar col)
{
	rectangle(mat, Point(x-5, y-5), Point(x+5, y+5), col, 1);
}

int iabs_sub(int n1, int n2)
{
	if (n1 > n2) return n1 - n2;
	return n2 - n1;
}

const char *help =
" d   | change target distance\n"
" h   | change target width\n"
" v   | change target height\n"
" +   | increase cm\n"
" -   | decrease cm\n"
" R   | up marker\n"
" T   | down marker\n"
" S   | right marker\n"
" Q   | left marker\n"
" ?   | show this help message\n"
" ESC | exit application\n";

int main(int argc, char **argv)
{
#define ZOOMW 50
	int	n, dcm = 239, hcm= 100, vcm = 100, *ptr_cm;
	int	x[2], y[2], index_xy = 0;
	char	ch;
	float	vdeg, hdeg;		
	double	vview, hview;

	ptr_cm = &dcm;

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
	Mat roi_mat;
	Mat zoom_mat = Mat::zeros(ZOOMW*4, ZOOMW*4, CV_8UC1);

	x[0] = x[1] = colmat.cols/2;
	y[0] = colmat.rows/2 - 20;
	y[1] = colmat.rows/2 + 20;
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
	rectangle(colmat, Point(0, 0), Point(100, 20), Scalar(0, 0, 0), -1);
	put_marker(colmat, x[1], y[1], Scalar(0,255,255));
	put_marker(colmat, x[0], y[0], Scalar(0,0,255));
	rectangle(colmat, Point(0, 0), Point(lmat.cols/2, 90), Scalar(0,0,0), -1);
	mat_printf(colmat, 4, 20, "distance:%4dcm", dcm);
	mat_printf(colmat, 4, 40, "horz.: %dcm, vert.: %dcm", hcm, vcm);
	hview = (double)(hcm*width)/(double)(iabs_sub(x[0],x[1]));
	vview = (double)(vcm*height)/(double)(iabs_sub(y[0],y[1]));
	vdeg = 2.0*R2D(atan((vview/2.0)/(double)dcm));
	hdeg = 2.0*R2D(atan((hview/2.0)/(double)dcm));
	mat_printf(colmat, 4, 60, "V: %dcm, %3.3fdeg", (int)vview, vdeg);
	mat_printf(colmat, 4, 80, "H: %dcm, %3.3fdeg", (int)hview, hdeg);
	//mat_printf(colmat, 4, 100, "x0:%4d, y0:%d", x[0], y[0]);
	//mat_printf(colmat, 4, 120, "x1:%4d, y1:%d", x[1], y[1]);
	roi_mat = rmat(Rect(x[index_xy]-ZOOMW,y[index_xy]-ZOOMW,ZOOMW*2,ZOOMW*2));
	zoom(roi_mat, zoom_mat);
	adj_contrast(zoom_mat, 4.0);
	line(zoom_mat, Point(0, 0), Point(zoom_mat.cols, zoom_mat.rows), Scalar(0,0,0), 2);
	line(zoom_mat, Point(0, zoom_mat.rows), Point(zoom_mat.cols, 0), Scalar(0,0,0), 2);
	imshow("color", colmat);
	imshow("zoom", zoom_mat);
	ch = waitKey(30);
	if (ch == 0x1b) {
		goto end;
	} else if (ch == '0') {
		index_xy = 0;
	} else if (ch == '1') {
		index_xy = 1;
	} else if (ch == 'd') {
		ptr_cm = &dcm;
	} else if (ch == 'v') {
		ptr_cm = &vcm;
	} else if (ch == 'h') {
		ptr_cm = &hcm;
	} else if (ch == '+') {
		if ((*ptr_cm += 1) > maxcm) *ptr_cm = maxcm;
	} else if (ch == '-') {
		if ((*ptr_cm -= 1) < 10) *ptr_cm = 10;
	} else if (ch == '?' || ch == 'h') {
		printf("%s", help);
	} else if (ch == 'R') {
		if (--y[index_xy] < 0) y[index_xy] = 0;
	} else if (ch == 'T') {
		if (++y[index_xy] > colmat.rows) y[index_xy] = colmat.rows;
	} else if (ch == 'Q') {
		if (--x[index_xy] < 0) x[index_xy] = 0;
	} else if (ch == 'S') {
		if (++x[index_xy] > colmat.cols) x[index_xy] = colmat.cols;
	} else {
		if ((ch & 255) != 0xff) printf("%02x(%c)\n", ch & 255, ch);
	}
	goto loop;
end:	
	return 0;
}
