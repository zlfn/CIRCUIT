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
#include "Window.h"
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

void renderTH(Buffer buf, int y_start, int y_end)
{
	COORD cur;
	for (int i = y_start; i < y_end; i++)
	{
		cur.X = 0;
		cur.Y = i;
		for (int j = 0; j < buf.size.x; j++)
		{
			CHAR_INFO letter;
			letter.Char.UnicodeChar = buf.textBuf[j][i]; //< X,Y ������ ���߱� ���� i,j�� �������ϴ�.
			SetConsoleTextAttribute(buf.screen, buf.colorBuf[j][i]);
			SetConsoleCursorPosition(buf.screen, cur);
			DWORD ws;
			WriteConsole(buf.screen, &letter, 1, &ws, NULL);
			if (isWide(buf.textBuf[j][i])) {
				j++; cur.X += 2;
			} //<�ѱ��� �����ϰԵ� �ܼ�â ��ĭ�� �����մϴ�. (�ѱ��� ������ ���� Textbuf�� ����)
			else cur.X += 1; //<������ �ƽ�Ű�ڵ�� �ܼ�â ��ĭ�� �����մϴ�.
		}
	}

	Sleep(0);
	return;
}

void renderTH_OP(Buffer buf, Buffer bbuf, int y_start, int y_end)
{
	COORD cur;
	for (int i = y_start; i < y_end; i++)
	{
		cur.X = 0;
		cur.Y = i;
		for (int j = 0; j < buf.size.x; j++)
		{
			if (buf.textBuf[j][i] != bbuf.textBuf[j][i] || buf.colorBuf[j][i] != bbuf.colorBuf[j][i])
			{
				CHAR_INFO letter;
				letter.Char.UnicodeChar = buf.textBuf[j][i]; //< X,Y ������ ���߱� ���� i,j�� �������ϴ�.
				SetConsoleTextAttribute(buf.screen, buf.colorBuf[j][i]);
				SetConsoleCursorPosition(buf.screen, cur);
				DWORD ws;
				WriteConsole(buf.screen, &letter, 1, &ws, NULL);
			}
			if (isWide(buf.textBuf[j][i])) {
				j++; cur.X += 2;
			} //<�ѱ��� �����ϰԵ� �ܼ�â ��ĭ�� �����մϴ�. (�ѱ��� ������ ���� Textbuf�� ����)
			else cur.X += 1; //<������ �ƽ�Ű�ڵ�� �ܼ�â ��ĭ�� �����մϴ�.
		}
	}

	Sleep(0);
	return;
}

int renderBuffer(Buffer buf, int threadSize)
{
	//��ü y�ټ��� ������ ������ŭ ������ ������ �����忡 �й��մϴ�.
	if (buf.size.y % threadSize != 0)
	{
		//�� �������� ����
		cout << "������ ������ ������ �������մϴ�.";
		throw;
	}
	vector<thread> threads;
	for (int i = 0; i < threadSize; i++)
		threads.push_back(thread(renderTH, buf, (buf.size.y / threadSize) * i, (buf.size.y / threadSize) * (i + 1)));

	//�������� �۾��� ������ ���� ���..���� �ϴµ� �ȱ�ٸ��ϴ�. �� �̻���
	for (int i = 0; i < threadSize; i++)
		threads[i].join();
	return 0;
}

int renderBuffer(Buffer buf, Buffer bbuf, int threadSize)
{
	//��ü y�ټ��� ������ ������ŭ ������ ������ �����忡 �й��մϴ�.
	if (buf.size.y % threadSize != 0)
	{
		//�� �������� ����
		cout << "������ ������ ������ �������մϴ�.";
		throw;
	}
	vector<thread> threads;
	for (int i = 0; i < threadSize; i++)
		threads.push_back(thread(renderTH_OP, buf, bbuf, (buf.size.y / threadSize) * i, (buf.size.y / threadSize) * (i + 1)));

	//�������� �۾��� ������ ���� ���..���� �ϴµ� �ȱ�ٸ��ϴ�. �� �̻���
	for (int i = 0; i < threadSize; i++)
		threads[i].join();
	return 0;
}

