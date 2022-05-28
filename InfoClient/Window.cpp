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

//������ â�� ���õ� �Լ��� ��Ƴ��� �����Դϴ�.
#include <Windows.h>
#include <cwchar>
#include "Graphic.h"

int setWindow(Buffer buf, bool noSpace)
{
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;

	CONSOLE_SCREEN_BUFFER_INFO bufinfo;
	GetConsoleScreenBufferInfo(buf.screen, & bufinfo);
	SMALL_RECT windowSize;
	COORD bufSize = {buf.size.x, buf.size.y};

	if (noSpace)
	{
		windowSize.Left = 0;
		windowSize.Top = 0;
		windowSize.Right = buf.size.x - 1;
		windowSize.Bottom = buf.size.y - 1;
		bufSize.X = buf.size.x;
		bufSize.Y = buf.size.y;
	}
	else
	{
		windowSize.Left = 0;
		windowSize.Top = 0;
		windowSize.Right = buf.size.x;
		windowSize.Bottom = buf.size.y;
		bufSize.X = buf.size.x+1;
		bufSize.Y = buf.size.y+1;
	}

	SetConsoleScreenBufferSize(buf.screen, bufSize);

	if (SetConsoleWindowInfo(buf.screen, TRUE, &windowSize)==0)
	{
		//���� dll���� ���� ��, ���� �ʿ�
		SetWindowPos(GetConsoleWindow(), 0, 0, 0, 10000, 10000, SWP_NOMOVE | SWP_SHOWWINDOW);

		SetConsoleWindowInfo(buf.screen, TRUE, &windowSize);
	}
	
	//��Ʈ�� ������ �� �ִٴ� ����� ������ ���� ���޾ҽ��ϴ�! ¥��
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 15;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	wcscpy_s(cfi.FaceName, LF_FACESIZE, L"NSimsun");
	SetCurrentConsoleFontEx(buf.screen, FALSE, &cfi);
	SetConsoleCursorInfo(buf.screen, &cci);
	return 0;
}
