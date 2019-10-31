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
#include "makeRect2Face.h"
#include "KScOpenCvUtils.h"

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
int CDECL makeRect2Face(KScUserDefineArray* classifier,KScRgbImage2d* inputImg,KScRgbImage2d* outputImg)
{
	KScCascadeClassifier *pClassifier = NULL;
	pClassifier = ConvertUserDefinedArrayToNewType(pClassifier, classifier); // Convert UserDefinedType to Classifier Type

	if (pClassifier == NULL) {  // if NULL, pointer is empty or different data type
		::MessageBox(NULL, "Not Valid Data: invalid Type or memory not allocated", "_cvFaceDetectWLoadedClassifier", MB_OK);
		return FALSE;
	}

	int dx = inputImg->GetXSize();
	int dy = inputImg->GetYSize();

	outputImg->Free();
	outputImg->Alloc(dx, dy);

	// KScRgbImage2d to Mat.
	cv::Mat inputMat = KScRgbImage2dToMat(inputImg);
	cv::Mat subMat;


	cv::Mat grayImg, smallImg;
	cv::cvtColor(inputMat, grayImg, CV_BGR2GRAY);

	// Point of Face Rect.
	std::vector<cv::Rect> faces;
	double scale = 1.0, fx;
	fx = 1 / scale;

	// Resize the Grayscale Image.
	resize(grayImg, smallImg, Size(), fx, fx, INTER_LINEAR);
	equalizeHist(smallImg, smallImg);

	// Detect faces of different sizes using cascade classifier.
	pClassifier->face_classifier.detectMultiScale(smallImg, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	// Draw the rect.
	for (int i = 0; i < faces.size(); i++) {
		Point lb(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
		Point tr(faces[i].x, faces[i].y);

		Rect rect(lb, tr);
		rectangle(inputMat, rect, Scalar(0, 0, 0), 1, 4, 0);

		subMat = inputMat(rect);
	}


	// Mat to KScRgbImage2d.
	MatToKScRgbImage2d(subMat, outputImg);


	return TRUE;
}

// 2 개 이상의 함수를 정의할 경우, 아래에 수동으로 삽입합니다.


