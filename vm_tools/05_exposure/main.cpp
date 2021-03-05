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
#include "commands.h"
int cmd_proc(char *cmd);

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

unsigned impose_depth(Mat colmat, Mat fdepth)
{
	int	x, y, cm;
	unsigned	npoints = 0;
	float	*fptr = (float*)fdepth.data;
	for (y = 0 ;  y < colmat.rows ; y++) {
		for (x = 0 ;  x < colmat.cols ; x++) {
			cm = depth2cm(*fptr++);
			if (cm >= maxcm || cm <= 0) continue;
			colmat.at<Vec3b>(y, x)[0] = blue[cm];
			colmat.at<Vec3b>(y, x)[1] = green[cm];
			colmat.at<Vec3b>(y, x)[2] = red[cm];
			++npoints;
		}
	}
	return npoints;
}

void show_hist(Mat mat, Mat hplot)
{
	unsigned	max = 0, hist[256];
	int		i, j, d;
	int		w = hplot.cols/256;
	memset(hist, 0, sizeof(hist));
	hplot = 0;
	for (j = 0 ; j < mat.rows ; j++) {
		for (i = 0 ; i < mat.cols ; i++) {
			++hist[mat.at<uchar>(j, i)];
		}
	}
	for (i = 0 ; i <  256 ; i++) { 
		if (hist[i] > max) max = hist[i];
	}
	for (i = 0 ; i <  256 ; i++) {
		d = (int)((double)(hist[i]*hplot.rows)/(double)max);
		rectangle(hplot,
			     Point(w*i, hplot.rows),
			     Point(w*(i+i), hplot.rows-d),
			     Scalar(128, 128, 0),
			     -1);
	}
	draw_grid(hplot, 4, 4, 200);
}

unsigned set_exposure(unsigned n)
{
	int	ret, retry = 0;
	unsigned	val = n;
	do {
		ret = sdk->SetExposureValue(n);
		if (ret) {
			if (waitKey(10) == 0x1b)
				exit(1);
		}
	} while(ret && (++retry < 3))
       		;
	if (ret) {
		ret = sdk->GetExposureValue(&val);
		check_if_0("GetExposureValue", ret);
	}
	return val;
}

int main(int argc, char **argv)
{
	int		n;
        unsigned	npoints;
	char		ch, cmd[2];

	remove_ftdi_sio();
	sdk = (ISCSDKLib*)new ISCSDKLib();
	n = sdk->OpenISC();
	check_if_0("OpenISC", n);
	atexit(bye);

	maxcm = allocate_rgb_mem(1000, &red, &green, &blue);
	read_camera_param();
	get_size(&width, &height);

	n = sdk->SetShutterControlMode(1);	// single gain/exposure
	check_if_0("SetShutterControlMode", n);

	n = sdk->SetAutoCalibration(1);		// auto mode
	check_if_0("SetAutoCalibration", n);

	n = sdk->StartGrab(2);	// 2:depth , 3:calibrated 4: original 
	check_if_0("StartGrab", n);

	Mat lmat = Mat::zeros(height, width, CV_8UC1);
	Mat rmat = Mat::zeros(height, width, CV_8UC1);
	Mat colmat = Mat::zeros(height, width, CV_8UC3);
	Mat fdepth = Mat::zeros(height, width, CV_32F);
	#define CELLW 2
	Mat	hplot(480, (256+1)*CELLW, CV_8UC3, Scalar(0,0,0));
#if 1
	namedWindow("original");
	namedWindow("color");
	namedWindow("histgram");
	moveWindow("original", 0, 0);
	moveWindow("color", 0, rmat.rows+36);
	moveWindow("histgram", rmat.cols+10, 0);
#endif
loop:
	//do {
		n = sdk->GetImage((unsigned char*)lmat.data, (unsigned char*)rmat.data, 1); 
	//} while (n != 0)
		;
	n = sdk->GetDepthInfo((float*)fdepth.data);
	if (n) {
		printf("GetDepthInfo failed\n");
		goto loop;
	}
	cvtColor(rmat, colmat, cv::COLOR_GRAY2RGB);
	npoints = impose_depth(colmat, fdepth);
	imshow("original", rmat);
	imshow("color", colmat);
	show_hist(rmat, hplot);
	mat_printf(hplot, 4, 20, "mode:%d, exp:%d, npoints:%u", get_exp_mode(), get_expval(), npoints);
	imshow("histgram", hplot);
	ch = waitKey(30);
	if (ch == 0x1b) {
		goto end;
	}
#if 0       
	else if (ch == '+') {
		if ((expval += 10) > 480)
			expval = 479;
		expval = set_exposure(expval);
	} else if (ch == '-') {
		if ((expval -= 10) < 1)
			expval = 2;
		expval = set_exposure(expval);
	}
#endif
	if ((ch > ' ') && (ch <= 'z')) {
		cmd[0] = ch;
		cmd[1] = '\0';
		cmd_proc(cmd);
	}
	goto loop;
end:	
	return 0;
}
