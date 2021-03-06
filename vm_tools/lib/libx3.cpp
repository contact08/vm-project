#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/fast_math.hpp>

using namespace cv;
using namespace std;

#include "libbasic.h"
#include "libx3.h"

static float	fpp2hangle;
static float	fpp2vangle;

// 'zero order hold' zooming
 void zoom(Mat from, Mat to)
{
	for (int j = 0 ; j < from.rows ; j++) {
		for (int i = 0 ; i < from.cols-1 ; i++) {
			// just a copy
			to.at<uchar>(j*2,i*2) = from.at<uchar>(j,i);
			// horizontal interpolation
			to.at<uchar>(j*2,i*2+1) =
			         (from.at<uchar>(j,i)
				+from.at<uchar>(j,i+1))/2;
			if (!j) continue;
			// vertical interpolation
			to.at<uchar>(j*2+1,i*2) =
			        (from.at<uchar>(j-1,i)
				+from.at<uchar>(j  ,i))/2;
		}
	}
}

// 'zero order hold' zooming
 void color_zoom(Mat from, Mat to)
{
	int	k;
	for (int j = 0 ; j < from.rows ; j++) {
		for (int i = 0 ; i < from.cols-1 ; i++) {
			// just a copy
			for (k = 0 ; k <  3 ; k++)
				to.at<Vec3b>(j*2,i*2)[k] = from.at<Vec3b>(j,i)[k];
			// horizontal interpolation
			for (k = 0 ; k <  3 ; k++)
				to.at<Vec3b>(j*2,i*2+1)[k] =
			        	 (from.at<Vec3b>(j,i)[k]
					+from.at<Vec3b>(j,i+1)[k])/2;
			if (!j) continue;
			// vertical interpolation
			for (k = 0 ; k <  3 ; k++)
				to.at<Vec3b>(j*2+1,i*2)[k] =
			        	(from.at<Vec3b>(j-1,i)[k]
					+from.at<Vec3b>(j  ,i)[k])/2;
		}
	}
}

void adj_contrast(Mat mat, double ratio)
{
	int	i, j;
	unsigned char	*ptr;
	int	npixel = 0, mean = 0;
	double	newc;
	ptr = mat.data;
	for (j = 0 ; j < mat.rows ; j++) {
		for (i = 0 ; i < mat.cols ; i++) {
			++npixel;
			mean += *ptr++; 
		}
	}
	mean = (unsigned)((double)mean/(double)npixel);
	ptr = mat.data;
	for (j = 0 ; j < mat.rows ; j++) {
		for (i = 0 ; i < mat.cols ; i++) {
			newc = (ratio*(double)(*ptr-mean)) + mean;
			if (newc > 255) newc = 255;
			if (newc < 0) newc = 0;
			*ptr++ = (unsigned char)newc;
		}
	}
}

void set_vmpp2_vangle(float vangle)
{
	fpp2vangle = vangle;
	fpp2hangle = get_vmpp2_hangle();
	printf("vangles:%f, hangle:%f\n", fpp2vangle, fpp2hangle);
}

float get_hangle(void)
{
	return fpp2hangle;
}

float get_vangle(void)
{
	return fpp2vangle;
}

void rotate_by_x(float *x, float *y, float *z, int degree)
{
	double	tmpy = *y, tmpz = *z;
	double	rad = D2R(degree);
	*y = (float)(tmpy*cos(rad) - tmpz*sin(rad));
	*z = (float)(tmpy*sin(rad) + tmpz*cos(rad));
}

void rotate_by_y(float *x, float *y, float *z, int degree)
{
	double	tmpx = *x, tmpz = *z;
	double	rad = D2R(degree);
	*x = (float)(tmpx*cos(rad) + tmpz*sin(rad));
	*z = (float)(tmpz*cos(rad) - tmpx*sin(rad));
}

void rotate_by_z(float *x, float *y, float *z, int degree)
{
	double	tmpx = *x, tmpy = *y;
	double	rad = D2R(degree);
	*x = (float)(tmpx*cos(rad) - tmpy*sin(rad));
	*y = (float)(tmpx*sin(rad) + tmpy*cos(rad));
}
