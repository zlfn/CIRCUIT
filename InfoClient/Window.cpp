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
#include "Graphic.h"

/// <summary>
/// 스크린 버퍼를 버퍼크기에 맞춥니다.
/// </summary>
/// <param name="buf">크기를 조정할 스크린 버퍼</param>
/// <param name="x">x방향 크기</param>
/// <param name="y">y방향 크기</param>
/// <param name="noSpace">여백없는 출력여부 (릴리스에서 문제가 발생하였음)</param>
/// <returns>정상적으로 조절되었다면 0이 리턴됩니다.</returns>
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

	//SetConsoleWIndowInfo는 콘솔창을 줄일수는 있지만 늘릴수는 없습니다.
	//만약 콘솔창이 목표 크기보다 작다면 SetWindowPos를 이용해 강제로 늘립니다.
	if (SetConsoleWindowInfo(buf.screen, TRUE, &windowSize) == 0)
	{
		SetWindowPos(GetConsoleWindow(), 0, 0, 0, 10000, 10000, SWP_NOMOVE | SWP_SHOWWINDOW);
		SetConsoleWindowInfo(buf.screen, TRUE, &windowSize); //<재시도
	}
	SetConsoleCursorInfo(buf.screen, &cci);
	return 0;
}
