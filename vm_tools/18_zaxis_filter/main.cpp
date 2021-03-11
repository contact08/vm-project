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

typedef struct {
	int	cols;
	int	rows;
	uchar	*z_count;
} ZCOUNT;

static uchar *allocate_zcount_map(Mat mat)
{
	uchar	*ptr = (uchar*)calloc((mat.cols*mat.rows)/8, 1);
	return ptr;
}

static int read_zcount_map(ZCOUNT *zc, int x, int y)
{
	uchar	*ptr = zc->z_count + zc->cols*y + x;
	return *ptr;
}

static void add_zcount_map(ZCOUNT *zc, int x, int y)
{
	uchar	*ptr = zc->z_count + zc->cols*y + x;
	++*ptr;
}

unsigned impose_with_z_filter(Mat colmat, Mat fdepth, int near_cm, int far_cm)
{
	int	x, y, cm;
	unsigned	npoints = 0;
	float	*fptr = (float*)fdepth.data;
	uchar	*z_count = allocate_zcount_map(colmat);
	ZCOUNT	zc;

	zc.rows = colmat.rows;
	zc.cols = colmat.cols;
	zc.z_count = z_count;

	if (z_count == NULL) {
		printf("failed to allocate z_count\n");
		exit(1);
	}
	memset(count_cm, 0, maxcm*sizeof(float));
	for (y = 0 ;  y < colmat.rows ; y++) {
		for (x = 0 ;  x < colmat.cols ; x++) {
			cm = depth2cm(*fptr++);
			if (cm >= maxcm || cm <= 0) {
				colmat.at<Vec3b>(y, x)[0] = 0;
				colmat.at<Vec3b>(y, x)[1] = 0;
				colmat.at<Vec3b>(y, x)[2] = 0;
				continue;
			}
			if (cm >= far_cm || cm < near_cm) {
				colmat.at<Vec3b>(y, x)[0] = 0;
				colmat.at<Vec3b>(y, x)[1] = 0;
				colmat.at<Vec3b>(y, x)[2] = 0;
				continue;
			}
			++npoints;
			++count_cm[cm];
			add_zcount_map(&zc, x>>3, y>>3);
		}
	}
	return npoints;
#if 0	
	fptr = (float*)fdepth.data;
	for (y = 0 ;  y < colmat.rows ; y++) {
		for (x = 0 ;  x < colmat.cols ; x++) {
			cm = depth2cm(*fptr++);
			if (cm >= maxcm || cm <= 0) {
				colmat.at<Vec3b>(y, x)[0] = 0;
				colmat.at<Vec3b>(y, x)[1] = 0;
				colmat.at<Vec3b>(y, x)[2] = 0;
				continue;
			}
			int n = read_zcount_map(&zc, x>>3, y>>3);
			if (n < 25 || n > 80) {
				colmat.at<Vec3b>(y, x)[0] = 0;
				colmat.at<Vec3b>(y, x)[1] = 0;
				colmat.at<Vec3b>(y, x)[2] = 0;
				continue;
			}
			//colmat.at<Vec3b>(y, x)[0] = blue[cm];
			//colmat.at<Vec3b>(y, x)[1] = green[cm];
			//colmat.at<Vec3b>(y, x)[2] = red[cm];
		}
	}
#else
	for (y = 0 ;  y < colmat.rows ; y+=8) {
		for (x = 0 ;  x < colmat.cols ; x+=8) {
			int n = read_zcount_map(&zc, x>>3, y>>3);
			if (n < 25 || n > 80) {
				for (int j = y ; j < y+8 ; j++) {
					for (int i = x ; i < x+8 ; i++) {
						colmat.at<Vec3b>(j, i)[0] = 0;
						colmat.at<Vec3b>(j, i)[1] = 0;
						colmat.at<Vec3b>(j, i)[2] = 0;
					}
				}
			}
		}
	}
#endif	
	free(z_count);
	return npoints;
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

void detect_line(Mat edge, Mat line_mat, unsigned nlines)
{
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
	for(size_t i = 0; i < nlines ; i++) {
        	Vec4i l = linesP[i];
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
	//npoints = impose_with_z_filter(colmat, fdepth, near_cm, far_cm);
	npoints = 0;

	cvtColor(colmat, edge, cv::COLOR_RGB2GRAY);
	Canny(edge, edge, 64, 200);
	imshow("original", rmat);
	imshow("edge", edge);
	z_filter(edge, fdepth, near_cm, far_cm);
	detect_line(edge, line_mat, 10);
	mat_printf(line_mat, 4, 20, "npoints:%u", npoints);
	mat_printf(line_mat, 4, 40, "near:%4u far:%4u", near_cm, far_cm);
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
