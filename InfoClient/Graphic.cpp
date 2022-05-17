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
/*그래픽 오브젝트를 모아서 더블버퍼링 기법으로 출력합니다.
사용되는 렌더링 기법에 대한 설명이 아래에 있습니다.*/

//Graphic의 렌더링 순서
/* 1. 매 프레임마다 게임 진행 코드로부터 렌더링 되어야 할 GraphicObject를 입력받습니다.
*  2. Object목록을 바탕으로 버퍼에 그래픽 데이터를 빌드합니다.
*  3. 버퍼의 그래픽 데이터를 바탕으로 스크린 버퍼를 렌더링합니다.
*  4. 렌더링된 스크린 버퍼를 현재 버퍼와 바꿉니다.
* 
* 상기의 과정을 통해 다루기 쉬우면서도 깜박임 현상이 없는 콘솔 그래픽을 표시할 수 있습니다.
*/

#include "Chars.h"
#include <conio.h>
#include <windows.h>
#include <iostream>
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
/// 버퍼의 정보를 이용하여 스크린버퍼를 렌더링합니다.
/// </summary>
/// <param name="buf">렌더링할 버퍼</param>
/// <param name="x">스크린버퍼의 x 사이즈</param>
/// <param name="y">스크린버퍼의 y 사이즈</param>
/// <returns>성공적으로 렌더링 되었다면 0이 리턴됩니다.</returns>
int renderBuffer(Buffer buf,int x, int y)
{
	//커서 위치
	COORD cur;
	cur.X = 0;
	cur.Y = 0;
	
	//한줄 씩 렌더링 합니다.
	for (int i = 0; i < y; i++)
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
			} //<한글은 끔찍하게도 콘솔창 두칸을 차지합니다.
			else cur.X += 1; //<영문과 아스키코드는 콘솔창 한칸을 차지합니다.
		}
	}
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

		//스크린 버퍼 HANDLE을 발급받습니다.
		buf.screen = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleCursorInfo(buf.screen, &cci);

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