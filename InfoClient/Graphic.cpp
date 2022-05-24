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
	//SetConsoleActiveScreenBuffer함수는 정상적으로 실행되지 않았을 때 0을 리턴합니다. 이를 감지하여 예외처리합니다.
	if (SetConsoleActiveScreenBuffer(bbuf.screen) != 0) return 0;
	else
	{
		cout << "ERR: 스크린 버퍼 표시에 실패하였습니다. 예상치 못한 지점에서 버퍼가 해제되었을 가능성이 있습니다.";
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
			letter.Char.UnicodeChar = buf.textBuf[j][i]; //< X,Y 방향을 맞추기 위해 i,j를 뒤집습니다.
			SetConsoleTextAttribute(buf.screen, buf.colorBuf[j][i]);
			SetConsoleCursorPosition(buf.screen, cur);
			DWORD ws;
			WriteConsole(buf.screen, &letter, 1, &ws, NULL);
			if (isWide(buf.textBuf[j][i])) {
				j++; cur.X += 2;
			} //<한글은 끔찍하게도 콘솔창 두칸을 차지합니다. (한글이 나오면 다음 Textbuf는 무시)
			else cur.X += 1; //<영문과 아스키코드는 콘솔창 한칸을 차지합니다.
		}
	}

	Sleep(0);
	return;
}


//함수 파라미터 오버로딩하고 싶었는데 스레드에 넣을때 에러가 나서 어쩔 수 없이 분리했습니다.
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
				letter.Char.UnicodeChar = buf.textBuf[j][i]; //< X,Y 방향을 맞추기 위해 i,j를 뒤집습니다.
				SetConsoleTextAttribute(buf.screen, buf.colorBuf[j][i]);
				SetConsoleCursorPosition(buf.screen, cur);
				DWORD ws;
				WriteConsole(buf.screen, &letter, 1, &ws, NULL);
			}
			if (isWide(buf.textBuf[j][i])) {
				j++; cur.X += 2;
			} //<한글은 끔찍하게도 콘솔창 두칸을 차지합니다. (한글이 나오면 다음 Textbuf는 무시)
			else cur.X += 1; //<영문과 아스키코드는 콘솔창 한칸을 차지합니다.
		}
	}

	Sleep(0);
	return;
}

int refreshBuffer(Buffer buf, int threadSize)
{
	//전체 y줄수를 스레드 개수만큼 나눠서 각각의 스레드에 분배합니다.
	if (buf.size.y % threadSize != 0)
	{
		//안 나눠지면 뻗음
		cout << "ERR: 렌더링 스레드 개수가 부적절합니다.";
		throw;
	}
	vector<thread> threads;
	for (int i = 0; i < threadSize; i++)
		threads.push_back(thread(renderTH, buf, (buf.size.y / threadSize) * i, (buf.size.y / threadSize) * (i + 1)));

	//스레드의 작업이 끝나는 것을 기다..려야 하는데 안기다립니다. 좀 이상함
	for (int i = 0; i < threadSize; i++)
		threads[i].join();
	return 0;
}

int renderBuffer(Buffer buf, Buffer bbuf, int threadSize)
{
	//전체 y줄수를 스레드 개수만큼 나눠서 각각의 스레드에 분배합니다.
	if (buf.size.y % threadSize != 0)
	{
		//안 나눠지면 뻗음
		cout << "ERR: 렌더링 스레드 개수가 부적절합니다.";
		throw;
	}
	vector<thread> threads;
	for (int i = 0; i < threadSize; i++)
		threads.push_back(thread(renderTH_OP, buf, bbuf, (buf.size.y / threadSize) * i, (buf.size.y / threadSize) * (i + 1)));

	//스레드의 작업이 끝나는 것을 기다..려야 하는데 안기다립니다. 좀 이상함
	for (int i = 0; i < threadSize; i++)
		threads[i].join();
	return 0;
}

Buffer getBuffer(int x, int y)
{
	//커서를 보이지 않게 합니다.
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;

	COORD bufsize = { x,y };

	Buffer buf;
	buf.size.x = x;
	buf.size.y = y;
	try
	{
		//그래픽 데이터 2차원 배열을 동적할당합니다.
		buf.textBuf = new wchar * [x];
		for (int j = 0; j < x; j++)
			buf.textBuf[j] = new wchar[y];
		buf.colorBuf = new int* [x];
		for (int j = 0; j < x; j++)
			buf.colorBuf[j] = new int[y];

		//검은색 빈칸으로 초기화합니다.
		for(int i = 0; i < x; i++)
			for (int j = 0; j < y; j++)
			{
				buf.textBuf[i][j] = ' ';
				buf.colorBuf[i][j] = 15;
			}

		//스크린 버퍼 HANDLE을 발급받습니다.
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
		//new의 실행이 실패하였을때 catch 합니다.
		cout << "ERR: 버퍼 할당에 실패하였습니다.";
		throw;
	}
}

int freeBuffer(Buffer buf)
{
	//동적할당 한 변수들을 모두 반환합니다.
	for (int i = 0; i < buf.size.x; i++)
	{
		delete buf.textBuf[i];
		delete buf.colorBuf[i];
	}
	delete buf.textBuf;
	delete buf.colorBuf;
	
	//스크린버퍼 핸들을 닫아줍니다.
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
			//스크린 버퍼의 범위를 넘어갔다면 플래그 전환후 건너뛰기합니다.
			if (cx >= buf.size.x || cy >= buf.size.y || 0 > cx || 0> cy)
			{
				flag = 1;
			}
			else
			{
				//Color::Black은 알파채널의 기능을 합니다.
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
		cout << "ERR: 두 버퍼 크기가 동일하지 않아 버퍼를 동기화 할 수 없습니다.";
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
	_wfopen_s(&fs, path, L"r,ccs=UNICODE"); //<유니코드 읽기 모드로 열기
	if (fs == NULL)
	{
		wcout << "ERR: " << path << " 리소스 파일을 찾을 수 없습니다.";
		throw;
	}

	int sizeX, sizeY;
	fwscanf_s(fs, L"%d %d\n", &sizeX, &sizeY);

	//색깔이 투명인지 확인하는 임시 배열
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
				//검은색 (투명)은 렌더링하지 않습니다.
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
