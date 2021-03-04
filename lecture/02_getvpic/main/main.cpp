#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <ISCSDKLib.h>

using namespace cv;
using namespace std;

#include "libbasic.h"

ISCSDKLib	*sdk = NULL;
ISCSDKLib::CameraParamInfo	camera_param;

int	width, height;

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
}

int main(int argc, char **argv)
{
	int	n;

	remove_ftdi_sio();
	sdk = (ISCSDKLib*)new ISCSDKLib();
	n = sdk->OpenISC();
	check_if_0("OpenISC", n);
	atexit(bye);
	read_camera_param();
	get_size(&width, &height);

	n = sdk->SetShutterControlMode(true);	// auto gain/exposure mode
	check_if_0("SetShutterControlMode", n);
	n = sdk->SetAutoCalibration(1);		// auto mode
	check_if_0("SetAutoCalibration", n);

	n = sdk->StartGrab(2);	// 2:depth , 3:calibrated 4: original 
	check_if_0("StartGrab", n);
	Mat lmat = Mat::zeros(height, width, CV_8UC1);
	Mat rmat = Mat::zeros(height, width, CV_8UC1);
	Mat fdepth = Mat::zeros(height, width, CV_32F);

	// must wait!! to have 1st frame
	do {
		n = sdk->GetImage((unsigned char*)lmat.data, (unsigned char*)rmat.data, 1); 
	} while (n != 0)
		;
	n = sdk->GetDepthInfo((float*)fdepth.data);
	check_if_0("GetDepthInfo", n);

#if 1
	namedWindow("original");
	namedWindow("depth");
	moveWindow("original", 0, 0);
	moveWindow("depth", rmat.rows+10, 0);
#endif
	rotate(fdepth, fdepth, ROTATE_90_CLOCKWISE);	// ROTATE_180, ROTATE_90_COUNTERCLOCKWISE
	flip(fdepth, fdepth, 1);			// flip x
	imshow("depth", fdepth);
	rotate(rmat, rmat, ROTATE_90_CLOCKWISE);	// ROTATE_180, ROTATE_90_COUNTERCLOCKWISE
	flip(rmat, rmat, 1);				// flip x
	imshow("original", rmat);
	waitKey(0);
	return 0;
}
