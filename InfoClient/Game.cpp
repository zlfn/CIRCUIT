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


int drawGame(Buffer buf, GameState state)
{
	switch (state.scene)
	{
	case Main:
		drawMainScreen(buf, state);
		break;
	case Setting:
		drawSettingScreen(buf, state);
		break;
	case Host:
		drawHostScreen(buf, state);
		break;
	case Find:
		drawFindScreen(buf, state);
		break;
	}
	return 0;
}

int playGame(Buffer buf, GameState* state)
{
	switch (state->scene)
	{
	case Main:
		playMainScreen(buf, state);
		break;
	case Setting:
		playSettingScreen(buf, state);
		break;
	case Host:
		playHostScreen(buf, state);
		break;
	case Find:
		playFindScreen(buf, state);
		break;
	}
	return 0;
}