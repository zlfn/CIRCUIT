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
	drawText(buf, L"������ ������ �������ų� ġ������ ������ �߻��߽��ϴ�.", 0, 0, 100, Color::White);
	return 0;
}

int playErrorScreen(Buffer buf, GameState* state)
{
	//���� �����忡�� ������ ����� �� �ִ� ������ ���.
	if (!start)
	{
		thread no(delay, state);
		no.detach();
		start = false;
	}
	return 0;
}