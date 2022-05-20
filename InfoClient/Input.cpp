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

#include "Input.h"
#include <thread>
using namespace std;

/// <summary>
/// ���콺 Ŭ���� �޴� �����������Դϴ�.
/// </summary>
MouseClick* c;

void getClickTH(MouseClick* c)
{
	MouseClick result;

	INPUT_RECORD rec;
	DWORD dwNOER;

	for (;;)
	{
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &rec, 1, &dwNOER);

		if (rec.EventType == MOUSE_EVENT)
		{
			if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
			{
				result.pos.X = rec.Event.MouseEvent.dwMousePosition.X;
				result.pos.Y = rec.Event.MouseEvent.dwMousePosition.Y;
				result.type = Left;
			}
			else if (rec.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
			{
				result.pos.X = rec.Event.MouseEvent.dwMousePosition.X;
				result.pos.Y = rec.Event.MouseEvent.dwMousePosition.Y;
				result.type = Right;
			}
		}
		*c = result;
	}
}

int startGetClick()
{
	c = new MouseClick;
	thread input(getClickTH, c);
	input.detach();
	return 0;
}

MouseClick getClick()
{
	MouseClick temp = *c;
	c->type = None;
	return temp;
}
