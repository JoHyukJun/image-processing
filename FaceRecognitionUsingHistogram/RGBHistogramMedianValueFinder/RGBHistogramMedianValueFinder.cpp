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
#include "RGBHistogramMedianValueFinder.h"
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
int CDECL RGBHistogramMedianValueFinder(KScRgbImage2d* originalImg,KScRgbImage2d* inputImg,KScRgbImage2d* outputImg)
{
	// KScRgbImage2d to Mat.
	Mat inputMat = KScRgbImage2dToMat(inputImg);

	// Separate the Face Image in B, G, R Plane.
	vector<Mat> bgrPlane;
	split(inputMat, bgrPlane);


	int histSize = 256;
	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true;
	bool accumulate = false;

	Mat bHist, gHist, rHist;

	// Calculate the Histogram.
	calcHist(&bgrPlane[0], 1, 0, Mat(), bHist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgrPlane[1], 1, 0, Mat(), gHist, 1, &histSize, &histRange, uniform, accumulate);
	calcHist(&bgrPlane[2], 1, 0, Mat(), rHist, 1, &histSize, &histRange, uniform, accumulate);

	// Normalize the result to 0, 255.
	normalize(bHist, bHist, 0, 255, NORM_MINMAX, -1, Mat());
	normalize(gHist, bHist, 0, 255, NORM_MINMAX, -1, Mat());
	normalize(rHist, bHist, 0, 255, NORM_MINMAX, -1, Mat());


	// Draw the Histogram.
	/*int hist_w = 512, hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));*/

	/*for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(bHist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(bHist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(gHist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(gHist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(rHist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(rHist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
	}*/


	// Compute Cumulative Distribution Function.
	Mat bCDF, gCDF, rCDF;
	bHist.copyTo(bCDF);
	gHist.copyTo(gCDF);
	rHist.copyTo(rCDF);


	for (int i = 1; i < histSize; i++) {
		bCDF.at<float>(i) += bCDF.at<float>(i - 1);
		gCDF.at<float>(i) += gCDF.at<float>(i - 1);
		rCDF.at<float>(i) += rCDF.at<float>(i - 1);
	}

	bCDF /= bgrPlane[0].total();
	gCDF /= bgrPlane[1].total();
	rCDF /= bgrPlane[2].total();

	// Compute Median Value.
	int bMedianVal = 128;
	int gMedianVal = 128;
	int rMedianVal = 128;

	for (int i = 0; i < histSize; i++) {
		if (bCDF.at<float>(i) >= 0.5) {
			bMedianVal = i;
			break;
		}
	}

	for (int i = 0; i < histSize; i++) {
		if (gCDF.at<float>(i) >= 0.5) {
			gMedianVal = i;
			break;
		}
	}

	for (int i = 0; i < histSize; i++) {
		if (rCDF.at<float>(i) >= 0.5) {
			rMedianVal = i;
			break;
		}
	}

	// Compute Minimum and Maximum Value of RGB.
	Mat bTemp, gTemp, rTemp;
	bHist.copyTo(bTemp);
	gHist.copyTo(gTemp);
	rHist.copyTo(rTemp);

	int bMinVal = 128, bMaxVal = 128;
	int gMinVal = 128, gMaxVal = 128;
	int rMinVal = 128, rMaxVal = 128;

	float bCum = 0, gCum = 0, rCum = 0;

	for (int i = 1; i < histSize; i++) {
		bCum = bCum + bTemp.at<float>(bMedianVal + i) + bTemp.at<float>(bMedianVal - i) + bTemp.at<float>(bMedianVal);
		if (bCum / bgrPlane[0].total() >= 0.2) {
			bMinVal = bMedianVal - i;
			bMaxVal = bMedianVal + i;

			break;
		}
	}

	for (int i = 1; i < histSize; i++) {
		gCum = gCum + gTemp.at<float>(gMedianVal + i) + gTemp.at<float>(gMedianVal - i) + gTemp.at<float>(gMedianVal);
		if (gCum / bgrPlane[1].total() >= 0.2) {
			gMinVal = gMedianVal - i;
			gMaxVal = gMedianVal + i;

			break;
		}
	}

	for (int i = 1; i < histSize; i++) {
		rCum = rCum + rTemp.at<float>(rMedianVal + i) + rTemp.at<float>(rMedianVal - i) + rTemp.at<float>(rMedianVal);
		if (rCum / bgrPlane[2].total() >= 0.2) {
			rMinVal = rMedianVal - i;
			rMaxVal = rMedianVal + i;

			break;
		}
	}


	// Test Value.
	string medi1 = to_string(bMaxVal);
	string medi2 = to_string(bMinVal);
	putText(inputMat, medi1, Point(20, 20), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255, 255, 255), 1, CV_AA);
	putText(inputMat, medi2, Point(40, 40), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(255, 255, 255), 1, CV_AA);


	// Initializing Data.
	int dx = originalImg->GetXSize();
	int dy = originalImg->GetYSize();

	outputImg->Free();
	outputImg->Alloc(dx, dy);

	// RGB Buffer Allocation.
	KScScalarImage2dUint8 *RedPlane = new KScScalarImage2dUint8;
	KScScalarImage2dUint8 *GreenPlane = new KScScalarImage2dUint8;
	KScScalarImage2dUint8 *BluePlane = new KScScalarImage2dUint8;

	RedPlane->Alloc(dx, dy);
	GreenPlane->Alloc(dx, dy);
	BluePlane->Alloc(dx, dy);

	originalImg->SplitRedPlane(RedPlane);
	originalImg->SplitGreenPlane(GreenPlane);
	originalImg->SplitBluePlane(BluePlane);

	// 2D Array Converting.
	KSdUint8 **pRed2dArray = RedPlane->Get2dArray();
	KSdUint8 **pGreen2dArray = GreenPlane->Get2dArray();
	KSdUint8 **pBlue2dArray = BluePlane->Get2dArray();


	// RGB Transform.
	for (int y = 0; y < dy; y++) {
		for (int x = 0; x < dx; x++) {
			if (pRed2dArray[y][x] < 0) {
				pRed2dArray[y][x] = 0;
			}
			if (pGreen2dArray[y][x] < 0) {
				pGreen2dArray[y][x] = 0;
			}
			if (pBlue2dArray[y][x] < 0) {
				pBlue2dArray[y][x] = 0;
			}


			// Skin Color RGB.
			if (pRed2dArray[y][x] < rMinVal || pRed2dArray[y][x] > rMaxVal) {
				if (pGreen2dArray[y][x] < gMinVal || pGreen2dArray[y][x] > gMaxVal) {
					if (pBlue2dArray[y][x] < bMinVal || pBlue2dArray[y][x] > bMaxVal) {
						pRed2dArray[y][x] = 0;
						pGreen2dArray[y][x] = 0;
						pBlue2dArray[y][x] = 0;
					}
				}
			}

			if (pRed2dArray[y][x] > 255) {
				pRed2dArray[y][x] = 255;
			}
			if (pGreen2dArray[y][x] > 255) {
				pGreen2dArray[y][x] = 255;
			}
			if (pBlue2dArray[y][x] > 255) {
				pBlue2dArray[y][x] = 255;
			}
		}
	}


	// RGB Plane Merging.
	outputImg->MergeAllPlane(RedPlane, GreenPlane, BluePlane);

	delete RedPlane;
	delete GreenPlane;
	delete BluePlane;

	return TRUE;
}

// 2 개 이상의 함수를 정의할 경우, 아래에 수동으로 삽입합니다.


