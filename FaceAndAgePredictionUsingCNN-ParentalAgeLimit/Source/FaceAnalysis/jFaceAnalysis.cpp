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
#include "jFaceAnalysis.h"

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


tuple<Mat, vector<vector<int>>> getFaceBox(Net net, Mat &frame, double faceConfidenceThreshold) {
	Mat frameFull = frame.clone();
	int frameHeight = frameFull.rows;
	int frameWidth = frameFull.cols;
	double inScaleFactor = 1.0;
	Size size = Size(300, 300);

	// 영상 평균 값.
	Scalar meanVal = Scalar(104, 117, 123);

	cv::Mat inputBlob;
	// blob 객체는 1 * 3 * 300 * 300 형태의 4차원 행렬.
	// net 객체에 입력으로 사용.
	inputBlob = cv::dnn::blobFromImage(frameFull, inScaleFactor, size, meanVal, true, false);

	net.setInput(inputBlob, "data");

	// detection 은 1 * 1 * N * 7 크기의 4차원 행렬.
	// 처음 두 개의 차원 크기는 항상 1이고, 네 번째 차원 크기는 항상 7, N 은 검출된 얼굴 후보 영역 개수.
	// 최대 200개 까지 검출 가능.
	cv::Mat detection = net.forward("detection_out");

	// 1, 2 차원 크기는 항상 1이므로 detection 을 3, 4 차원만 이용하여 새로운 2차원 행렬 구성.
	// N * 7 크기의 2차원 행열(TYPE : CV_32F).
	// 행렬의 0번, 1번 열에는 항상 0과 1 저장 -> 2번째 열에는 face confidence 저장.
	// confidence 는 0부터 1 사이의 float 로 저장.
	// 얼굴일 가능성이 높을면 1에 가까운 값으로 저장.
	// 3번부터 6번 열에는 얼굴 영역 사각형 좌측 상단 좌표 (x1, y1) 과 우측 하단 좌표 (x2, y2) 저장.
	// 사각형 좌표는 영상의 가로와 세로 크기를 1로 정규화하여 저장된 좌표이므로 실제 픽셀 좌표는 영상의 가로 및 세로 크기를 곱해서 계산.
	// 각 행은 face confidence 가 높은 순서대로 정렬.
	// 행을 읽으면서 face confidence 가 임계값을 기준으로 출력.
	cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

	// 좌측 상단 좌표 (x1, y1), 우측 하단 좌표 (x2, y2).
	vector<vector<int>> bboxes;


	// Draw Rectangle.
	for (int i = 0; i < detectionMat.rows; i++)
	{
		// face confidence 0부터 1사이의 float.
		float faceConfidence = detectionMat.at<float>(i, 2);

		if (faceConfidence > faceConfidenceThreshold)
		{
			int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frameWidth);
			int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frameHeight);
			int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frameWidth);
			int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frameHeight);
			vector<int> box = { x1, y1, x2, y2 };
			bboxes.push_back(box);

			// 사각형 그리기.
			cv::rectangle(frameFull, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 0, 255), 2, 4);
		}
	}

	return make_tuple(frameFull, bboxes);
}


