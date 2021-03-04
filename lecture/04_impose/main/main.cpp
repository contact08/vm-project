#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <ISCSDKLib.h>

using namespace cv;
using namespace std;

#include "libbasic.h"

ISCSDKLib	*sdk = NULL;
ISCSDKLib::CameraParamInfo	camera_param;
int	maxcm;
int	width, height;
uchar	*red, *green, *blue;

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

int main(int argc, char **argv)
{
	int	n;

	remove_ftdi_sio();
	sdk = (ISCSDKLib*)new ISCSDKLib();
	n = sdk->OpenISC();
	check_if_0("OpenISC", n);
	atexit(bye);

	maxcm = allocate_rgb_mem(1000, &red, &green, &blue);
	read_camera_param();
	get_size(&width, &height);

	n = sdk->SetShutterControlMode(true);	// auto gain/exposure mode
	check_if_0("SetShutterControlMode", n);
	n = sdk->SetAutoCalibration(1);		// auto mode
	check_if_0("SetAutoCalibration", n);

	n = sdk->StartGrab(2);	// 2:depth , 3:calibrated 4: original 
	check_if_0("StartGrab", n);
	Mat lmat = Mat::zeros(height, width, CV_8UC1);
	Mat rmat = Mat::zeros(height, width, CV_8UC1);
	Mat fdepth = Mat::zeros(height, width, CV_32F);

	// must wait!! to have 1st frame
	do {
		n = sdk->GetImage((unsigned char*)lmat.data, (unsigned char*)rmat.data, 1); 
	} while (n != 0)
		;
	n = sdk->GetDepthInfo((float*)fdepth.data);
	check_if_0("GetDepthInfo", n);

	Mat colmat = Mat::zeros(height, width, CV_8UC3);
	cvtColor(rmat, colmat, cv::COLOR_GRAY2RGB);
	impose_depth(colmat, fdepth);
#if 1
	namedWindow("original");
	namedWindow("color");
	moveWindow("original", 0, 0);
	moveWindow("color", rmat.cols+10, 0);
#endif
	imshow("original", rmat);
	imshow("color", colmat);
	waitKey(0);
	return 0;
}
