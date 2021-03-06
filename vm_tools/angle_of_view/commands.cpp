#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/fast_math.hpp>
#include <ISCSDKLib.h>
#include "libbasic.h"
#include "libx3.h"

#define EXTRN extern
#include "global.h"

int get_zoom_width(void);

int  select_pt0(char *line)
{
	index_xy = 0;
	return 0;
}

int  select_pt1(char *line)
{
	index_xy = 1;
	return 0;
}

int  adj_dist(char *line)
{
	ptr_cm = &dcm;	
	return 0;
}

int  mv_up(char *line)
{
	if (--y[index_xy] <= get_zoom_width())
		y[index_xy] = height-get_zoom_width();
	return 0;
}

int  mv_down(char *line)
{
	if (++y[index_xy] > (height-get_zoom_width()))
		y[index_xy] = get_zoom_width();
	return 0;
}

int  mv_left(char *line)
{
	if (--x[index_xy] < get_zoom_width())
		x[index_xy] = width-get_zoom_width();
	return 0;
}

int  mv_right(char *line)
{
	if (++x[index_xy] > width-get_zoom_width())
	       x[index_xy] = get_zoom_width();
	return 0;
}

int  inc_var(char *line)
{
	if (*ptr_cm < maxcm) ++*ptr_cm;
	return 0;
}

int  dec_var(char *line)
{
	if (*ptr_cm) --*ptr_cm;
	return 0;
}

int  adj_vsize(char *line)
{
	ptr_cm = &vcm;	
	return 0;
}

int  adj_hsize(char *line)
{
	ptr_cm = &hcm;	
	return 0;
}
