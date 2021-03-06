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

#define EXTRN
#include "global.h"

int cmd_proc(char *cmd);
static unsigned	*count_cm;

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
	memset(count_cm, 0, maxcm*sizeof(float));
	for (y = 0 ;  y < colmat.rows ; y++) {
		for (x = 0 ;  x < colmat.cols ; x++) {
			cm = depth2cm(*fptr++);
			if (cm >= maxcm || cm <= 0) continue;
			colmat.at<Vec3b>(y, x)[0] = blue[cm];
			colmat.at<Vec3b>(y, x)[1] = green[cm];
			colmat.at<Vec3b>(y, x)[2] = red[cm];
			++npoints;
			++count_cm[cm];
		}
	}
	return npoints;
}

void show_z_hist(Mat hplot)
{
	unsigned	max = 0;
	int		i, d;
	int		ncm = maxcm;
	hplot = 0;
	for (i = 0 ; i <  ncm ; i++) { 
		if (count_cm[i] > max) max = count_cm[i];
	}
	for (i = 0 ; i <  ncm ; i++) {
		d = (int)((double)(count_cm[i]*hplot.rows)/(double)max);
		line(hplot,
		     Point(i, hplot.rows),
		     Point(i, hplot.rows-d),
		     Scalar(128, 128, 0),
		     1);
	}
	draw_grid(hplot, 7, 2, 200);
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

	maxcm = allocate_rgb_mem(700, &red, &green, &blue);
	if ((count_cm = (unsigned int*)malloc(maxcm*sizeof(unsigned))) == NULL) {
		printf("failed to allocate count_cm[]\n");
		exit(1);
	}
	read_camera_param();
	get_size(&width, &height);

	n = sdk->SetShutterControlMode(2);	// double shutter
	check_if_0("SetShutterControlMode", n);

	n = sdk->SetAutoCalibration(1);		// auto mode
	check_if_0("SetAutoCalibration", n);

	n = sdk->StartGrab(2);	// 2:depth , 3:calibrated 4: original 
	check_if_0("StartGrab", n);

	Mat lmat = Mat::zeros(height, width, CV_8UC1);
	Mat rmat = Mat::zeros(height, width, CV_8UC1);
	Mat colmat = Mat::zeros(height, width, CV_8UC3);
	Mat fdepth = Mat::zeros(height, width, CV_32F);
	Mat	hplot(480, maxcm, CV_8UC3, Scalar(0,0,0));
#if 1
	namedWindow("color");
	namedWindow("histgram");
	moveWindow("color", 0, 0);
	moveWindow("histgram", rmat.cols+10, 0);
#endif
loop:
	do {
		n = sdk->GetImage((unsigned char*)lmat.data, (unsigned char*)rmat.data, 1); 
		if (n && (n != -102)) printf("GetImage: %d\n", n);
	} while (n != 0)
		;
	n = sdk->GetDepthInfo((float*)fdepth.data);
	if (n) {
		printf("GetDepthInfo failed\n");
		goto loop;
	}
	cvtColor(rmat, colmat, cv::COLOR_GRAY2RGB);
	npoints = impose_depth(colmat, fdepth);
	//imshow("original", rmat);

	n = sdk->GetNoiseFilter(&noisethresh);
	check_if_0_silent("GetNoiseFilter", n);

	imshow("color", colmat);
	show_z_hist(hplot);
	mat_printf(hplot, 4, 20, "mode:%d, exp:%d, npoints:%u", get_exp_mode(), get_expval(), npoints);
	mat_printf(hplot, 4, 40, "noise filter:%d", noisethresh);
	imshow("histgram", hplot);
	cmd[0] = ch = waitKey(30);
	cmd[1] = '\0';
	if (ch == 0x1b) goto end;
	if ((ch > ' ') && (ch <= 'z')) {
		cmd_proc(cmd);
	}
	goto loop;
end:	
	return 0;
}
