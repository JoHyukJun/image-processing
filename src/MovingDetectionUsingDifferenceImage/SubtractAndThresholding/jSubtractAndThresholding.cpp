//////////////////////////////////////////////////////////////////////////
//
// 이 화일은 함수 등록시 자동으로 생성된 프로젝트입니다.
//
// 사용자가 임으로 만든 경우, Project의 Setting...의 설정이 필요하므로,
//
// 이는 도움말의 '함수 작성법'을 참조 바랍니다.
//
//////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "jSubtractAndThresholding.h"

// DLL을 만들기 위한 부분
BOOL WINAPI DllMain (HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
		case DLL_PROCESS_ATTACH: break;
		case DLL_THREAD_ATTACH:  break;
		case DLL_THREAD_DETACH:  break;
		case DLL_PROCESS_DETACH: break;
	}

	return TRUE;
}

// 함수의 정의 부분
int CDECL jSubtractAndThresholding(KScScalarImage2dUint8* inBackgroundImg, KScScalarImage2dUint8* inSequenceImg, KScScalarImage2dUint8* outputImg, int* thresholidngValue)
{
	int dx = inBackgroundImg->GetXSize();
	int dy = inBackgroundImg->GetYSize();

	outputImg->Free();
	outputImg->Alloc(dx, dy);

	cv::Mat imgBackground = KScScalarImage2dUint8ToMat(inBackgroundImg);
	cv::Mat imgSequence = KScScalarImage2dUint8ToMat(inSequenceImg);
	cv::Mat imgResult = KScScalarImage2dUint8ToMat(outputImg);

	// Substract and Threshold Image.
	// Pixel Value	:	{0, 255}
	int tempThresholdingValue = 15;

	for (int y = 0; y < dy; y++) {
		for (int x = 0; x < dx; x++) {
			if (fabs(imgBackground.at<uchar>(y, x) - imgSequence.at<uchar>(y, x) < tempThresholdingValue)) {
				imgResult.at<uchar>(y, x) = 0;
			}
			else {
				imgResult.at<uchar>(y, x) = 255;
			}
		}
	}

	*thresholidngValue = tempThresholdingValue;
	MatToKScScalarImage2dUint8(imgResult, outputImg);

	return TRUE;
}

// 2 개 이상의 함수를 정의할 경우, 아래에 수동으로 삽입합니다.


