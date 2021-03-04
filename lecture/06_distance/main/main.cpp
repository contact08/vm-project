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
uchar	*red, *green, *blue;
int	maxcm, width, height;
Point	p0;
Mat fdepth;
Mat status;

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

void marker(Mat mat, int x, int y, Scalar col)
{
	int	size = 5;
	rectangle(mat,
		Point(x-size, y-size),	
		Point(x+size, y+size),	
		col,
		1);
	line(mat,Point(x-size, y-size),
		 Point(x+size, y+size),	
		 col,
		 1,
		 LINE_AA);
	line(mat,Point(x-size, y+size),
		 Point(x+size, y-size),	
		 col,
		 1,
		 LINE_AA);
}

void update_status(int x, int y)
{
	int	i, j, cm;
	status = 0;
	mat_printf(status, 0, 20, "x: %3d y: %3d", x, y);
	for (j = 0 ; j < 5 ; j++) {
		for (i = 0 ; i < 5  ; i++) {
			cm = depth2cm(fdepth.at<float>(y-(2+j), x-(2+i))); 
			if (cm)
				mat_printf(status, i*60+20, j*20+60, "%3d", cm);
			else
				mat_printf(status, i*60+20, j*20+60, "  .", cm);
		}
	}
	imshow("status", status);
}

void callback(int event, int x, int y, int flags, void* userdata)
{
	if  (event == EVENT_LBUTTONDOWN ) {
		p0.x = x;		
		p0.y = y;		
	} else if  (event == EVENT_LBUTTONUP) {
		;
	} else if  (event == EVENT_RBUTTONDOWN ) {
        	;
	} else if  (event == EVENT_MBUTTONDOWN ) {
        	;
	} else if (event == EVENT_MOUSEMOVE ) {
		;
	}
}

void init_mouse(Mat mat)
{
	p0.x = (mat.cols)/2;
	p0.y = (mat.rows)/2;
}

int main(int argc, char **argv)
{
	int		n, mode = 1;
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
	status.create(height, width, CV_8UC3);
#if 1
	namedWindow("color");
	namedWindow("status");
	moveWindow("color", 0, 0);
	moveWindow("status", rmat.cols+10, 0);
#endif
	init_mouse(colmat);
	setMouseCallback("color", callback, NULL);
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
	update_status(p0.x, p0.y);
	marker(colmat, p0.x, p0.y, Scalar(255,255,0));
	imshow("color", colmat);
	imshow("status", status);
	ch = waitKey(200);
	if (ch == 0x1b) {
		goto end;
	} else if (ch == 'm') {
		mode ^= 1;
	}
	goto loop;
end:	
	return 0;
}