Buffer getBuffer(int x, int y)
{
	//Ŀ���� ������ �ʰ� �մϴ�.
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;

	COORD bufsize = { x,y };

	Buffer buf;
	buf.size.x = x;
	buf.size.y = y;
	try
	{
		//�׷��� ������ 2���� �迭�� �����Ҵ��մϴ�.
		buf.textBuf = new wchar * [x];
		for (int j = 0; j < x; j++)
			buf.textBuf[j] = new wchar[y];
		buf.colorBuf = new int* [x];
		for (int j = 0; j < x; j++)
			buf.colorBuf[j] = new int[y];

		//������ ��ĭ���� �ʱ�ȭ�մϴ�.
		for(int i = 0; i < x; i++)
			for (int j = 0; j < y; j++)
			{
				buf.textBuf[i][j] = ' ';
				buf.colorBuf[i][j] = 15;
			}

		//��ũ�� ���� HANDLE�� �߱޹޽��ϴ�.
		buf.screen = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleCursorInfo(buf.screen, &cci);
		SetConsoleScreenBufferSize(buf.screen, bufsize);

		DWORD prev_mode;
		GetConsoleMode(buf.screen, &prev_mode);
		SetConsoleMode(buf.screen, prev_mode & ~ENABLE_QUICK_EDIT_MODE);
		SetConsoleMode(buf.screen, ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

		return buf;
	}
	catch (std::bad_alloc) {
		//new�� ������ �����Ͽ����� catch �մϴ�.
		cout << "���� �Ҵ翡 �����Ͽ����ϴ�.";
		throw;
	}
}

int freeBuffer(Buffer buf)
{
	//�����Ҵ� �� �������� ��� ��ȯ�մϴ�.
	for (int i = 0; i < buf.size.x; i++)
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

int drawText(Buffer buf, const wchar* text,int x, int y, int width, int color)
{
	int c = 0;
	int cx = x;
	int cy = y;
	int flag = 0;
	while (text[c] != '\0')
	{
		for (int i = 0; i < width; i++)
		{
			//��ũ�� ������ ������ �Ѿ�ٸ� �÷��� ��ȯ�� �ǳʶٱ��մϴ�.
			if (cx >= buf.size.x || cy >= buf.size.y)
			{
				flag = 1;
			}
			else
			{
				buf.textBuf[cx][cy] = text[c];
				buf.colorBuf[cx][cy] = color;
				if (isWide(text[c]))
				{
					cx++; i++;
				}
			}

			c++;
			cx++;
			if (text[c] == '\0') break;
		}
		cx = x;
		cy++;
	}

	return flag;
}

int resetBuffer(Buffer buf)
{
	for(int i = 0; i<buf.size.x; i++)
		for (int j = 0; j < buf.size.y; j++)
		{
			buf.textBuf[i][j] = ' ';
			buf.colorBuf[i][j] = 15;
		}
	return 0;
}

int syncroBuffer(Buffer buf, Buffer fbuf)
{
	if (buf.size.x != buf.size.x || buf.size.y!= fbuf.size.y)
	{
		cout << "�� ���� ũ�Ⱑ �������� �ʾ� ���۸� ����ȭ �� �� �����ϴ�.";
		throw;
	}
	SMALL_RECT rr = { 0,0,buf.size.x - 1,buf.size.y -1 };
	COORD sz = { buf.size.x, buf.size.y };
	COORD xy = { 0,0 };

	CHAR_INFO* temp;
	temp = new CHAR_INFO[buf.size.x * buf.size.y];

	ReadConsoleOutput(fbuf.screen, temp, sz, xy, &rr);
	WriteConsoleOutput(buf.screen, temp, sz, xy, &rr);

	for(int i = 0; i<buf.size.x; i++)
		for (int j = 0; j < buf.size.y; j++)
		{
			buf.textBuf[i][j] = fbuf.textBuf[i][j];
			buf.colorBuf[i][j] = fbuf.colorBuf[i][j];
		}
	delete temp;

	return 0;
}
