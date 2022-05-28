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

//윈도우 창과 관련된 함수를 모아놓은 파일입니다.
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
		//정적 dll사용시 버그 남, 제거 필요
		SetWindowPos(GetConsoleWindow(), 0, 0, 0, 10000, 10000, SWP_NOMOVE | SWP_SHOWWINDOW);

		SetConsoleWindowInfo(buf.screen, TRUE, &windowSize);
	}
	
	//폰트를 고정할 수 있다는 사실을 일주일 만에 깨달았습니다! 짜잔
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
