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


#include <vector>
#include <iostream>
#include <windows.h>
#include <thread>
#include "Chars.h"
#include "Graphic.h"
#include "Window.h"
using namespace std;


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


//�Լ� �Ķ���� �����ε��ϰ� �;��µ� �����忡 ������ ������ ���� ��¿ �� ���� �и��߽��ϴ�.
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

int refreshBuffer(Buffer buf, int threadSize)
{
	//��ü y�ټ��� ������ ������ŭ ������ ������ �����忡 �й��մϴ�.
	if (buf.size.y % threadSize != 0)
	{
		//�� �������� ����
		cout << "ERR: ������ ������ ������ �������մϴ�.";
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
		cout << "ERR: ������ ������ ������ �������մϴ�.";
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
		cout << "ERR: ���� �Ҵ翡 �����Ͽ����ϴ�.";
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
			if (cx >= buf.size.x || cy >= buf.size.y || 0 > cx || 0> cy)
			{
				flag = 1;
			}
			else
			{
				//Color::Black�� ����ä���� ����� �մϴ�.
				if (buf.colorBuf[cx][cy] != Color::Black)
				{
					buf.textBuf[cx][cy] = text[c];
					buf.colorBuf[cx][cy] = color;
				}
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

int resetScreen(Buffer buf)
{
	unsigned long dw;
	FillConsoleOutputCharacter(buf.screen, ' ', buf.size.x * buf.size.y, { 0,0 }, &dw);
	return 0;
}

int syncroBuffer(Buffer buf, Buffer fbuf)
{
	if (buf.size.x != fbuf.size.x || buf.size.y!= fbuf.size.y)
	{
		cout << "ERR: �� ���� ũ�Ⱑ �������� �ʾ� ���۸� ����ȭ �� �� �����ϴ�.";
		throw;
	}
	SMALL_RECT rr = { 0,0,buf.size.x - 1,buf.size.y -1 };
	COORD sz = {(SHORT)buf.size.x, (SHORT)buf.size.y };
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

int drawImage(Buffer buf, const wchar* path, int x, int y)
{
	int flag = 0;


	FILE* fs;
	_wfopen_s(&fs, path, L"r,ccs=UNICODE"); //<�����ڵ� �б� ���� ����
	if (fs == NULL)
	{
		wcout << "ERR: " << path << " ���ҽ� ������ ã�� �� �����ϴ�.";
		throw;
	}

	int sizeX, sizeY;
	fwscanf_s(fs, L"%d %d\n", &sizeX, &sizeY);

	//������ �������� Ȯ���ϴ� �ӽ� �迭
	bool** alphaChecker;
	alphaChecker = new bool* [sizeY];
	for (int i = 0; i < sizeY; i++)
	{
		alphaChecker[i] = new bool[sizeX];
		for (int j = 0; j < sizeX; j++)
			alphaChecker[i][j] = false;
	}

	for (int i = 0; i < sizeY; i++)
	{
		for (int j = 0; j < sizeX; j++)
		{
			wchar k;
			fwscanf_s(fs, L"%c", &k);
			if (j+x >= buf.size.x || i+y >= buf.size.y || 0 > j+x || 0 > i+y)
			{
				flag = 1;
			}
			else
			{
				int n;
				switch (k)
				{
					case L'0': n = 0; break;
					case L'1': n = 1; break;
					case L'2': n = 2; break;
					case L'3': n = 3; break;
					case L'4': n = 4; break;
					case L'5': n = 5; break;
					case L'6': n = 6; break;
					case L'7': n = 7; break;
					case L'8': n = 8; break;
					case L'9': n = 9; break;
					case L'A': n = 10; break;
					case L'B': n = 11; break;
					case L'C': n = 12; break;
					case L'D': n = 13; break;
					case L'E': n = 14; break;
					case L'F': n = 15; break;
				}

				if (n != Color::Black) { buf.colorBuf[j+x][i+y] = n; }
				else { alphaChecker[i][j] = true; }
			}
		}
		fwscanf_s(fs, L"\n");
	}

	for (int i = 0; i < sizeY; i++)
	{
		for (int j = 0; j < sizeX; j++)
		{
			char k;
			fwscanf_s(fs, L"%c", &k);
			if (j+x >= buf.size.x || i+y >= buf.size.y || 0 > j+x || 0 > i+y)
			{
				flag = 1;
			}
			else
			{
				int n;
				switch (k)
				{
					case '0': n = 0; break;
					case '1': n = 1; break;
					case '2': n = 2; break;
					case '3': n = 3; break;
					case '4': n = 4; break;
					case '5': n = 5; break;
					case '6': n = 6; break;
					case '7': n = 7; break;
					case '8': n = 8; break;
					case '9': n = 9; break;
					case 'A': n = 10; break;
					case 'B': n = 11; break;
					case 'C': n = 12; break;
					case 'D': n = 13; break;
					case 'E': n = 14; break;
					case 'F': n = 15; break;
				}

				if(!alphaChecker[i][j]) buf.colorBuf[j+x][i+y] += n*16;
			}
		}
		fwscanf_s(fs, L"\n");
	}

	for (int i = 0; i < sizeY; i++)
	{
		for (int j = 0; j < sizeX; j++)
		{
			wchar c; fwscanf_s(fs, L"%c", &c);
			if (j+x >= buf.size.x || i+y >= buf.size.y || 0 > j+x || 0> i+y)
			{
				flag = 1;
			}
			else
			{
				//������ (����)�� ���������� �ʽ��ϴ�.
				if (!alphaChecker[i][j]) buf.textBuf[j+x][i+y] = c;
				if (isWide(c)) j++;
			}
		}
		fwscanf_s(fs, L"\n");
	}

	fclose(fs);
	delete alphaChecker;
	return flag;
}
