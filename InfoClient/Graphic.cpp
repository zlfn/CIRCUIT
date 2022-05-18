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

//그래픽 관련 함수를 모아놓은 파일입니다.
/*이 게임은 cout, printf를 비롯한 표준 입출력을 사용하지 않습니다.
대신,WindowsAPI를 적극 활용하여 TUI 그래픽을 더블버퍼링 기법으로 출력합니다.
사용되는 렌더링 기법에 대한 설명이 아래에 있습니다.*/

//게임의 렌더링 순서
/* 1. 매 프레임마다 게임 진행 코드로부터 렌더링 되어야 할 스프라이트를 입력받습니다.
*  2. 각각의 스프라이트의 그래픽 데이터를 바탕으로 버퍼에 그래픽 데이터를 빌드합니다.
*  3. 버퍼의 그래픽 데이터를 바탕으로 스크린 버퍼를 렌더링합니다.
*  4. 렌더링된 스크린 버퍼를 현재 버퍼와 바꿉니다.
* 
* 상기의 과정을 통해 다루기 쉬우면서도 깜박임 현상이 없는 콘솔 그래픽을 표시할 수 있습니다.
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
/// 버퍼를 화면에 표시합니다.
/// </summary>
/// <param name="bbuf">표시할 버퍼</param>
/// <returns>성공적으로 표시되었다면 0이 리턴됩니다.</returns>
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

/// <summary>
/// 버퍼 데이터를 이용하여 스크린 버퍼를 렌더링하는 함수입니다.
/// 멀티스레딩을 이용할경우 10% 내외의 성능 향상이 있습니다.
/// </summary>
/// <param name="buf">렌더링할 버퍼</param>
/// <param name="x">버퍼의 x크기</param>
/// <param name="y_start">렌더링을 시작할 y좌표</param>
/// <param name="y_end">렌더링을 끝낼 y좌표</param>
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
			letter.Char.UnicodeChar = buf.textBuf[j][i]; //< X,Y 방향을 맞추기 위해 i,j를 뒤집습니다.
			SetConsoleTextAttribute(buf.screen, buf.colorBuf[j][i]);
			SetConsoleCursorPosition(buf.screen, cur);
			DWORD ws;
			WriteConsole(buf.screen, &letter, 1, &ws, NULL);

			if (isWide(buf.textBuf[j][i])) {
				j++; cur.X += 2;
			} //<한글은 끔찍하게도 콘솔창 두칸을 차지합니다. (한글이 나오면 다음 Textbuf는 무시
			else cur.X += 1; //<영문과 아스키코드는 콘솔창 한칸을 차지합니다.
		}
	}
	return;
}

/// <summary>
/// 버퍼의 정보를 이용하여 스크린버퍼를 렌더링합니다.
/// 이때 멀티스레딩을 이용합니다.
/// </summary>
/// <param name="buf">렌더링할 버퍼</param>
/// <param name="x">스크린버퍼의 x 사이즈</param>
/// <param name="y">스크린버퍼의 y 사이즈</param>
/// <param name="threadSize">사용할 스레드 수. 반드시 y값의 약수여야 합니다.</param>
/// <returns>성공적으로 렌더링 되었다면 0이 리턴됩니다.</returns>
int renderBuffer(Buffer buf,int x, int y, int threadSize)
{
	//전체 y줄수를 스레드 개수만큼 나눠서 각각의 스레드에 분배합니다.
	if (y % threadSize != 0)
	{
		//안 나눠지면 뻗음
		cout << "렌더링 스레드 개수가 부적절합니다.";
		throw;
	}
	vector<thread> threads;
	for (int i = 0; i < threadSize; i++)
		threads.push_back(thread(renderTH,buf, x, (y / threadSize) * i, (y / threadSize) * (i + 1)));

	//스레드의 작업이 끝나는 것을 기다..려야 하는데 안기다립니다. 좀 이상함
	for (int i = 0; i < threadSize; i++)
		threads[i].join();
	return 0;
}


/// <summary>
/// 버퍼링을 위한 Buffer변수를 동적할당하여 가져옵니다.
/// </summary>
/// <param name="x">스크린 버퍼의 x 사이즈</param>
/// <param name="y">스크린 버퍼의 y 사이즈</param>
/// <returns>할당된 Buffer변수가 반환됩니다.</returns>
Buffer getBuffer(int x, int y)
{
	//커서를 보이지 않게 합니다.
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 1;
	cci.bVisible = FALSE;

	COORD bufsize = { x,y };

	Buffer buf;
	try
	{
		//그래픽 데이터 2차원 배열을 동적할당합니다.
		buf.textBuf = new wchar*[x];
		for (int i = 0; i < x; i++)
		buf.textBuf[i] = new wchar[y];
		buf.colorBuf = new int*[x];
		for (int i = 0; i < x; i++)
		buf.colorBuf[i] = new int[y];

		//검은색 빈칸으로 초기화합니다.
		for(int i = 0; i<x; i++)
			for (int j = 0; j < y; j++)
			{
				buf.textBuf[i][j] = ' ';
				buf.colorBuf[i][j] = 15;
			}

		//스크린 버퍼 HANDLE을 발급받습니다.
		buf.screen = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleCursorInfo(buf.screen, &cci);
		SetConsoleScreenBufferSize(buf.screen, bufsize);

		return buf;
	}
	catch (std::bad_alloc) {
		//new의 실행이 실패하였을때 catch 합니다.
		cout << "버퍼 할당에 실패하였습니다.";
		throw;
	}
}

/// <summary>
/// 버퍼링을 위한 Buffer변수를 해제합니다.
/// </summary>
/// <param name="buf">해제할 Buffer변수</param>
/// <param name="x">스크린 버퍼의 x방향 사이즈</param>
/// <param name="y">스크린 버퍼의 y방향 사이즈</param>
/// <returns>성공적으로 해제되었다면 0이 리턴됩니다.</returns>
int freeBuffer(Buffer buf,int x, int y)
{
	//동적할당 한 변수들을 모두 반환합니다.
	for (int i = 0; i < x; i++)
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

/// <summary>
/// 버퍼에 특정 너비의 글을 빌드합니다.
/// </summary>
/// <param name="buf">글을 출력할 버퍼</param>
/// <param name="text">출력할 텍스트, 한글에 간격을 적용하지 않습니다.</param>
/// <param name="x">출력을 시작할 x좌표</param>
/// <param name="y">출력을 시작할 y좌표</param>
/// <param name="width">텍스트의 너비</param>
/// <param name="color">텍스트의 색깔</param>
/// <returns>성공적으로 빌드했다면 0이 반환됩니다.</returns>
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
/// 버퍼의 데이터를 리셋합니다.
/// 스크린 버퍼는 리셋하지 않습니다.
/// </summary>
/// <param name="buf">리셋할 버퍼</param>
/// <param name="x">버퍼의 x방향 사이즈</param>
/// <param name="y">버퍼의 y방향 사이즈</param>
/// <returns>정상적으로 리셋되면 0이 출력됩니다. </returns>
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