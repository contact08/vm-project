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

void z_filter(Mat edge, Mat fdepth, int near_cm, int far_cm)
{
	int	x, y, cm;
	float	*fptr = (float*)fdepth.data;
	for (y = 0 ;  y < edge.rows ; y++) {
		for (x = 0 ;  x < edge.cols ; x++) {
			cm = depth2cm(*fptr++);
			if (cm >= maxcm || cm <= 0) {
				continue;
			}
			if (cm >= far_cm || cm < near_cm) {
				edge.at<uchar>(y, x) = 0;
				continue;
			}
		}
	}
}

void detect_line(Mat edge, Mat line_mat, unsigned nlines, double mindeg, int maxdeg)
{
	double	degree, slope;
#if 1	
	vector<Vec4i> linesP;
	HoughLinesP(edge, linesP, 1, CV_PI/180, 30, 200, 80);		// SAKUDAIRA
	cvtColor(edge, line_mat, COLOR_GRAY2BGR);
#if 0	
	for(size_t i = 0; i < linesP.size(); i++) {
        	Vec4i l = linesP[i];
        	line(line_mat, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,200), 1, LINE_AA);
	}
#endif
	if (nlines > linesP.size()) nlines = linesP.size();
	nlines = linesP.size();
	for(size_t i = 0; i < nlines ; i++) {
        	Vec4i l = linesP[i];
		slope = (double)(l[3]-l[1])/(double)(l[2]-l[0]);
		degree = R2D(atan(slope));
		//printf("%d: %3.2lf\n", i, degree);
		if (degree < mindeg || degree > maxdeg) continue;
        	line(line_mat, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,255,0), 1, LINE_AA);
	}
#else
	vector<Vec2f> lines;
	HoughLines(edge, lines, 1, CV_PI/180,  64, 100, 50);		// SAKUDAIRA
	cvtColor(edge, line_mat, COLOR_GRAY2BGR);
    	if (nlines > (int)lines.size()) nlines = lines.size();
    for(int i = 0; i < nlines ; i++) {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line(line_mat, pt1, pt2, Scalar(0,0,255), 1, LINE_AA);
    }
#endif	
}

void put_axes(Mat mat, int x, int y)
{
	line(mat, Point(x, y), Point(x+32, y),Scalar(0,255,255),1);
	line(mat, Point(x, y), Point(x, y+32),Scalar(0,255,255),1);
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
	Mat edge = Mat::zeros(height, width, CV_8UC1);
	Mat line_mat = Mat::zeros(height, width, CV_8UC3);
	Mat colmat = Mat::zeros(height, width, CV_8UC3);
	Mat fdepth = Mat::zeros(height, width, CV_32F);
#if 0
	namedWindow("color");
	moveWindow("color", 0, 0);
#endif
	near_cm = 350;
	far_cm = 400;
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
	npoints = 0;

	cvtColor(colmat, edge, cv::COLOR_RGB2GRAY);
	Canny(edge, edge, 48, 96);
	imshow("original", rmat);
	imshow("edge", edge);
	z_filter(edge, fdepth, near_cm, far_cm);
	detect_line(edge, line_mat, 10, 10.0, 14.0);
	mat_printf(line_mat, 8, 20, "npoints:%u", npoints);
	mat_printf(line_mat, 8, 40, "near:%4u far:%4u", near_cm, far_cm);
	put_axes(line_mat, 4, 4);
	imshow("lines", line_mat);
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
