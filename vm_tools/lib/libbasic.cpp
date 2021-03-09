#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <linux/types.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <ISCSDKLib.h>
#include "libbasic.h"

using namespace cv;

extern ISCSDKLib	*sdk;

static int	width, height;
static float	fD_INF;
static float	fBF;   //[pixel*m]
static float	B;     // should be 10cm for VM2020
static float	fViewAngle;
static ISCSDKLib::CameraParamInfo	camera_param;
static float	fpp2hangle;	// added for pp2
static float	fpp2vangle;	// added for pp2

void remove_ftdi_sio(void)
{
	FILE	*pf = fopen("/proc/modules", "r");
	if (pf) {
		int detect = 0;
		char line[512];
		while(fgets(line, 512, pf)){
			if(strncmp(line,"ftdi_sio",8)==0){
				detect = 1;
				break;	
			}
		}
		fclose(pf);
		if (detect) {
			if(system("sudo rmmod ftdi_sio") <= 0) {
				printf("warning: can't remove ftdi_sio\n");
			}
		}
	}
}

void check_if_0(const char *title, int n)
{
	if (n) {
		printf("%s: error(%d)\n", title, n);
		exit(1);
	}  else {
		printf("%s: ok\n", title);
	}
}

void check_if_0_silent(const char *title, int n)
{
	if (n) {
		printf("%s: error(%d)\n", title, n);
		exit(1);
	}
}

void draw_grid(Mat mat, int xdiv, int ydiv, int value)
{
	int	i, pos;
	for (i = 0 ; i < xdiv ; i++) {
		pos = cvRound((double)(i*mat.cols)/(double)xdiv);
		line(mat,Point(pos,0),Point(pos,mat.rows), Scalar(value,value,value),1,LINE_AA);
	}
	for (i = 0 ; i < ydiv ; i++) {
		pos = cvRound((double)(i*mat.rows)/(double)ydiv);
		line(mat,Point(0,pos),Point(mat.cols,pos), Scalar(value,value,value),1,LINE_AA);
	}
}

static Scalar col_printf = Scalar(255, 255, 255);
static double size_printf = 1.2;

void mat_printf_color(Scalar col)
{
	col_printf = col;
}

void mat_printf_size(double n)
{
	size_printf = n;
}

void mat_printf(Mat mat, int x, int y, const char *fmt, ...) 
{
	int n;
	char line[1024];
	va_list ap;
	va_start(ap, fmt);
	n = vsnprintf(line, sizeof(line), fmt, ap);
	va_end(ap);
	if (n < 0) return;
       	putText(mat,			// Mat
	       	line,			// string
		Point(x,y),		// position
		FONT_HERSHEY_PLAIN,	// font
		size_printf,		// font scale
       		col_printf,		// color
		1);			// thicklness
}

void get_size(int *pwidth, int *pheight)
{
	*pwidth = width;
	*pheight = height;
}

float get_view_angle(void)
{
	return fViewAngle;
}

void dump_mem(void *ptr, int n);

void copy_camera_param(void *ptr)
{
	memcpy(ptr, &camera_param, sizeof(camera_param));
}

int read_camera_param(void)
{
	int n = sdk->GetCameraParamInfo(&camera_param);
	check_if_0("GetCameraParamInfo", n);
	width = camera_param.nImageWidth;
	height = camera_param.nImageHeight;
	fD_INF = camera_param.fD_INF;
	fBF = camera_param.fBF;
	B = camera_param.fBaseLength;
	fViewAngle = camera_param.fViewAngle;
	printf("\n\n");
	printf("image size: %dx%d\n", width, height);
	printf("fD_INF: %f\n", fD_INF);
	printf("fBF: %f\n", fBF);
	printf("fBaseLength: %f\n", B);
	printf("fViewAngle: %f\n", fViewAngle);
	printf("nProductNumber: %d\n", camera_param.nProductNumber);
	printf("nSerialNumber: %s\n", camera_param.nSerialNumber);
	//printf("nFPGA_Version: %d\n", camera_param.nFPGA_Version);
	get_vmpp2_hangle();
	printf("\n\n");
	return 0;
}

float depth2distance(float depth)
{
	float	z;
	if (fD_INF >= depth) return (float)0.0; // invalid depth
        z = fBF / (depth - fD_INF);
        return (float)(z);
}

int depth2cm(float depth)
{
	float	z;
	if (fD_INF >= depth) return (int)0; // invalid depth
        z = cvRound((fBF*100.0)/(depth-fD_INF));
        return (int)z;
}

static void init_rgb_tbl(int min_cm, int max_cm, uchar *redp, uchar *greenp, uchar *bluep)
{
	int	i, cm, step = (int)(((double)max_cm-(double)min_cm)/(double)6.0);
	double	slope = 255.0/(double)step;
	cm = min_cm;
	for (i = 0 ; i < step ; cm++, i++) {
		redp[cm] = 255;
		greenp[cm] = (uchar)(slope*i);
		bluep[cm] = 0;
	}
	for (i = 0 ; i < step ; cm++, i++) {
		redp[cm] = 255-(uchar)(slope*i);
		greenp[cm] = 255;
		bluep[cm] = 0;
	}
	for (i = 0 ; i < step ; cm++, i++) {
		redp[cm] = 0;
		greenp[cm] = 255;
		bluep[cm] = (uchar)(slope*i);
	}
	for (i = 0 ; i < step ; cm++, i++) {
		redp[cm] = 0;
		greenp[cm] = 255-(uchar)(slope*i);
		bluep[cm] = 255;
	}
	for (i = 0 ; i < step ; cm++, i++) {
		redp[cm] = (uchar)(slope*i);
		greenp[cm] = 0;
		bluep[cm] = 255;
	}
	for (i = 0 ; cm < max_cm ; cm++, i++) {
		redp[cm] = 255;
		greenp[cm] = 0;
		bluep[cm] = 255-(uchar)(slope*i);
	}

	bluep[0] = greenp[0] = redp[0] = 0;
#if 0
	for (i = 0 ; i < max_cm ; i++) {
		printf("%d, %d, %d, %d\n", i, redp[i], greenp[i], bluep[i]);
	}
	printf("\n%d, %e\n", step, slope);
	printf("step: %d\n", step);
	printf("slope: %e\n", slope);
#endif	
}

int allocate_rgb_mem(int cm, uchar **red, uchar **green, uchar **blue)
{
	if ((*red = (uchar*)malloc(cm)) == NULL) {
		printf("failed to allocate red\n");
		exit(1);
	}
	if ((*green = (uchar*)malloc(cm)) == NULL) {
		printf("failed to allocate green\n");
		exit(1);
	}
	if ((*blue = (uchar*)malloc(cm)) == NULL) {
		printf("failed to allocate blue\n");
		exit(1);
	}
	init_rgb_tbl(0, cm, *red, *green, *blue);
	return cm;
}

void dump_mem(void *ptr, int n)
{
	for (int i = 0 ; i < n ; i++) {
		if (!(i &15)) printf("\n%p: ", (uchar*)ptr+i);
		printf("%2x ", *((uchar*)ptr+i) & 255);
	}
	printf("\n");
}

float get_vmpp2_hangle(void)
{
	float	newangle, oldview, newview;
	#define OLDW ((double)640)	// pp1
	#define NEWW ((double)752)	// pp2
	oldview = tan(D2R(fViewAngle/2.0));
	newview = oldview * NEWW/OLDW;
	newangle = R2D(atan(newview)) * 2.0;
	printf("pp2 hangle: %f\n", newangle);
	fpp2hangle = newangle;
	return newangle;
}

