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
int	maxcm;
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

void bthresh(Mat gray, Scalar rect)
{
	int	x1 = rect.val[0];	// left	
	int	y1 = rect.val[1];	// upper	
	int	x2 = x1 + rect.val[2];	// right	
	int	y2 = y1 + rect.val[3];	// lower	
	int	npoint = 0;
	double	sum;
	uchar	n;

	// get mean
	for (int j = y1 ; j < y2 ; j++) {
		for (int i = x1 ; i < x2 ; i++) {
			sum += (double)gray.at<uchar>(j, i) ;
			++npoint;
		}
	}
	// digitize by mean
	n = (uchar)(cvRound(sum/(double)npoint));
	for (int j = y1 ; j < y2 ; j++) {
		for (int i = x1 ; i < x2 ; i++) {
			if (gray.at<uchar>(j, i) > n) {
				gray.at<uchar>(j, i) = 255;
			} else {
				gray.at<uchar>(j, i) = 0;
			}
		}
	}
}

int main(int argc, char **argv)
{
	int		n;
	char		ch;

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
#if 1
	//namedWindow("original");
	//moveWindow("original", 0, 0);
	namedWindow("color");
	moveWindow("color", 0, 0);
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
	#define HWIDTH 480
	#define VWIDTH 200	
	Scalar	rect(rmat.cols/2-HWIDTH/2, rmat.rows/2-VWIDTH/2, HWIDTH, VWIDTH);
	bthresh(rmat, rect);
	cvtColor(rmat, colmat, cv::COLOR_GRAY2RGB);
	impose_depth(colmat, fdepth);
	//imshow("original", rmat);
	imshow("color", colmat);
	ch = waitKey(30);
	if (ch == 0x1b) {
		goto end;
	}
	goto loop;
end:	
	exit(0);
}
