// �ȉ��� ifdef �u���b�N�� DLL ����̃G�N�X�|�[�g��e�Ղɂ���}�N�����쐬���邽�߂� 
// ��ʓI�ȕ��@�ł��B���� DLL ���̂��ׂẴt�@�C���́A�R�}���h ���C���Œ�`���ꂽ ISCDOTNET_EXPORTS
// �V���{�����g�p���ăR���p�C������܂��B���̃V���{���́A���� DLL ���g�p����v���W�F�N�g�ł͒�`�ł��܂���B
// �\�[�X�t�@�C�������̃t�@�C�����܂�ł��鑼�̃v���W�F�N�g�́A 
// ISCDOTNET_API �֐��� DLL ����C���|�[�g���ꂽ�ƌ��Ȃ��̂ɑ΂��A���� DLL �́A���̃}�N���Œ�`���ꂽ
// �V���{�����G�N�X�|�[�g���ꂽ�ƌ��Ȃ��܂��B

#ifndef __ISCSDKLIB_H__
#define	__ISCSDKLIB_H__

#include "WinTypes.h"

class ISCSDKLib
{
public:
	struct CameraParamInfo
	{
		// D INF�l
		float	fD_INF;
		// D INT�l
		unsigned int nD_INF;
		// BF�l
		float fBF;
		// ������l
		float fBaseLength;
		// ����p
		float fViewAngle;
		// �摜�T�C�Y(��)
		unsigned int nImageWidth;
		// �摜�T�C�Y(�c)
		unsigned int nImageHeight;
		// �v���_�N�g�i���o�[
		unsigned int nProductNumber;
		// �V���A���i���o�[
		unsigned int nSerialNumber;
		// FPGA version
		unsigned int nFPGA_version;
		// ���̔F���p�̊�W��
		unsigned int nDistanceHistValue;
		// ���O�ΏۂƂ��鎋����臒l
		unsigned int  nParallaxThreshold;
	};

	// �R���X�g���N�^
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
