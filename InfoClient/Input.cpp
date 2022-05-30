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

#include "Input.h"
#include "Graphic.h"
#include <conio.h>
#include <thread>
using namespace std;

/// <summary>
/// 마우스 클릭을 받는 전역포인터입니다.
/// </summary>
MouseClick* c;

/// <summary>
/// 키 입력을 받는 전역포인터입니다.
/// </summary>
int* k;

bool canKey = true;

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
		else if (rec.EventType == KEY_EVENT)
		{
			*k = rec.Event.KeyEvent.wVirtualKeyCode;
		}

		if (!rec.Event.KeyEvent.bKeyDown)
		{
			*k = 0;
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

MouseClick getClickOnce()
{
	MouseClick temp = getClick();
	c->type = None;

	return temp;
}

int getKey()
{
	return *k;
}

int getKeyOnce()
{
	int temp = *k;
	*k = 0;

	return temp;
}

int getClickObject(Buffer buf)
{
	MouseClick c = getClick();

	//여백없는 출력을 끌 시 충돌이 발생하였으므로 처리합니다.
	if (c.pos.X < buf.size.x && c.pos.Y < buf.size.y && c.pos.X >= 0 && c.pos.Y >= 0)
		return buf.clickBuf[c.pos.X][c.pos.Y];
	else return -1;
}
