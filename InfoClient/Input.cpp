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
#include "Graphic.h"
#include <thread>
using namespace std;

/// <summary>
/// ���콺 Ŭ���� �޴� �����������Դϴ�.
/// </summary>
MouseClick* c;

/// <summary>
/// Ű �Է��� �޴� �����������Դϴ�.
/// </summary>
int* k;

void getInputTH(MouseClick* c, int* k)
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
			else
			{
				result.pos.X = rec.Event.MouseEvent.dwMousePosition.X;
				result.pos.Y = rec.Event.MouseEvent.dwMousePosition.Y;
				result.type = None;
			}
			*c = result;
		}
		if (rec.EventType == KEY_EVENT)
		{
			*k = rec.Event.KeyEvent.wVirtualKeyCode;
		}
	}
}


int startGetInput()
{
	c = new MouseClick;
	k = new int;
	thread input(getInputTH, c, k);
	input.detach();
	return 0;
}

MouseClick getClick()
{
	return *c;
}

int getKey()
{
	return *k;
}

int getClickObject(Buffer buf)
{
	MouseClick c = getClick();

	//������� ����� �� �� �浹�� �߻��Ͽ����Ƿ� ó���մϴ�.
	if (c.pos.X < buf.size.x && c.pos.Y < buf.size.y)
		return buf.clickBuf[c.pos.X][c.pos.Y];
	else return -1;
}