// 함수의 정의 부분
int CDECL jFaceAnalysis(KScUserDefineArray* faceModel, KScUserDefineArray* genderModel, KScUserDefineArray* ageModel, KScUserDefineArray* src_uda, KScUserDefineArray* dst_uda, int* outGender, int* outAge)
{
	// Get the source image
	KScMatType_cv *matSrcPtr = NULL;
	matSrcPtr = GetNewObjFromUDA(matSrcPtr, src_uda); // Convert UserDefinedType to Mat Type
	if (matSrcPtr == NULL) {  // if NULL, pointer is empty or different data type
		::MessageBox(NULL, "ERROR: Type Mismatched or Memory not Initializated", "jFaceAnalytics", MB_OK);
		return FALSE;
	}

	// Create the output image.
	KScMatType_cv *matDstPtr = NULL;
	matDstPtr = AttachNewObjToUDA(matDstPtr, dst_uda);  // define Mat for User define Array


	// Get the face model.
	JoModelReader *faceNet = NULL;
	faceNet = GetNewObjFromUDA(faceNet, faceModel); // Convert UserDefinedType to Net Type
	if (faceNet == NULL) {  // if NULL, pointer is empty or different data type
		::MessageBox(NULL, "ERROR: Type Mismatched or Memory not Initializated", "jFaceAnalytics", MB_OK);
		return FALSE;
	}

	// Get the gender model.
	JoModelReader *genderNet = NULL;
	genderNet = GetNewObjFromUDA(genderNet, genderModel); // Convert UserDefinedType to Net Type
	if (genderNet == NULL) {  // if NULL, pointer is empty or different data type
		::MessageBox(NULL, "ERROR: Type Mismatched or Memory not Initializated", "jFaceAnalytics", MB_OK);
		return FALSE;
	}

	// Get the age model.
	JoModelReader *ageNet = NULL;
	ageNet = GetNewObjFromUDA(ageNet, ageModel); // Convert UserDefinedType to Net Type
	if (ageNet == NULL) {  // if NULL, pointer is empty or different data type
		::MessageBox(NULL, "ERROR: Type Mismatched or Memory not Initializated", "jFaceAnalytics", MB_OK);
		return FALSE;
	}

	Mat frame = matSrcPtr->image.clone();

	vector<vector<int>> bboxes;
	Mat frameFace;

	// Get face box.
	// face confiden threshold value 는 0.7 로 선택
	tie(frameFace, bboxes) = getFaceBox(faceNet->face_Model, frame, 0.7);

	// System error.
	if (bboxes.size() == 0) {
		::MessageBox(NULL, "ERROR: No face detected, checking next frame.", "jFaceAnalytics", MB_OK);
		return FALSE;
	}

	// Age, Gender prediction.
	for (auto it = begin(bboxes); it != end(bboxes); ++it) {
		Rect rec(it->at(0) - padding, it->at(1) - padding, it->at(2) - it->at(0) + 2 * padding, it->at(3) - it->at(1) + 2 * padding);

		// take the ROI of box on the frame.
		Mat face = frame(rec);

		Mat blob;
		// blob 객체는 1 * 3 * 277 * 277 형태의 4차원 행렬 -> net 객체에 입력으로 사용.
		blob = blobFromImage(face, 1, Size(227, 227), MODEL_MEAN_VALUES, false);

		// Finding gender.
		genderNet->gender_Model.setInput(blob);
		vector<float> genderPreds = genderNet->gender_Model.forward();

		// Find max element index.
		int max_index_gender = std::distance(genderPreds.begin(), max_element(genderPreds.begin(), genderPreds.end()));
		string gender = genderList[max_index_gender];
		GenderParser(gender, outGender);

		// Finding age.
		ageNet->age_Model.setInput(blob);
		vector<float> agePreds = ageNet->age_Model.forward();

		// Finding maximum indicd in the age_preds vector.
		int max_indice_age = std::distance(agePreds.begin(), max_element(agePreds.begin(), agePreds.end()));
		string age = ageList[max_indice_age];
		AgeParser(age, outAge);

		// Label to put in the image.
		string label = "[ Gender : " + gender + ", Age : " + age + "]";
		cv::putText(frameFace, label, Point(it->at(0), it->at(1) - 15), cv::FONT_HERSHEY_PLAIN, 1.2, Scalar(0, 255, 0), 2, cv::LINE_AA);

	}


	// Full frame image.
	matDstPtr->image = frameFace.clone();

	return TRUE;
}


void GenderParser(string inGender, int* outGender) {
	if (inGender == "Male") {
		*outGender = 0;
	}
	else if (inGender == "Female") {
		*outGender = 1;
	}

	return;
}


void AgeParser(string inAge, int* outAge) {
	if (inAge == "(0-2)") {
		*outAge = 0;
	}
	else if (inAge == "(4-6)") {
		*outAge = 1;
	}
	else if (inAge == "(8-12)") {
		*outAge = 2;
	}
	else if (inAge == "(15-20)") {
		*outAge = 3;
	}
	else if (inAge == "(25-32)") {
		*outAge = 4;
	}
	else if (inAge == "(38-43)") {
		*outAge = 5;
	}
	else if (inAge == "(48-53)") {
		*outAge = 6;
	}
	else if (inAge == "(60-100)") {
		*outAge = 7;
	}

	return;
}


int GenderAgeParser(int gender, int age) {
	if (gender == 0) {
		switch (age) {
		case 0:
			return 1;
			break;
		case 1:
			return 2;
			break;
		case 2:
			return 3;
			break;
		case 3:
			return 4;
			break;
		case 4:
			return 5;
			break;
		case 5:
			return 6;
			break;
		case 6:
			return 7;
			break;
		case 7:
			return 8;
			break;
		default:
			return 0;
			break;
		}

	}
	else if (gender == 1) {
		switch (age) {
		case 0:
			return 9;
			break;
		case 1:
			return 10;
			break;
		case 2:
			return 11;
			break;
		case 3:
			return 12;
			break;
		case 4:
			return 13;
			break;
		case 5:
			return 14;
			break;
		case 6:
			return 15;
			break;
		case 7:
			return 16;
			break;
		default:
			return 0;
			break;
		}
	}
}


