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

//�ڵ尡 ��ģ���� �����ϱ� ������ ���ϴ� ���� ������ ���� �ּ��� �޾Ƴ�������, 
//VisualStudio�� ����ȭ �ּ� ����� ���� �̿��߽��ϴ�. GoodLuck

//���� �Լ��� �ִ� ���� �����Դϴ�. �����׿�.
#include <windows.h>
#include <stdlib.h>
#include "Window.h"
#include "Chars.h"
#include "Graphic.h"

//�� ������ �� ���� �� ����Ŭ�� ��ġ�� �˴ϴ�.
int main()
{
	//�ʱ⼳��
	initWchar();
	//�ܼ��� ���� �Է� ��� (Ŭ��)�� ��Ȱ��ȭ
	DWORD prev_mode;
	HANDLE std;
	std = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(std, &prev_mode);
	SetConsoleMode(std, prev_mode & ~ENABLE_QUICK_EDIT_MODE);

	bool flag = true;

	Buffer buf;
	buf = getBuffer(20, 10);

	for (;;) {

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
		{
			if (flag)
			{
				buf.textBuf[i][j] = L')';
				buf.colorBuf[i][j] = 3;
			}
			else
			{
				buf.textBuf[i][j] = L'(';
				buf.colorBuf[i][j] = 4;
			}
		}
	for (int i = 10; i < 20; i++)
		for (int j = 0; j < 10; j++)
		{
			if (flag)
			{
				buf.textBuf[i][j] = L'-';
				buf.colorBuf[i][j] = 4;
			}
			else
			{
				buf.textBuf[i][j] = L'|';
				buf.colorBuf[i][j] = 3;
			}
		}
		renderBuffer(buf, 20, 10);
		swapBuffer(buf);
		Sleep(10);
		flag = !flag;
	}
}
