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
Point	p0;
Mat fdepth;
VideoWriter video;

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

int open_avi(string path, int fps, int width, int height)
{
	video.open(path,VideoWriter::fourcc('M','J','P','G'),fps, Size(width,height)); 
	if (!video.isOpened()) {
		return -1;
	}
	return 0;
}

const char *help =

" m   | toggle impose mode\n"
" v   | start/stop avi recording\n"
" ?   | show this help message\n"
" ESC | exit application\n";

int main(int argc, char **argv)
{
	int		n, mode = 1, recording = 0;
	char		ch;

	remove_ftdi_sio();
	sdk = (ISCSDKLib*)new ISCSDKLib();
	n = sdk->OpenISC();
	check_if_0("OpenISC", n);
	atexit(bye);

	maxcm = allocate_rgb_mem(1000, &red, &green, &blue);
	read_camera_param();
	get_size(&width, &height);

	n = sdk->SetShutterControlMode(true);	// auto gain/exposure
	check_if_0("SetShutterControlMode", n);

	n = sdk->SetAutoCalibration(1);		// auto mode
	check_if_0("SetAutoCalibration", n);

	n = sdk->StartGrab(2);	// 2:depth , 3:calibrated 4: original 
	check_if_0("StartGrab", n);

	Mat lmat = Mat::zeros(height, width, CV_8UC1);
	Mat rmat = Mat::zeros(height, width, CV_8UC1);
	Mat colmat = Mat::zeros(height, width, CV_8UC3);
	fdepth.create(height, width, CV_32F);
#if 1
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
	cvtColor(rmat, colmat, cv::COLOR_GRAY2RGB);
	if (mode) impose_depth(colmat, fdepth);
	if (recording) {
		video.write(colmat);
		circle(colmat, Point(16,16), 10, Scalar(0,0,255), -1);
	}
	imshow("color", colmat);
	ch = waitKey(30);
	if (ch == 0x1b) {
		goto end;
	} else if (ch == 'm') {
		mode ^= 1;
	} else if (ch == 'v') {
		recording ^= 1;
		if (recording) {
			string filename = "./video.avi";
			if (open_avi(filename, 30, colmat.cols, colmat.rows) < 0) {
				recording ^= 1;
				cout << "failed to openn " << filename << endl;
			}
		} else {
			video.release();
		}
	} else if (ch == '?' || ch == 'h') {
		printf("%s", help);
	}
	goto loop;
end:	
	if (recording) video.release();
	return 0;
}
