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
#include "commands.h"

#define EXTRN
#include "global.h"

int cmd_proc(char *cmd);

ISCSDKLib	*sdk = NULL;

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
	if (arbox[0] != NULL) destroy_arbox(arbox[0]);
	if (arbox[1] != NULL) destroy_arbox(arbox[1]);
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

unsigned impose_depth(Mat colmat, Mat fdepth)
{
	int	x, y, cm;
	unsigned	npoints = 0;
	float	*fptr = (float*)fdepth.data;
	//memset(count_cm, 0, maxcm*sizeof(float));
	for (y = 0 ;  y < colmat.rows ; y++) {
		for (x = 0 ;  x < colmat.cols ; x++) {
			cm = depth2cm(*fptr++);
			if (cm >= maxcm || cm <= 0) continue;
			colmat.at<Vec3b>(y, x)[0] = blue[cm];
			colmat.at<Vec3b>(y, x)[1] = green[cm];
			colmat.at<Vec3b>(y, x)[2] = red[cm];
			++npoints;
			//++count_cm[cm];
		}
	}
	return npoints;
}

void gen_mask_mat(Mat mask, ARBOX *ptr, Mat original)
{
	int	from, to, yes;
	ptr->color = Scalar(255,255,255);
	mask = 0;
	write_arbox(mask, ptr);
	for (int j = 0 ; j < mask.rows ; j++) {
		yes = 0;
		for (from = 0 ; from < mask.cols ; from++)
			if (mask.at<uchar>(j, from)) {
				++yes;
				break;
			}
		for (to = mask.cols ; to >= 0 ; to--)
			if (mask.at<uchar>(j, to)) {
				++yes;
				break;
			}
		if (yes == 2) {
			for (int i = from ; i <= to ; i++) {
				mask.at<uchar>(j, i) = original.at<uchar>(j, i);
			}
		}
	}
	Canny(mask, mask, 48, 96);
	//erase_arbox(mask, ptr);
}

int main(int argc, char **argv)
{
	int	n;
	char	ch, cmd[2];
	unsigned	npoints;

	remove_ftdi_sio();
	sdk = (ISCSDKLib*)new ISCSDKLib();
	n = sdk->OpenISC();
	check_if_0("OpenISC", n);
	atexit(bye);
	maxcm = allocate_rgb_mem(700, &red, &green, &blue);

	read_camera_param();
	maxcm = 1000;
	get_size(&width, &height);
	//degree = get_vmpp2_hangle();		// estimated value from parameter
	set_vmpp2_vangle((float)20.231);	// measured by "angle_of_view"
	printf("width: %d, height: %d\n", width, height);

	n = sdk->SetShutterControlMode(2);	// double shutter
	check_if_0("SetShutterControlMode", n);

	n = sdk->SetAutoCalibration(1);		// auto mode
	check_if_0("SetAutoCalibration", n);

	n = sdk->StartGrab(2);	// 2:depth , 3:calibrated 4: original 
	check_if_0("StartGrab", n);

	lmat.create(height, width, CV_8UC1);
	rmat.create(height, width, CV_8UC1);
	mask.create(height, width, CV_8UC1);
	colmat.create(height, width, CV_8UC3);
	fdepth.create(height, width, CV_32F);

	arbox[0] = new_arbox();
	arbox[1] = new_arbox();
	set_arbox(arbox[0], Scalar(-0.6, 3.4, 1.2, 1.5), 0.2, 0.1);
	set_arbox(arbox[1], Scalar(-0.7, 3.6, 1.4, 1.5), 0.4, 0.1);
	arbox[1]->color = Scalar(255,0,0);
	adj_g("");

#if 0
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
	if (impose) {
		npoints = impose_depth(colmat, fdepth);
	}
	mat_printf(colmat, 4, 20, "rolling: %d, pitching: %d ",
		rolling_deg, pitching_deg);

	set_rolling(rolling_deg);
	set_pitching(pitching_deg);
	write_arbox(colmat, arbox[index_arbox]);

	gen_mask_mat(mask, arbox[index_arbox], rmat);
	if (disp_mode == 0) {
		imshow("image", colmat);
	} else {
		imshow("image", mask);
	}
	cmd[0] = ch = waitKey(30);
	cmd[1] = '\0';
	cmd_proc(cmd);
	if (ch == 0x1b) {
		goto end;
	}
	goto loop;
end:	
	return 0;
}
