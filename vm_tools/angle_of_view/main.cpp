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
#include "libx3.h"

#define EXTRN
#include "global.h"

#define ZOOMW 50

int cmd_proc(char *cmd);

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

int get_zoom_width(void)
{
	return ZOOMW;
}

int main(int argc, char **argv)
{
	char	ch, cmd[2];
	int	n;

	ptr_cm = &dcm;

	remove_ftdi_sio();
	sdk = (ISCSDKLib*)new ISCSDKLib();
	n = sdk->OpenISC();
	check_if_0("OpenISC", n);
	atexit(bye);

	read_camera_param();
	get_size(&width, &height);
	maxcm = 1000;
	n = sdk->SetShutterControlMode(2);	// double shutter mode
	check_if_0("SetShutterControlMode", n);

	n = sdk->SetAutoCalibration(1);		// auto mode
	check_if_0("SetAutoCalibration", n);

	n = sdk->StartGrab(2);	// 2:depth , 3:calibrated 4: original 
	check_if_0("StartGrab", n);

	lmat.create(height, width, CV_8UC1);
	rmat.create(height, width, CV_8UC1);
	colmat.create(height, width, CV_8UC3);
	Mat roi_mat;
	Mat zoom_mat = Mat::zeros(ZOOMW*4, ZOOMW*4, CV_8UC3);

	x[0] = x[1] = colmat.cols/2;
	y[0] = colmat.rows/2 - 20;
	y[1] = colmat.rows/2 + 20;
#if 1
	namedWindow("color");
	moveWindow("color", 0, 0);
	namedWindow("zoom");
	moveWindow("zoom", width+10, 0);
#endif
loop:
	do {
		n = sdk->GetImage((unsigned char*)lmat.data, (unsigned char*)rmat.data, 1); 
	} while (n != 0)
		;
	cvtColor(rmat, colmat, cv::COLOR_GRAY2RGB);
	rectangle(colmat, Point(0, 0), Point(100, 20), Scalar(0, 0, 0), -1);
	put_marker(colmat, x[0], y[0], Scalar(0,0,255));
	put_marker(colmat, x[1], y[1], Scalar(0,255,255));
	rectangle(colmat, Point(0, 0), Point(lmat.cols/2, 90), Scalar(0,0,0), -1);
	mat_printf(colmat, 4, 20, "distance:%4dcm", dcm);
	mat_printf(colmat, 4, 40, "horz.: %dcm, vert.: %dcm", hcm, vcm);
	hview = (double)(hcm*width)/(double)(iabs_sub(x[0],x[1]));
	vview = (double)(vcm*height)/(double)(iabs_sub(y[0],y[1]));
	vdeg = 2.0*R2D(atan((vview/2.0)/(double)dcm));
	hdeg = 2.0*R2D(atan((hview/2.0)/(double)dcm));
	mat_printf(colmat, 4, 60, "V: %dcm, %3.3fdeg", (int)vview, vdeg);
	mat_printf(colmat, 4, 80, "H: %dcm, %3.3fdeg", (int)hview, hdeg);

	roi_mat = rmat(Rect(x[index_xy]-ZOOMW,y[index_xy]-ZOOMW,ZOOMW*2,ZOOMW*2));
	cvtColor(roi_mat, roi_mat, cv::COLOR_GRAY2RGB);
	color_zoom(roi_mat, zoom_mat);
	adj_contrast(zoom_mat, 1.2);
	line(zoom_mat, Point(0, 0), Point(zoom_mat.cols, zoom_mat.rows), Scalar(0,0,255), 2);
	line(zoom_mat, Point(0, zoom_mat.rows), Point(zoom_mat.cols, 0), Scalar(0,0,255), 2);

	rectangle(colmat, Point(ZOOMW, ZOOMW), Point(width-ZOOMW, height-ZOOMW), Scalar(0,0,255), 1);
	imshow("color", colmat);
	imshow("zoom", zoom_mat);
	cmd[0] = ch = waitKey(30);
	cmd[1] = '\0';
	if (ch == 0x1b) goto end;
	cmd_proc(cmd);	
	goto loop;
end:	
	return 0;
}
