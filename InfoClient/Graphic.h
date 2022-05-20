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

#pragma once
#include "Chars.h"
#include <windows.h>

/// <summary>
/// 화면 크기를 저장하기 위한 구조체입니다.
/// <para>x: x방향(가로) 크기</para>
/// <para>y: y방향(세로) 크기</para>
/// </summary>
struct Size
{
	int x;
	int y;
};

/// <summary>
/// 화면 출력 버퍼입니다.
/// <para>더블 버퍼링을 이용하게 될 경우 버퍼 변수 2개를 이용하게 됩니다.</para>
/// <para>screen: 콘솔 출력 핸들러</para>
/// <para>textBuf: 글자 데이터</para>
/// <para>colorBuf: 색깔 데이터</para>
/// <para>size: 화면 크기</para>
/// </summary>
struct Buffer
{
	HANDLE screen;
	wchar** textBuf;
	int** colorBuf;
	Size size;
};

/// <summary>
/// 버퍼 데이터를 이용하여 스크린 버퍼를 렌더링하는 함수입니다.
/// <para>스레드에 넣기 위한 함수입니다.</para>
/// </summary>
/// <param name="buf">렌더링할 버퍼</param>
/// <param name="y_start">렌더링을 시작할 y좌표</param>
/// <param name="y_end">렌더링을 끝낼 y좌표</param>
extern void renderTH(Buffer bbuf, int y_start, int y_end);

/// <summary>
/// 버퍼 데이터를 이용하여 스크린 버퍼를 렌더링하는 함수입니다.
/// <para>프론트 버퍼와 백 버퍼를 비교하여 출력을 최적화합니다. 화면의 변경부분이 적을 경우 매우 큰 성능상의 이점이 있습니다.</para>
/// <para>스레드에 넣기위한 함수입니다.</para>
/// </summary>
/// <param name="bbuf">렌더링할 버퍼</param>
/// <param name="fbuf">비교할 버퍼</param>
/// <param name="y_start">렌더링을 시작할 y좌표</param>
/// <param name="y_end">렌더링을 끝낼 y좌표</param>
extern void renderTH_OP(Buffer bbuf, Buffer fbuf, int y_start, int y_end);

/// <summary>
/// 버퍼의 정보를 이용하여 스크린버퍼를 렌더링합니다.
/// <para>지정한 개수만큼 멀티스레딩을 이용하며, 2개 이상의 스레드 사용시 10% 내외의 성능 향상이 생깁니다.</para>
/// <para>프론트버퍼 없는 백버퍼 렌더링은 잔상제거의 효과 또한 가집니다.</para>
/// </summary>
/// <param name="buf">렌더링할 버퍼</param>
/// <param name="threads">사용할 스레드 수. 반드시 y값의 약수여야 합니다.</param>
/// <returns>성공적으로 렌더링 되었다면 0이 리턴됩니다.</returns>
extern int renderBuffer(Buffer bbuf, int threads);


/// <summary>
/// 버퍼의 정보를 이용하여 스크린버퍼를 렌더링합니다.
/// <para>백버퍼와 프론트버퍼를 비교하여 최적화하며, 화면의 변경 부분이 작다면 매우 큰 성능상의 이점이 존재합니다.</para>
/// <para>지정한 개수만큼 멀티스레딩을 이용하며, 2개 이상의 스레드 이용시 10%내외의 성능 향상이 있습니다.</para>
/// </summary>
/// <param name="buf">렌더링할 버퍼(백 버퍼)</param>
/// <param name="fbuf">비교할 버퍼(프론트 버퍼)</param>
/// <param name="threads">사용할 스레드 수. 반드시 y값의 약수여야 합니다.</param>
/// <returns>성공적으로 렌더링 되었다면 0이 리턴됩니다.</returns>
extern int renderBuffer(Buffer bbuf, Buffer fbuf, int threads);


/// <summary>
/// 버퍼링을 위한 Buffer변수를 동적할당하여 가져옵니다.
/// </summary>
/// <param name="x">스크린 버퍼의 x 사이즈</param>
/// <param name="y">스크린 버퍼의 y 사이즈</param>
/// <returns>할당된 Buffer변수가 반환됩니다.</returns>
extern Buffer getBuffer(int x, int y);

/// <summary>
/// 버퍼를 화면에 표시합니다.
/// </summary>
/// <param name="buf">표시할 버퍼</param>
/// <returns>성공적으로 표시되었다면 0이 리턴됩니다.</returns>
extern int swapBuffer(Buffer buf);

/// <summary>
/// 버퍼링을 위한 Buffer 변수의 동적할당을 해제합니다.
/// </summary>
/// <param name="buf">해제할 Buffer 변수</param>
/// <returns>성공적으로 해제되었다면 0이 반환됩니다.</returns>
extern int freeBuffer(Buffer buf);

/// <summary>
/// 버퍼에 특정 너비의 글을 빌드합니다.
/// </summary>
/// <param name="buf">글을 출력할 버퍼</param>
/// <param name="text">출력할 텍스트, 한글에 간격을 적용하지 않습니다.</param>
/// <param name="x">출력을 시작할 x좌표</param>
/// <param name="y">출력을 시작할 y좌표</param>
/// <param name="width">텍스트의 너비</param>
/// <param name="color">텍스트의 색깔</param>
/// <returns>성공적으로 빌드했다면 0이 반환됩니다.
/// <para>텍스트가 범위를 넘어갔다면 1이 반환됩니다.</para></returns>
extern int drawText(Buffer buf, const wchar* text, int x, int y, int width, int color);

/// <summary>
/// 버퍼의 데이터를 리셋합니다.
/// <para>스크린 버퍼는 리셋하지 않습니다.</para>
/// </summary>
/// <param name="bbuf">리셋할 버퍼</param>
/// <returns>정상적으로 리셋되면 0이 반환됩니다. </returns>
extern int resetBuffer(Buffer bbuf);


/// <summary>
/// 두 버퍼를 동기화합니다. 더블 버퍼링 과정에 필수적입니다.
/// </summary>
/// <param name="client">복사될 버퍼</param>
/// <param name="server">복사할 버퍼</param>
/// <returns>정상적으로 동기화되면 0이 반환됩니다.</returns>
extern int syncroBuffer(Buffer client, Buffer server);
