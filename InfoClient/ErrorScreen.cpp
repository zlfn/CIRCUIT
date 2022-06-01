//GNU GPL 3.0 lisence
/*
 * ���� ������ ����
 * Copyright (C) 2022 ������, ������, ����ȯ
 *
 * �� ���α׷��� ���� ����Ʈ�����Դϴ�. ����Ʈ������ �Ǿ絵�ڴ� ���� ����Ʈ����
 * ����� ��ǥ�� GNU �Ϲ� ���� ��� �㰡�� 3�� Ȥ�� �� ���� ���� ���Ƿ� �����Ͽ�
 * �� ������ ���� ���α׷��� �����ϰų� ������� �� �ֽ��ϴ�.
 *
 * �� ���α׷��� �����ϰ� ���� �� ��������� ������� �����ǰ� ������, Ư����
 * ������ �´� ���ռ� ���γ� �Ǹſ����� ����� �� ��������� �������� ������ ������
 * ��� ������ ������ �������� �ʽ��ϴ�. ���� �ڼ��� ���׿� ���ؼ���
 * GNU �Ϲ� ���� �㰡���� �����Ͻñ� �ٶ��ϴ�.
 *
 * GNU �Ϲ� ���� ��� �㰡���� �� ���α׷��� �Բ� �����˴ϴ�. ���� ������ �����Ǿ��ִٸ�
 * <http://www.gnu.org/licenses/>�� �����Ͻñ� �ٶ��ϴ�.
 */

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