#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/fast_math.hpp>

using namespace cv;
using namespace std;

#include "libx3.h"

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
