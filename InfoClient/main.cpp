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

//코드가 미친듯이 복잡하기 때문에 많은 주석을 달아놓았으며, 
//특히 헤더파일에는 게임의 작동 원리와 함수에 대한 상세한 설명을 적어놓았습니다.

//주석작성에 VisualStudio의 문서화 주석 기능을 적극 이용했습니다.
//따라서 코드 확인 시 VisualStudio 이용을 적극 권장합니다.
//(에초에 VisualStudio말고는 컴파일이 되기는 하는지 모르겠네요.)

//사용한 VisualStudio버전은 VisualStudio Professional 2022 입니다.
//WindowsAPI를 아주 아주 많-이 사용하기 때문에 Windows 10, 11 네이티브 환경을 
//제외한 환경에서의 정상작동을 보증하지 않습니다.

//가상머신, Windows 8.1 이하, Wine, MacOS Parallels Desktop 에서는 문제생겨도 안 고쳐줄거에요


//이 파일은 메인 함수가 있는 메인 파일입니다. 멋지네요.
#include <windows.h>
#include <ctime>
#include <random>
#include <thread>
#include "Window.h"
#include "Input.h"
#include "Chars.h"
#include "Graphic.h"
#include "GameState.h"

using namespace std;

//한 프레임 당 메인 한 사이클을 거치게 됩니다.
int main()
{
	//콘솔의 빠른 입력 모드 (클릭)을 비활성화
	DWORD prev_mode;
	HANDLE std;
	std = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(std, &prev_mode);
	SetConsoleMode(std, prev_mode & ~ENABLE_QUICK_EDIT_MODE);
	SetConsoleMode(std, ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

	//랜덤 디바이스로 시드 초기화
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> dis(0, 200);

	//버퍼 두개 할당받음
	Buffer buf[2];
	bool bufCount = false;
	buf[0] = getBuffer(80, 40);
	buf[1] = getBuffer(80, 40);

	//fps측정 변수
	double frame = 0;
	
	//잔상제거체크 변수
	double refreshCheck = 0;

	//초기 설정
	initWchar();
	startGetClick();
	resetGameState();

	int CardX;
	int CardY;
	int CardClicked;
	bool CardColor;

	//랜더링 사이클
	for (;;) {
		clock_t start = clock();

		Settings set = getGameState().setting;

		resetBuffer(buf[bufCount]);
		
		//화면 크기 조정
		setWindow(buf[0], set.noSpaceWindow);
		setWindow(buf[1], set.noSpaceWindow);

		//병렬 리프레시
		if (set.refresh)
		{
			if (refreshCheck >= set.refreshInterval)
			{
				thread refresh(refreshBuffer,buf[bufCount],set.refreshThreadsCount);
				if (set.parallelRefresh)
				{
					refresh.detach();
				}
				else
				{
					refresh.join();
				}
				refreshCheck = 0;
			}
		}

		//프론트 버퍼 디더링
		if (set.frontBufferDithering)
		{
			for (int i = 0; i < set.ditheringSize; i++)
			{
				buf[!bufCount].colorBuf[dis(gen) % buf->size.x][dis(gen) % buf->size.y] = 16;
			}
		}

		//랜덤 글자 렌더링
		/*for (int i = 0; i < 1; i++)
		{
			buf[bufCount].textBuf[dis(gen) % 80][dis(gen) % 40] = 31 + dis(gen) % 96;
			buf[bufCount].textBuf[dis(gen) % 80][dis(gen) % 40] = ' ';
			buf[bufCount].textBuf[dis(gen) % 80][dis(gen) % 40] = ' ';
			//buf[bufCount].colorBuf[dis(gen) % 80][dis(gen) % 40] = dis(gen) % 15;
			buf[bufCount].colorBuf[dis(gen) % 80][dis(gen) % 40] = Color::Yellow;
			buf[bufCount].colorBuf[dis(gen) % 80][dis(gen) % 40] = Color::Gray;
		}*/

		//그림판 렌더링
		MouseClick c = getClick();
		//if(c.type==Left)drawText(buf[bufCount], L"██", c.pos.X, c.pos.Y, 2, Color::White);
		//if(c.type==Right)drawText(buf[bufCount], L"        ", c.pos.X, c.pos.Y, 4, Color::White);


		if (c.type == Left) 
		{
			CardColor = true;
			drawImage(buf[bufCount], L"CardA1.gres", c.pos.X - 4, c.pos.Y-3);
			CardX = c.pos.X;
			CardY = c.pos.Y;
		}
		if (c.type == Right) 
		{
			CardColor = false;
			drawImage(buf[bufCount], L"CardB1.gres", c.pos.X -4, c.pos.Y-3);
			CardX = c.pos.X;
			CardY = c.pos.Y;
		}

		if (c.type == None)
		{
			if (CardColor)
				drawImage(buf[bufCount], L"CardA1.gres", CardX - 4, CardY - 3);
			else
				drawImage(buf[bufCount], L"CardB1.gres", CardX - 4, CardY - 3);
		}



		drawImage(buf[bufCount], L"GBSLAND_LOGO.gres", 11, 4);
		drawText(buf[bufCount], L"경기북과학고등학교 정보 수행평가", 37, 10, 100, Color::LightYellow);
		drawText(buf[bufCount], L"(C) 2022. 박찬웅, 김진서, 박지환", 0/*29*/, 38, 200, Color::Yellow);
		drawText(buf[bufCount], L"This software distribute under GNU GPL 3.0 license", 0/*30*/, 39, 200, Color::Yellow);

		//FPS 표시
		if (set.showFPS)
		{
			wchar frametext[50];
			swprintf_s(frametext, sizeof(frametext), L"%.2lf", frame);
			drawText(buf[bufCount], frametext, 0, 39, 5, Color::White);
		}

		//버퍼 렌더링, 표시
		renderBuffer(buf[bufCount],buf[!bufCount], set.renderThreadsCount);
		swapBuffer(buf[bufCount]);

		//버퍼 스왑, 동기화
		bufCount = !bufCount;
		syncroBuffer(buf[bufCount], buf[!bufCount]);

		//FPS 측정
		clock_t end = clock();
		frame =	1 / ((double)(end - start) / CLOCKS_PER_SEC);
		refreshCheck += (double)(end - start) / CLOCKS_PER_SEC;

		//안정화
		Sleep(0);
	}
}
