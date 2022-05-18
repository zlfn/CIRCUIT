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

//�׷��� ���� �Լ��� ��Ƴ��� �����Դϴ�.
/*�� ������ cout, printf�� ����� ǥ�� ������� ������� �ʽ��ϴ�.
���,WindowsAPI�� ���� Ȱ���Ͽ� TUI �׷����� ������۸� ������� ����մϴ�.
���Ǵ� ������ ����� ���� ������ �Ʒ��� �ֽ��ϴ�.*/

//������ ������ ����
/* 1. �� �����Ӹ��� ���� ���� �ڵ�κ��� ������ �Ǿ�� �� ��������Ʈ�� �Է¹޽��ϴ�.
*  2. ������ ��������Ʈ�� �׷��� �����͸� �������� ���ۿ� �׷��� �����͸� �����մϴ�.
*  3. ������ �׷��� �����͸� �������� ��ũ�� ���۸� �������մϴ�.
*  4. �������� ��ũ�� ���۸� ���� ���ۿ� �ٲߴϴ�.
* 
* ����� ������ ���� �ٷ�� ����鼭�� ������ ������ ���� �ܼ� �׷����� ǥ���� �� �ֽ��ϴ�.
*/

#include "Chars.h"
#include <conio.h>
#include <vector>
#include <windows.h>
#include <iostream>
#include <thread>
#include "Graphic.h"
using namespace std;

/// <summary>
/// ���۸� ȭ�鿡 ǥ���մϴ�.
/// </summary>
/// <param name="bbuf">ǥ���� ����</param>
/// <returns>���������� ǥ�õǾ��ٸ� 0�� ���ϵ˴ϴ�.</returns>
int swapBuffer(Buffer bbuf)
{
	//SetConsoleActiveScreenBuffer�Լ��� ���������� ������� �ʾ��� �� 0�� �����մϴ�. �̸� �����Ͽ� ����ó���մϴ�.
	if (SetConsoleActiveScreenBuffer(bbuf.screen) != 0) return 0;
	else
	{
		cout << "ERR: ��ũ�� ���� ǥ�ÿ� �����Ͽ����ϴ�. ����ġ ���� �������� ���۰� �����Ǿ��� ���ɼ��� �ֽ��ϴ�.";
		throw;
	}
}

/// <summary>
/// ���� �����͸� �̿��Ͽ� ��ũ�� ���۸� �������ϴ� �Լ��Դϴ�.
/// ��Ƽ�������� �̿��Ұ�� 10% ������ ���� ����� �ֽ��ϴ�.
/// </summary>
/// <param name="buf">�������� ����</param>
/// <param name="x">������ xũ��</param>
/// <param name="y_start">�������� ������ y��ǥ</param>
/// <param name="y_end">�������� ���� y��ǥ</param>
void renderTH(Buffer buf,int x, int y_start, int y_end)
{
	COORD cur;
	for (int i = y_start; i < y_end; i++)
	{
		cur.X = 0;
		cur.Y = i;
		for (int j = 0; j < x; j++)
		{
			CHAR_INFO letter;
			letter.Char.UnicodeChar = buf.textBuf[j][i]; //< X,Y ������ ���߱� ���� i,j�� �������ϴ�.
			SetConsoleTextAttribute(buf.screen, buf.colorBuf[j][i]);
			SetConsoleCursorPosition(buf.screen, cur);
			DWORD ws;
			WriteConsole(buf.screen, &letter, 1, &ws, NULL);

			if (isWide(buf.textBuf[j][i])) {
				j++; cur.X += 2;
			} //<�ѱ��� �����ϰԵ� �ܼ�â ��ĭ�� �����մϴ�. (�ѱ��� ������ ���� Textbuf�� ����
			else cur.X += 1; //<������ �ƽ�Ű�ڵ�� �ܼ�â ��ĭ�� �����մϴ�.
		}
	}
	return;
}

/// <summary>
/// ������ ������ �̿��Ͽ� ��ũ�����۸� �������մϴ�.
/// �̶� ��Ƽ�������� �̿��մϴ�.
/// </summary>
/// <param name="buf">�������� ����</param>
/// <param name="x">��ũ�������� x ������</param>
/// <param name="y">��ũ�������� y ������</param>
/// <param name="threadSize">����� ������ ��. �ݵ�� y���� ������� �մϴ�.</param>
/// <returns>���������� ������ �Ǿ��ٸ� 0�� ���ϵ˴ϴ�.</returns>
int renderBuffer(Buffer buf,int x, int y, int threadSize)
{
	//��ü y�ټ��� ������ ������ŭ ������ ������ �����忡 �й��մϴ�.
	if (y % threadSize != 0)
	{
		//�� �������� ����
		cout << "������ ������ ������ �������մϴ�.";
		throw;
	}
	vector<thread> threads;
	for (int i = 0; i < threadSize; i++)
		threads.push_back(thread(renderTH,buf, x, (y / threadSize) * i, (y / threadSize) * (i + 1)));

	//�������� �۾��� ������ ���� ���..���� �ϴµ� �ȱ�ٸ��ϴ�. �� �̻���
	for (int i = 0; i < threadSize; i++)
		threads[i].join();
	return 0;
}


