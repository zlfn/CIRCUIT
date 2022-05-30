#include "Graphic.h"
#include "GameState.h"
#include "Scenes.h"
#include <thread>

using namespace std;

int start = false;

void delay(GameState *state)
{
	Sleep(3000);
	resetCard();
	start = true;
	state->scene = Main;
}

int drawErrorScreen(Buffer buf, GameState state)
{
	drawText(buf, L"상대와의 연결이 끊어졌거나 치명적인 오류가 발생했습니다.", 0, 0, 100, Color::White);
	return 0;
}

int playErrorScreen(Buffer buf, GameState* state)
{
	//메인 스레드에서 슬립을 사용할 수 있는 유일한 장소.
	if (!start)
	{
		thread no(delay, state);
		no.detach();
		start = false;
	}
	return 0;
}