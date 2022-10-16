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
#include "func01.h"

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
int CDECL func01(KScRgbImage2d* inputImg,KScRgbImage2d* outputImg)
{
	// Initializing Data
	int dx = inputImg->GetXSize();
	int dy = inputImg->GetYSize();

	outputImg->Free();
	outputImg->Alloc(dx, dy);

	// RGB Buffer Allocation
	KScScalarImage2dUint8 *RedPlane = new KScScalarImage2dUint8;
	KScScalarImage2dUint8 *GreenPlane = new KScScalarImage2dUint8;
	KScScalarImage2dUint8 *BluePlane = new KScScalarImage2dUint8;

	RedPlane->Alloc(dx, dy);
	GreenPlane->Alloc(dx, dy);
	BluePlane->Alloc(dx, dy);

	inputImg->SplitRedPlane(RedPlane);
	inputImg->SplitGreenPlane(GreenPlane);
	inputImg->SplitBluePlane(BluePlane);

	// 2D Array Converting
	KSdUint8 **pRed2dArray = RedPlane->Get2dArray();
	KSdUint8 **pGreen2dArray = GreenPlane->Get2dArray();
	KSdUint8 **pBlue2dArray = BluePlane->Get2dArray();


	// RGB Transfomr
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


			// Skin Color RGB
			if (pRed2dArray[y][x] < 190 || pRed2dArray[y][x] > 220) {
				if (pGreen2dArray[y][x] < 140 || pGreen2dArray[y][x] > 170) {
					if (pBlue2dArray[y][x] < 120 || pBlue2dArray[y][x] > 140) {
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


	// RGB Plane Merging
	outputImg->MergeAllPlane(RedPlane, GreenPlane, BluePlane);

	delete RedPlane;
	delete GreenPlane;
	delete BluePlane;

	return TRUE;
}


