//GNU GPL 3.0 lisence
/* 
 * 정보 수행평가 게임
 * Copyright (C) 2022 박찬웅, 김진서, 박지환
 *
 * 이 프로그램은 자유 소프트웨어입니다. 소프트웨어의 피양도자는 자유 소프트웨어
 * 재단이 공표한 GNU 일반 공중 사용 허가서 3판 혹은 그 이후 판을 임의로 선택하여
 * 그 규정에 따라 프로그램을 개작하거나 재배포할 수 있습니다.
 *
 * 이 프로그램은 유용하게 사용될 수 있으리라는 희망에서 배포되고 있지만, 특정한
 * 목적에 맞는 적합성 여부나 판매용으로 사용할 수 있으리라는 묵시적인 보증을 포함한
 * 어떠한 형태의 보증도 제공하지 않습니다. 보다 자세한 사항에 대해서는
 * GNU 일반 공중 허가서를 참고하시기 바랍니다.
 * 
 * GNU 일반 공중 사용 허가서는 이 프로그램과 함께 제공됩니다. 만약 문서가 누락되어있다면
 * <http://www.gnu.org/licenses/>을 참조하시기 바랍니다.
 */

//코드가 미친듯이 복잡하기 때문에 과하다 싶을 정도로 많은 주석을 달아놓았으며, 
//VisualStudio의 문서화 주석 기능을 적극 이용했습니다. GoodLuck

//메인 함수가 있는 메인 파일입니다. 멋지네요.
#include <windows.h>
#include <iostream>
#include <ctime>
#include <random>
#include "Window.h"
#include "Chars.h"
#include "Graphic.h"

//한 프레임 당 메인 한 사이클을 거치게 됩니다.
int main()
{
	//초기설정
	initWchar();
	//콘솔의 빠른 입력 모드 (클릭)을 비활성화
	DWORD prev_mode;
	HANDLE std;
	std = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(std, &prev_mode);
	SetConsoleMode(std, prev_mode & ~ENABLE_QUICK_EDIT_MODE);
	SetConsoleMode(std, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dis(0, 200);

	Buffer buf;
	buf = getBuffer(80, 40);

	double frame = 0;

	for (;;) {
		clock_t start = clock();

		resetBuffer(buf, 80, 40);

		wchar frametext[50];
		swprintf_s(frametext, sizeof(frametext), L"%.1lf", frame);
		drawText(buf, frametext, 76, 39, 4, 15);

		renderBuffer(buf, 80, 40, 2);
		setWindow(buf,80,40);
		swapBuffer(buf);

		clock_t end = clock();
		frame =1/((double)(end - start) / CLOCKS_PER_SEC);
	}
}
