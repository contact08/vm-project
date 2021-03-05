#ifndef __ISCSDKLIB_H__
#define	__ISCSDKLIB_H__

// ERROR CODE DEFINE

// OK
#define ISC_OK								0

// FTDI 
#define FT_INVALID_HANDLE					1
#define FT_DEVICE_NOT_FOUND					2
#define FT_DEVICE_NOT_OPENED				3
#define FT_IO_ERROR							4
#define FT_INSUFFICIENT_RESOURCES			5
#define FT_INVALID_PARAMETER				6
#define FT_INVALID_BAUD_RATE				7
#define FT_DEVICE_NOT_OPENED_FOR_ERASE		8
#define FT_DEVICE_NOT_OPENED_FOR_WRITE		9
#define FT_FAILED_TO_WRITE_DEVICE			10
#define FT_EEPROM_READ_FAILED				11
#define FT_EEPROM_WRITE_FAILED				12
#define FT_EEPROM_ERASE_FAILED				13
#define FT_EEPROM_NOT_PRESENT				14
#define FT_EEPROM_NOT_PROGRAMMED			15
#define FT_INVALID_ARGS						16
#define FT_NOT_SUPPORTED					17
#define FT_OTHER_ERROR						18
#define FT_DEVICE_LIST_NOT_READY			19

// ISC
#define ERR_READ_DATA						-1
#define ERR_WRITE_DATA						-2
#define ERR_WAIT_TIMEOUT					-3
#define ERR_OBJECT_CREATED					-4
#define ERR_USB_OPEN						-5
#define ERR_USB_SET_CONFIG					-6
#define ERR_CAMERA_SET_CONFIG				-7
#define ERR_REGISTER_SET					-8
#define ERR_THREAD_RUN						-9
#define ERR_RESET_ERROR						-10
#define ERR_FPGA_MODE_ERROR					-11
#define ERR_GRAB_MODE_ERROR					-12
#define ERR_TABLE_FILE_OPEN					-13
#define ERR_MODSET_ERROR					-14
#define ERR_CALIBRATION_TABLE				-15
#define ERR_GETIMAGE						-16
#define ERR_INVALID_VALUE					-17
#define ERR_NO_CAPTURE_MODE					-18
#define ERR_NO_VALID_IMAGES_CALIBRATING		-19
#define ERR_REQUEST_NOT_ACCEPTED			-20
#define ERR_USB_ERR							-100
#define ERR_USB_ALREADY_OPEN				-101
#define ERR_USB_NO_IMAGE					-102
#define ERR_AUTOCALIB_GIVEUP_WARN			-201
#define ERR_AUTOCALIB_GIVEUP_ERROR			-202
#define ERR_AUTOCALIB_OUTRANGE				-203
#define ERR_IMAGEINPUT_IMAGEERROR			-204

// Shutter Control Mode Define
#define SHUTTER_CONTROLMODE_MANUAL						0
#define SHUTTER_CONTROLMODE_AUTO						1
#define SHUTTER_CONTROLMODE_DOUBLE						2
#define SHUTTER_CONTROLMODE_DOUBLE_INDEPENDENT_OUT		3
#define SHUTTER_CONTROLMODE_SYSTEM_DEFAULT				4

// Auto Calibration
#define AUTOCALIBRATION_COMMAND_STOP					0
#define AUTOCALIBRATION_COMMAND_AUTO_ON					1
#define AUTOCALIBRATION_COMMAND_MANUAL_START			2
#define AUTOCALIBRATION_STATUS_BIT_AUTO_ON				0x00000002
#define AUTOCALIBRATION_STATUS_BIT_MANUAL_RUNNING		0x00000001

class ISCSDKLib
{
public:

	struct CameraParamInfo
	{
		// D INF値
		float	fD_INF;
		// D INT値
		unsigned int nD_INF;
		// BF値
		float fBF;
		// 基線長値
		float fBaseLength;
		// Delta-Z
		float fdZ;
		// 視野角
		float fViewAngle;
		// 画像サイズ(幅)
		unsigned int nImageWidth;
		// 画像サイズ(縦)
		unsigned int nImageHeight;
		// プロダクトナンバー
		unsigned int nProductNumber;
		// プロダクトナンバー(上位ワード)
		unsigned int nProductNumber2;
		// シリアルナンバー(文字列)
		char nSerialNumber[16];
		// FPGA version
		unsigned int nFPGA_version_major;
		unsigned int nFPGA_version_minor;
		// 物体認識用の基準係数
		unsigned int nDistanceHistValue;
		// 除外対象とする視差の閾値
		unsigned int  nParallaxThreshold;
	};

	// コンストラクタ
	ISCSDKLib();
	~ISCSDKLib();

	// 外部公開関数
	int OpenISC();
	int CloseISC();

	int SetISCRunMode(int nMode);
	int GetISCRunMode(int* pMode);

	int StartGrab(int nMode);
	int StopGrab();

	int GetImage(unsigned char* pBuffer1, unsigned char* pBuffer2,int nSkip);
	int GetDepthInfo(float* pBuffer);

	int GetCameraParamInfo(CameraParamInfo* pParam);
	int GetImageSize(unsigned int* pnWidth, unsigned int* pnHeight);

	int SetAutoCalibration(int nMode);
	int GetAutoCalibration(int* nMode);

	int SetAutomaticDisparityAdjustment(int nMode);
	int GetAutomaticDisparityAdjustment(int* pnMode);

	int SetForceDisparityAdjustment(int nMode);
	int GetForceDisparityAdjustment(int* pnMode);

	int SetShutterControlMode(int nMode);
	int GetShutterControlMode(int* nMode);

	int SetExposureValue(unsigned int nValue);
	int GetExposureValue(unsigned int* pnValue );

	int SetGainValue(unsigned int nValue);
	int GetGainValue(unsigned int* pnValue);

	int SetHDRMode(int nValue);
	int SetHiResolutionMode(int nValue);

	int SetNoiseFilter(unsigned int nDCDX);
	int GetNoiseFilter(unsigned int* nDCDX);

	int SetMeasArea(int mode, int nTop, int nLeft, int nRight, int nBottom, int nTop_Left, int nTop_Right, int nBottom_Left, int nBottom_Right);
	int GetMeasArea(int* mode, int* nTop, int* nLeft, int* nRight, int* nBottom, int* nTop_Left, int* nTop_Right, int* nBottom_Left, int* nBottom_Right);

	int SetCameraFPSMode(int nMode);
	int GetCameraFPSMode(int* pnMode, int* pnNominalFPS);

};

#endif	/*__ISCSDKLIB_H__*/

