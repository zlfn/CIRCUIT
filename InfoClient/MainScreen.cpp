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
#include "Input.h"

const int SETTING_BUTTON = 1;
const int Card1 = 2;
const int Card2 = 3;

int Card1X = 65;
int Card1Y = 7;
int Card2X = 7;
int Card2Y = 2;

//1���� ���� �ִ� ����
bool stack = false;

int drawMainScreen(Buffer buf, GameState state)
{
	drawImage(buf, L"SettingButton.gres", 68, 37, SETTING_BUTTON);

	if (stack)
	{
		drawImage(buf, L"Card2.gres", Card2X, Card2Y, Card2);
		drawImage(buf, L"Card1.gres", Card1X, Card1Y, Card1);
	}
	else
	{
		drawImage(buf, L"Card1.gres", Card1X, Card1Y, Card1);
		drawImage(buf, L"Card2.gres", Card2X, Card2Y, Card2);
	}

	drawImage(buf, L"GBSLAND_LOGO.gres", 11, 4);
	drawText(buf, L"���ϰ��а���б� ���� ������", 37, 10, 100, Color::LightYellow);
	drawText(buf, L"(C) 2022. ������, ������, ����ȯ", 0/*29*/, 38, 200, Color::Yellow);
	drawText(buf, L"This software distribute under GNU GPL 3.0 license", 0/*30*/, 39, 200, Color::Yellow);


	return 0;
}

int playMainScreen(Buffer buf, GameState* state)
{
	if (getClickOnce().type == Left)
	{
		switch (getClickObject(buf))
		{
		case SETTING_BUTTON:
			state->scene = Setting;
			break;
		case Card1:
			Card1X = getClick().pos.X - 3;
			Card1Y = getClick().pos.Y - 3;
			stack = true;
			break;
		case Card2:
			Card2X = getClick().pos.X - 3;
			Card2Y = getClick().pos.Y - 3;
			stack = false;
			break;
		}
	}
	return 0;
}