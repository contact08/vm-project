// 以下の ifdef ブロックは DLL からのエクスポートを容易にするマクロを作成するための 
// 一般的な方法です。この DLL 内のすべてのファイルは、コマンド ラインで定義された ISCDOTNET_EXPORTS
// シンボルを使用してコンパイルされます。このシンボルは、この DLL を使用するプロジェクトでは定義できません。
// ソースファイルがこのファイルを含んでいる他のプロジェクトは、 
// ISCDOTNET_API 関数を DLL からインポートされたと見なすのに対し、この DLL は、このマクロで定義された
// シンボルをエクスポートされたと見なします。

#ifndef __ISCSDKLIB_H__
#define	__ISCSDKLIB_H__

#include "WinTypes.h"

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
		// 視野角
		float fViewAngle;
		// 画像サイズ(幅)
		unsigned int nImageWidth;
		// 画像サイズ(縦)
		unsigned int nImageHeight;
		// プロダクトナンバー
		unsigned int nProductNumber;
		// シリアルナンバー
		unsigned int nSerialNumber;
		// FPGA version
		unsigned int nFPGA_version;
		// 物体認識用の基準係数
		unsigned int nDistanceHistValue;
		// 除外対象とする視差の閾値
		unsigned int  nParallaxThreshold;
	};

	// コンストラクタ
	ISCSDKLib();
	~ISCSDKLib();

	int OpenISC();
	int CloseISC();

	int StartGrab(int nMode);
	int StopGrab();
	
	int GetImage(BYTE* pBuffer1, BYTE* pBuffer2,int nSkip);
	int GetDepthInfo(float* pBuffer);

	int SetAutoCalibration(int nMode);
	int GetAutoCalibration(int* nMode);

	int SetShutterControlMode(bool nMode);
	int GetShutterControlMode(bool* nMode);

	int GetImageSize(unsigned int* pnWidth,unsigned int* pnHeight);

	int GetExposureValue(unsigned int* pnValue );
	int SetExposureValue(unsigned int nValue);

	int GetGainValue(unsigned int* pnValue);
	int SetGainValue(unsigned int nValue);

	int GetCameraParamInfo(CameraParamInfo* pParam);

};

#endif	/*__ISCSDKLIB_H__*/
