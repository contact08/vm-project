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

ISCSDKLib	*sdk = NULL;
ISCSDKLib::CameraParamInfo	camera_param;
int	maxcm;
uchar	*red, *green, *blue;
static int	width, height;

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

void impose_depth(Mat colmat, Mat fdepth)
{
	int	x, y, cm;
	float	*fptr = (float*)fdepth.data;
	for (y = 0 ;  y < colmat.rows ; y++) {
		for (x = 0 ;  x < colmat.cols ; x++) {
			cm = depth2cm(*fptr++);
			if (cm >= maxcm || cm <= 0) continue;
			colmat.at<Vec3b>(y, x)[0] = blue[cm];
			colmat.at<Vec3b>(y, x)[1] = green[cm];
			colmat.at<Vec3b>(y, x)[2] = red[cm];
		}
	}
}

void get_date(char *buf, int size)
{
	time_t	now;
	struct tm	*tim;
	time(&now);
	tim = localtime(&now);
	strftime(buf, size, "%Y_%m%d_%H%M%S", tim);
	//printf("%s\n", buf);
}

void write_flame(void)
{
	int		n;
	char		ch;

	remove_ftdi_sio();
	sdk = (ISCSDKLib*)new ISCSDKLib();
	n = sdk->OpenISC();
	check_if_0("OpenISC", n);
	atexit(bye);
	maxcm = allocate_rgb_mem(1000, &red, &green, &blue);
	read_camera_param();
	copy_camera_param(&camera_param);
	get_size(&width, &height);

	n = sdk->SetShutterControlMode(true);	// auto gain/exposure
	check_if_0("SetShutterControlMode", n);

	n = sdk->SetAutoCalibration(1);		// auto mode
	check_if_0("SetAutoCalibration", n);

	n = sdk->StartGrab(2);	// 2:depth , 3:calibrated 4
	if (n < 0) return;
	check_if_0("StartGrab", n);

	Mat lmat = Mat::zeros(height, width, CV_8UC1);
	Mat rmat = Mat::zeros(height, width, CV_8UC1);
	Mat colmat = Mat::zeros(height, width, CV_8UC3);
	Mat fdepth = Mat::zeros(height, width, CV_32F);
#if 1
	namedWindow("original");
	namedWindow("color");
	moveWindow("original", 0, 0);
	moveWindow("color", 0, rmat.rows+36);
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
	impose_depth(colmat, fdepth);
	imshow("original", rmat);
	imshow("color", colmat);
	ch = waitKey(30);
	if (ch == 0x1b) {
		return;
	} else if (ch == 's') {
		int	error = 0;
		char	buf[128], fname[1024];
		get_date(buf, sizeof(buf));
		sprintf(fname, "%sCOL.png", buf);	
		memcpy(colmat.data, &camera_param, sizeof(camera_param));	
        	imwrite(fname, colmat);
		sprintf(fname, "%sIMG.png", buf);	
        	imwrite(fname, rmat);
		FILE	*fp;
		get_date(buf, sizeof(buf));
		sprintf(fname, "%sDEP.bin", buf);	
		if ((fp = fopen(fname, "wb")) == NULL) {
			printf("error: %s can't open for write\n", fname);
			exit(1);
		}
		if (fwrite((uchar*)fdepth.data, sizeof(float), width*height, fp) != (size_t)(width*height)) {
			printf("failed to write depth data\n");
			error = -1;
		}
		fclose(fp);
		if (error == 0) {
			printf("ok, freezed\n");
			goto end;
		}
	}
	goto loop;
end:	
	waitKey(0);
	return;
}

void read_flame(char *file)
{
	FILE	*fp;
	char	buf[1024], fname[104];
	printf("%s\n", file);
	strcpy(buf, file);
	buf[strlen(buf)-strlen("IMG.png")] = '\0';
	sprintf(fname, "%sCOL.png", buf);
	Mat colmat = imread(fname);
	memcpy(&camera_param, colmat.data, sizeof(camera_param));	
	width = camera_param.nImageWidth;
	height = camera_param.nImageHeight;
	printf("width: %d, height: %d\n", width, height);
	imshow("color", colmat);
	sprintf(fname, "%sIMG.png", buf);
	Mat rmat = imread(fname);
	imshow("original", rmat);
#if 1	
	Mat fdepth = Mat::zeros(height, width, CV_32F);
	sprintf(fname, "%sDEP.bin", buf);
	if ((fp = fopen(fname, "rb")) == NULL) {
		printf("%s can't open for read\n", fname);
		return;
	}
	size_t	n = width * height;
	if (fread(fdepth.data, sizeof(float), n, fp) != n) {
		printf("%s can't read\n", fname);
		return;
	}	
	fclose(fp);
	imshow("depth", fdepth);
#endif	
	waitKey(0);
}

int main(int argc, char **argv)
{
	if (argc < 2) {
		write_flame();
	} else {
		read_flame(argv[1]);
	}
}
