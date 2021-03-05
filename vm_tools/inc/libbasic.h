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

#define HERE printf("@%d, %s, %s\n", __LINE__, __func__, __FILE__);
#define PI ((double)3.141592654)
#define D2R(X) ((double)X*PI/180.0)
#define R2D(X) ((double)X*180.0/PI)

using namespace std;
using namespace cv;

void remove_ftdi_sio(void);
void check_if_0(const char *title, int n);
int allocate_rgb_mem(int cm, uchar **red, uchar **green, uchar **blue);
void draw_grid(Mat mat, int xdiv, int ydiv, int value);
void mat_printf_color(Scalar col);
void mat_printf_size(double n);
void mat_printf(Mat mat, int x, int y, const char *fmt, ...);
int read_camera_param(void);
void copy_camera_param(void *ptr);
void get_size(int *pwidth, int *pheight);
float get_view_angle(void);
float depth2distance(float depth);
int depth2cm(float depth);
int allocate_rgb_mem(int cm, uchar **red, uchar **green, uchar **blue);
void dump_mem(void *ptr, int n);
