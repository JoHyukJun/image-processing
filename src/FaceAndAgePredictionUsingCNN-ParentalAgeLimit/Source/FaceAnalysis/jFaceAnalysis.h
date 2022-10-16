//////////////////////////////////////////////////////////////////////////
//
// 이 화일은 함수 등록시 자동으로 생성된 프로젝트입니다.
//
// 사용자가 임으로 만든 경우, Project의 Setting...의 설정이 필요하므로,
//
// 이는 도움말의 '함수 작성법'을 참조 바랍니다.
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// KSClass 라이브러리의 링크
//
//////////////////////////////////////////////////////////////////////////
//
// 헤더 파일과 라이브러리 파일 검색을 위하여
// Visual Studio에서 메뉴 Tools - Options - Directories 탭에서
// Show Directories for: 부분의
//     Include files에는 KSClass의 Include의 디렉토리를
//     Library files에는 KSClass의 Lib       디렉토리를 설정하여야 한다.
//
//////////////////////////////////////////////////////////////////////////
//
// * 주의점
//
// Show Directories for: 부분의
//     Include files에 KSClass의 Include와 Lib 디렉토리를 모두 설정할 경우,
//     링크 에러 발생한다.
//
//////////////////////////////////////////////////////////////////////////

#include "KSClass.h" // KSClass들에 대한 헤더

#include <KScOpenCvUtils.h>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <stdlib.h>
#include <vector>
#include <tuple>
#include <iterator>


using namespace cv;
using namespace cv::dnn;
using namespace std;

// DLL 외부에서 호출하기 위해서 필요한 부분 ...
extern "C" __declspec( dllexport )
int CDECL jFaceAnalysis(KScUserDefineArray* par0, KScUserDefineArray* par1, KScUserDefineArray* par2, KScUserDefineArray* par3, KScUserDefineArray* par4, int* par5, int* par6);

Scalar MODEL_MEAN_VALUES = Scalar(78.4263377603, 87.7689143744, 114.895847746);

vector<string> ageList = { "(0-2)", "(4-6)", "(8-12)", "(15-20)", "(25-32)", "(38-43)", "(48-53)", "(60-100)" };
vector<string> genderList = { "Male", "Female" };


int padding = 20;


void GenderParser(string inGender, int* outGender);


void AgeParser(string inAge, int* outAge);


int GenderAgeParser(int gender, int age);


