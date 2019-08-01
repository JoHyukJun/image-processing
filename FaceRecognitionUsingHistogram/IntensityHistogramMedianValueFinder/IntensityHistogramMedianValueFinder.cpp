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
#include "IntensityHistogramMedianValueFinder.h"
#include "KScOpenCvUtils.h"

using namespace std;
using namespace cv;

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
int CDECL IntensityHistogramMedianValueFinder(KScRgbImage2d* originalImg,KScRgbImage2d* inputImg,KScRgbImage2d* outputImg)
{
	// KScRgbImage2d to Mat.
	Mat inputMat = KScRgbImage2dToMat(inputImg);

	// Convert BGR Image to HSV Image.
	Mat hsvMat;
	cvtColor(inputMat, hsvMat, CV_BGR2HSV);

	// Separate the Face Image in H, S, V Plane.
	vector<Mat> hsvPlane;
	split(hsvMat, hsvPlane);

	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true;
	bool accumulate = false;

	Mat vHist;

	// Calculate the Histogram.
	calcHist(&hsvPlane[2], 1, 0, Mat(), vHist, 1, &histSize, &histRange, uniform, accumulate);

	// Normalize the result to 0, 255.
	normalize(vHist, vHist, 0, 255, NORM_MINMAX, -1, Mat());

	// Compute Cumulative Distribution Function.
	Mat vCDF;
	vHist.copyTo(vCDF);

	for (int i = 1; i < histSize; i++) {
		vCDF.at<float>(i) += vCDF.at<float>(i - 1);
	}

	vCDF /= hsvPlane[2].total();

	// Compute Median Value.
	int vMedianVal = 128;

	for (int i = 0; i < histSize; i++) {
		if (vCDF.at<float>(i) >= 0.5) {
			vMedianVal = i;
			break;
		}
	}

	// Compute Minimum and Maximum Value of V.
	Mat vTemp;

	vHist.copyTo(vTemp);

	int vMinVal = 128, vMaxVal = 128;
	float vCum = 0;

	for (int i = 1; i < histSize; i++) {
		vCum = vCum + vTemp.at<float>(vMedianVal + i) + vTemp.at<float>(vMedianVal - i) + vTemp.at<float>(vMedianVal);
		if (vCum / hsvPlane[2].total() >= 0.2) {
			vMinVal = vMedianVal - i;
			vMaxVal = vMedianVal + i;

			break;
		}
	}

	int dx = originalImg->GetXSize();
	int dy = originalImg->GetYSize();

	outputImg->Free();
	outputImg->Alloc(dx, dy);

	KScHsiImage2d *srcHsiImg = new KScHsiImage2d;
	KScHsiImage2d *dstHsiImg = new KScHsiImage2d;
	srcHsiImg->Alloc(dx, dy);
	dstHsiImg->Alloc(dx, dy);

	KScScalarImage2dFloat *Hue = new KScScalarImage2dFloat;
	KScScalarImage2dFloat *Saturation = new KScScalarImage2dFloat;
	KScScalarImage2dFloat *Intensity = new KScScalarImage2dFloat;
	Hue->Alloc(dx, dy);
	Saturation->Alloc(dx, dy);
	Intensity->Alloc(dx, dy);

	originalImg->ConvertToHsiImage(srcHsiImg);

	srcHsiImg->SplitAllPlane(Hue, Saturation, Intensity);

	KSdFloat **pArrayHue = Hue->Get2dArray();
	KSdFloat **pArraySaturation = Saturation->Get2dArray();
	KSdFloat **pArrayIntensity = Intensity->Get2dArray();

	for (int y = 0; y < dy; y++) {
		for (int x = 0; x < dx; x++) {
			if (pArrayIntensity[y][x] < vMinVal || pArraySaturation[y][x] > vMaxVal) {
				pArrayIntensity[y][x] = 0;
			}


			if (pArrayHue[y][x] < 0) {
				pArrayHue[y][x] == 0;
			}
			else if (pArrayHue[y][x] > 360) {
				pArrayHue[y][x] == 360;
			}

			if (pArraySaturation[y][x] < 0) {
				pArraySaturation[y][x] == 0;
			}
			else if (pArraySaturation[y][x] > 1) {
				pArraySaturation[y][x] == 1;
			}

			if (pArrayIntensity[y][x] < 0) {
				pArrayIntensity[y][x] == 0;
			}
			else if (pArrayIntensity[y][x] > 255) {
				pArrayIntensity[y][x] == 255;
			}
		}
	}

	dstHsiImg->MergeAllPlane(Hue, Saturation, Intensity);

	dstHsiImg->ConvertToRgbImage(outputImg);

	delete srcHsiImg;
	delete dstHsiImg;
	delete Hue;
	delete Saturation;
	delete Intensity;

	return TRUE;
}

// 2 개 이상의 함수를 정의할 경우, 아래에 수동으로 삽입합니다.


