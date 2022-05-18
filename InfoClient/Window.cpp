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
#include "Graphic.h"

/// <summary>
/// ��ũ�� ���۸� ����ũ�⿡ ����ϴ�.
/// </summary>
/// <param name="buf">ũ�⸦ ������ ��ũ�� ����</param>
/// <param name="x">x���� ũ��</param>
/// <param name="y">y���� ũ��</param>
/// <param name="noSpace">������� ��¿��� (���������� ������ �߻��Ͽ���)</param>
/// <returns>���������� �����Ǿ��ٸ� 0�� ���ϵ˴ϴ�.</returns>
int setWindow(Buffer buf, bool noSpace)
{
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;

	CONSOLE_SCREEN_BUFFER_INFO bufinfo;
	GetConsoleScreenBufferInfo(buf.screen, & bufinfo);
	SMALL_RECT windowSize;

	if (noSpace)
	{
		windowSize.Left = 0;
		windowSize.Top = 0;
		windowSize.Right = buf.size.x - 1;
		windowSize.Bottom = buf.size.y - 1;
	}
	else
	{
		windowSize.Left = 0;
		windowSize.Top = 0;
		windowSize.Right = buf.size.x;
		windowSize.Bottom = buf.size.y;
	}

	COORD bufSize = { buf.size.x,buf.size.y};
	SetConsoleScreenBufferSize(buf.screen,bufSize);

	//SetConsoleWIndowInfo�� �ܼ�â�� ���ϼ��� ������ �ø����� �����ϴ�.
	//���� �ܼ�â�� ��ǥ ũ�⺸�� �۴ٸ� SetWindowPos�� �̿��� ������ �ø��ϴ�.
	if (SetConsoleWindowInfo(buf.screen, TRUE, &windowSize) == 0)
	{
		SetWindowPos(GetConsoleWindow(), 0, 0, 0, 10000, 10000, SWP_NOMOVE | SWP_SHOWWINDOW);
		SetConsoleWindowInfo(buf.screen, TRUE, &windowSize); //<��õ�
	}
	SetConsoleCursorInfo(buf.screen, &cci);
	return 0;
}
