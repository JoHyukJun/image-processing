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
#include "jLoadModel.h"

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
int CDECL jLoadModel(KScUserDefineArray* outFaceModel, KScUserDefineArray* outGenderModel, KScUserDefineArray* outAgeModel)
{
	JoModelReader *facePtr = NULL;
	JoModelReader *genderPtr = NULL;
	JoModelReader *agePtr = NULL;


	// Model path.
	// Face model.
	string faceProto = "C:\\Program Files (x86)\\MTES2.0\\FMS\\JoHyukJun\\models\\opencv_face_detector.pbtxt";
	string faceModel = "C:\\Program Files (x86)\\MTES2.0\\FMS\\JoHyukJun\\models\\opencv_face_detector_uint8.pb";

	// Age model.
	string ageProto = "C:\\Program Files (x86)\\MTES2.0\\FMS\\JoHyukJun\\models\\age_deploy.prototxt";
	string ageModel = "C:\\Program Files (x86)\\MTES2.0\\FMS\\JoHyukJun\\models\\age_net.caffemodel";

	// Gender model.
	string genderProto = "C:\\Program Files (x86)\\MTES2.0\\FMS\\JoHyukJun\\models\\gender_deploy.prototxt";
	string genderModel = "C:\\Program Files (x86)\\MTES2.0\\FMS\\JoHyukJun\\models\\gender_net.caffemodel";

	facePtr = AttachNewObjToUDA(facePtr, outFaceModel);
	facePtr->face_Model = readNetFromTensorflow(faceModel, faceProto);

	genderPtr = AttachNewObjToUDA(genderPtr, outGenderModel);
	genderPtr->gender_Model = readNet(genderModel, genderProto);

	agePtr = AttachNewObjToUDA(agePtr, outAgeModel);
	agePtr->age_Model = readNet(ageModel, ageProto);

	return TRUE;
}