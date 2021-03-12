#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <linux/types.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "libbasic.h"
#include "libx3.h"

static int	angle_initialized = 0;
static float	hangle, vangle;
static double	xfull, yfull;
static double	xcenter, ycenter;
static int	width, height;
static int	rolling_degree;

ARBOX *new_arbox(void)
{
	void	*ptr = malloc(sizeof(ARBOX));
	if (ptr == NULL) {
		printf("failed to allocate ARBOx\n");
		exit(1);
	}
	return (ARBOX*)ptr;
}

void destroy_arbox(ARBOX *ptr)
{
	free(ptr);
}

void set_arbox(ARBOX *ptr, Scalar rec, double altitude, double ground)
{
	for (int i = 0 ; i < 4 ; i++) {
		ptr->hrec.val[i] = ptr->lrec.val[i] = rec.val[i];
	}
	ptr->altitude = altitude;
	ptr->ground = ground;
	ptr->color = Scalar(0, 0, 255); // default is red except ground
}

Point get_coord(double xm, double zm, double ym)
{
	int	x, y;
	if (!angle_initialized) {
		hangle = get_hangle();
		vangle = get_vangle();
		printf("hangle: %f, vangle: %f\n", hangle, vangle);
		get_size(&width, &height);
		angle_initialized = 1;
		printf("parameters initialized\n");
	}
	xfull = 2.0*zm*tan(D2R(hangle/2.0));
	yfull = 2.0*zm*tan(D2R(vangle/2.0));
	xcenter = xfull/2.0;
	ycenter = yfull/2.0;
	x = (int)((double)width*(xm+xcenter))/xfull;
	y = (int)((double)height*(ym+ycenter))/yfull;
	return Point(x, y);
}

void set_rolling(int degree)
{
	rolling_degree = degree;
}

void init_pf_coordinate(ARBOX *ptr)
{
	Scalar	lrec = ptr->lrec;
	Scalar	hrec = ptr->hrec;
	double	ground = ptr->ground;
	double	altitude = ptr->altitude;
	Point	*pfcoord = ptr->pfcoord;
	pfcoord[0] = get_coord(lrec.val[0], lrec.val[1], ground);	
	pfcoord[1] = get_coord(lrec.val[0], lrec.val[1]+lrec.val[3], ground);	
	pfcoord[2] = get_coord(lrec.val[0]+lrec.val[2], lrec.val[1]+lrec.val[3], ground);	
	pfcoord[3] = get_coord(lrec.val[0]+lrec.val[2], lrec.val[1], ground);	
	pfcoord[4] = get_coord(hrec.val[0], hrec.val[1], ground-altitude);	
	pfcoord[5] = get_coord(hrec.val[0], hrec.val[1]+hrec.val[3], ground-altitude);	
	pfcoord[6] = get_coord(hrec.val[0]+hrec.val[2], hrec.val[1]+hrec.val[3], ground-altitude);	
	pfcoord[7] = get_coord(hrec.val[0]+hrec.val[2], hrec.val[1], ground-altitude);	
	for (int i = 0 ; i < 8 ; i++) {
		double	rad = D2R(rolling_degree);
		pfcoord[i].x =  pfcoord[i].x*cos(rad)+pfcoord[i].y*sin(rad);
		pfcoord[i].y = -pfcoord[i].x*sin(rad)+pfcoord[i].y*cos(rad);
		//printf("%d: %3d, %3d\n", i, pfcoord[i].x, pfcoord[i].y);
	}
}


void write_arbox(Mat mat, ARBOX *ptr)
{
	int	i, f, t;

	init_pf_coordinate(ptr);
	for (f = i = 0 ; i < 4 ; i++, ++f) {
		t = f+1;
		if  (t >= 4) t = 0;
		line(mat, ptr->pfcoord[f], ptr->pfcoord[t], ptr->color, 1); 
	}
	for (f = i = 0 ; i < 4 ; i++, ++f) {
		t = f+1;
		if  (t >= 4) t = 0;
		line(mat, ptr->pfcoord[f+4], ptr->pfcoord[t+4], ptr->color, 1); 
	}
	for (f = i = 0 ; i < 4 ; i++, ++f) {
		t = f+4;
		line(mat, ptr->pfcoord[f], ptr->pfcoord[t], ptr->color, 1); 
	}
}

void erase_arbox(Mat mat, ARBOX *ptr)
{
	int	i, f, t;
	Scalar	backup;
	backup = ptr->color;
	ptr->color = Scalar(0,0,0);
	init_pf_coordinate(ptr);
	for (f = i = 0 ; i < 4 ; i++, ++f) {
		t = f+1;
		if  (t >= 4) t = 0;
		line(mat, ptr->pfcoord[f], ptr->pfcoord[t], ptr->color, 5); 
	}
	for (f = i = 0 ; i < 4 ; i++, ++f) {
		t = f+1;
		if  (t >= 4) t = 0;
		line(mat, ptr->pfcoord[f+4], ptr->pfcoord[t+4], ptr->color, 5); 
	}
	for (f = i = 0 ; i < 4 ; i++, ++f) {
		t = f+4;
		line(mat, ptr->pfcoord[f], ptr->pfcoord[t], ptr->color, 5); 
	}
	ptr->color = backup;
}

