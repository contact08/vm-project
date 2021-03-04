#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/fast_math.hpp>
#include <ISCSDKLib.h>

using namespace cv;
using namespace std;

#include "libbasic.h"

ISCSDKLib	*sdk = NULL;
ISCSDKLib::CameraParamInfo	camera_param;
int	maxcm, near_cm = 100, far_cm = 200;
uchar	*red, *green, *blue;
static int	width, height;

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

void copy_depth(Mat gray, Mat fdepth, Rect rect)
{
	int	x, y, cm;
	int	x1 = rect.x;	// left	
	int	y1 = rect.y;	// upper	
	int	x2 = x1 + rect.width;	// right	
	int	y2 = y1 + rect.height;	// lower	
	float	fn;
	memset(gray.data, 0, (size_t)(gray.cols*gray.rows));	// clear mat
	for (y = y1 ;  y < y2 ; y++) {
		for (x = x1 ;  x < x2 ; x++) {
			fn = fdepth.at<float>(y, x);
			cm = depth2cm(fn);
			if (cm >= maxcm || cm <= 0) continue;
			if (cm <= near_cm || cm >= far_cm) continue;
			gray.at<uchar>(y, x) = 255;
		}
	}
}

unsigned count_mat(Mat mat)
{
	unsigned	non_zero = 0;
	for (int j = 0 ; j < mat.rows ; j++) {
		for (int i = 0 ; i < mat.cols ; i++) {
			if (mat.at<uchar>(j, i)) {
				++non_zero;
			}
		}
	}
	return non_zero;
}

void impose_depth(Mat colmat, Mat fdepth)
{
	int	x, y, cm;
	float	*fptr = (float*)fdepth.data;
	for (y = 0 ;  y < colmat.rows ; y++) {
		for (x = 0 ;  x < colmat.cols ; x++) {
			cm = depth2cm(*fptr++);
			if (cm >= maxcm || cm <= 0) continue;
			colmat.at<Vec3b>(y, x)[0] = blue[cm];
			colmat.at<Vec3b>(y, x)[1] = green[cm];
			colmat.at<Vec3b>(y, x)[2] = red[cm];
		}
	}
}

int main(int argc, char **argv)
{
	int		n, have_prev = 0;
	char		ch;
	unsigned	bit1 = 0;

	if (argc >= 3) {
		if (sscanf(argv[1], "%d", &near_cm) != 1)
			exit(1);
		if (sscanf(argv[2], "%d", &far_cm) != 1)
			exit(1);
	}
	printf("\nnear: %d, far: %d (cm)\n", near_cm, far_cm);	

	remove_ftdi_sio();
	sdk = (ISCSDKLib*)new ISCSDKLib();
	n = sdk->OpenISC();
	check_if_0("OpenISC", n);
	atexit(bye);
	maxcm = allocate_rgb_mem(1000, &red, &green, &blue);
	read_camera_param();
	copy_camera_param(&camera_param);
	get_size(&width, &height);

	n = sdk->SetShutterControlMode(true);	// auto gain/exposure
	check_if_0("SetShutterControlMode", n);

	n = sdk->SetAutoCalibration(1);		// auto mode
	check_if_0("SetAutoCalibration", n);

	n = sdk->StartGrab(2);	// 2:depth , 3:calibrated 4
	if (n < 0) exit(1) ;
	check_if_0("StartGrab", n);

	Mat lmat = Mat::zeros(height, width, CV_8UC1);
	Mat rmat = Mat::zeros(height, width, CV_8UC1);
	Mat colmat = Mat::zeros(height, width, CV_8UC3);
	Mat fdepth = Mat::zeros(height, width, CV_32F);
	#define HWIDTH 480
	#define VWIDTH 200	
	Rect	rect(rmat.cols/2-HWIDTH/2, rmat.rows/2-VWIDTH/2, HWIDTH, VWIDTH);
	Mat now = Mat::zeros(VWIDTH, HWIDTH, CV_8UC1);
	Mat prev = Mat::zeros(VWIDTH, HWIDTH, CV_8UC1);
	Mat dif = Mat::zeros(VWIDTH, HWIDTH, CV_8UC1);
#if 1
	namedWindow("original");
	moveWindow("original", colmat.cols+10, 0);
	namedWindow("color");
	moveWindow("color", 0, 0);
	namedWindow("xor");
	moveWindow("xor", colmat.cols+10+colmat.cols/2-HWIDTH/2, colmat.rows+36);
#endif
loop:
	do {
		n = sdk->GetImage((unsigned char*)lmat.data, (unsigned char*)rmat.data, 1); 
	} while (n != 0)
		;
	n = sdk->GetDepthInfo((float*)fdepth.data);
	if (n) {
		printf("GetDepthInfo failed\n");
		goto loop;
	}
	cvtColor(rmat, colmat, cv::COLOR_GRAY2RGB);
	impose_depth(colmat, fdepth);
	copy_depth(rmat, fdepth, rect);
	imshow("color", colmat);
	now = rmat(rect);
	if (have_prev) {
		bitwise_xor(now, prev, dif);
		bit1 = count_mat(dif);
	} else {
		have_prev = 1;
	}
	now.copyTo(prev);
	mat_printf(rmat, 4, 30, "%6d", bit1);
	imshow("color", colmat);
	imshow("original", rmat);
	imshow("xor", dif);
	ch = waitKey(30);
	if (ch == 0x1b) {
		goto end;
	}
	goto loop;
end:	
	exit(0);
}