/// <summary>
/// ���۸��� ���� Buffer������ �����Ҵ��Ͽ� �����ɴϴ�.
/// </summary>
/// <param name="x">��ũ�� ������ x ������</param>
/// <param name="y">��ũ�� ������ y ������</param>
/// <returns>�Ҵ�� Buffer������ ��ȯ�˴ϴ�.</returns>
Buffer getBuffer(int x, int y)
{
	//Ŀ���� ������ �ʰ� �մϴ�.
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;

	COORD bufsize = { x,y };

	Buffer buf;
	try
	{
		//�׷��� ������ 2���� �迭�� �����Ҵ��մϴ�.
		buf.textBuf = new wchar*[x];
		for (int i = 0; i < x; i++)
		buf.textBuf[i] = new wchar[y];
		buf.colorBuf = new int*[x];
		for (int i = 0; i < x; i++)
		buf.colorBuf[i] = new int[y];

		//������ ��ĭ���� �ʱ�ȭ�մϴ�.
		for(int i = 0; i<x; i++)
			for (int j = 0; j < y; j++)
			{
				buf.textBuf[i][j] = ' ';
				buf.colorBuf[i][j] = 15;
			}

		//��ũ�� ���� HANDLE�� �߱޹޽��ϴ�.
		buf.screen = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleCursorInfo(buf.screen, &cci);
		SetConsoleScreenBufferSize(buf.screen, bufsize);

		return buf;
	}
	catch (std::bad_alloc) {
		//new�� ������ �����Ͽ����� catch �մϴ�.
		cout << "���� �Ҵ翡 �����Ͽ����ϴ�.";
		throw;
	}
}

/// <summary>
/// ���۸��� ���� Buffer������ �����մϴ�.
/// </summary>
/// <param name="buf">������ Buffer����</param>
/// <param name="x">��ũ�� ������ x���� ������</param>
/// <param name="y">��ũ�� ������ y���� ������</param>
/// <returns>���������� �����Ǿ��ٸ� 0�� ���ϵ˴ϴ�.</returns>
int freeBuffer(Buffer buf,int x, int y)
{
	//�����Ҵ� �� �������� ��� ��ȯ�մϴ�.
	for (int i = 0; i < x; i++)
	{
		delete buf.textBuf[i];
		delete buf.colorBuf[i];
	}
	delete buf.textBuf;
	delete buf.colorBuf;
	
	//��ũ������ �ڵ��� �ݾ��ݴϴ�.
	CloseHandle(buf.screen);
	return 0;
}

/// <summary>
/// ���ۿ� Ư�� �ʺ��� ���� �����մϴ�.
/// </summary>
/// <param name="buf">���� ����� ����</param>
/// <param name="text">����� �ؽ�Ʈ, �ѱۿ� ������ �������� �ʽ��ϴ�.</param>
/// <param name="x">����� ������ x��ǥ</param>
/// <param name="y">����� ������ y��ǥ</param>
/// <param name="width">�ؽ�Ʈ�� �ʺ�</param>
/// <param name="color">�ؽ�Ʈ�� ����</param>
/// <returns>���������� �����ߴٸ� 0�� ��ȯ�˴ϴ�.</returns>
int drawText(Buffer buf, const wchar* text, int x, int y, int width, int color)
{
	int c = 0;
	int cx = x;
	int cy = y;
	while (text[c] != '\0')
	{
		for (int i = 0; i < width; i++)
		{
			buf.textBuf[cx][cy] = text[c];
			buf.colorBuf[cx][cy] = color;
			c++;
			cx++;

			if (isWide(text[c]))
			{
				cx++; i++;
			}

			if (text[c] == '\0') break;
		}
		cx = x;
		cy++;
	}

	return 0;
}

/// <summary>
/// ������ �����͸� �����մϴ�.
/// ��ũ�� ���۴� �������� �ʽ��ϴ�.
/// </summary>
/// <param name="buf">������ ����</param>
/// <param name="x">������ x���� ������</param>
/// <param name="y">������ y���� ������</param>
/// <returns>���������� ���µǸ� 0�� ��µ˴ϴ�. </returns>
int resetBuffer(Buffer buf, int x, int y)
{
	for(int i = 0; i<x; i++)
		for (int j = 0; j < y; j++)
		{
			buf.textBuf[i][j] = ' ';
			buf.colorBuf[i][j] = 15;
		}
	return 0;
}