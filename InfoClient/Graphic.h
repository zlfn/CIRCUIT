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

//그래픽 관련 함수를 모아놓은 헤더입니다.
/*이 게임은 cout, printf를 비롯한 표준 입출력을 사용하지 않습니다.
대신,WindowsAPI를 적극 활용하여 TUI 그래픽을 더블버퍼링 기법으로 출력합니다.
사용되는 렌더링 기법에 대한 설명이 아래에 있습니다.*/

//게임의 렌더링 순서
/* 게임에는 데이터 버퍼 2개와 스크린 버퍼 2개가 있습니다. D1, D2, S1, S2로 부릅시다.
* 데이터 버퍼는 화면에 표시될 텍스트와 색을 임시로 담은 정수 배열이고, 스크린 버퍼는 실제로 출력될 수 있는 WindowsAPI HANDLE입니다.
*
* 1. 가장 처음에는, D1, D2, S1, S2가 getBuffer()에 의해 초기화되어 모두 검은화면입니다.
* 2. D1에 화면에 표시될 텍스트를 빌드합니다. 프로그램 내부 정수배열에 접근하는 것이므로 빠른 속도로 수행할 수 있습니다.
* 3. D2와 D1을 비교하여 다른 부분만을 S1에 그립니다. 비교과정은 신속하지만, 그리는 과정은 콘솔에 한칸한칸 찍으므로 상당히 느립니다.
* 4. S1을 화면에 출력합니다. 이미 모두 그려져 있으므로 티어링이나 깜박임이 발생하지 않습니다.
* 5. D1을 D2에 복사, S1을 S2에 복사합니다.
* 6. D1과 D2, S1과 S2를 바꿔 2-5를 시행합니다. 이후 무한반복.
*
* 이 과정이 게임내에서 1프레임입니다. 변경부분이 적을때는 초당 1000프레임 이상, 
* 클 때에는 초당 10프레임 내외로 렌더링됩니다.
* 
* 이때 문제가 생깁니다. 멀티스레딩 버그와 창 크기 조절에 의한 글자 깨짐, 설계상의 실수 등으로 인해 
* 간혹 게임 진행 중 D1과 D2의 내용과 S1과 S2의 내용이 다른 일명 '잔상'이 생깁니다.
* 잔상이 생기면 D1과 D2의 데이터는 서로 같기 때문에 렌더링을 하더라도 S1과 S2에는 변화가 없습니다.
* 따라서 주기적으로 데이터 버퍼의 내용을 서로 비교하지 않고 스크린 버퍼에 그리는 '리프레시'가 필요합니다.
* 리프레시는 약 0.1초가 소요되며, 이를 메인 함수에서 실행하게 되면 리프레시할때마다 프레임드랍이 발생하므로,
* 실행시 스레드를 detach() 할 수 있습니다. 리프레시가 느려지지만 프레임드랍이 꽤 많이 줄어듭니다.
*
* 추가로, 리프레시 대신 게임 개발 과정 중에 개발된 잔상 제거 기법인 프론트 버퍼 디더링을 이용할 수 있습니다.
* 인위적으로 프론트버퍼에 랜덤한 노이즈를 만듬으로써 매 프레임마다 잔상제거가 이루어집니다.
* 단, 평균 프레임이 낮을 때는 잔상제거가 상당히 느리게 이루어지며, 평균 프레임 또한 낮아집니다.
* 
* 상기의 과정을 통해 빠르면서도 깜박임 현상이 없는 콘솔 그래픽을 표시할 수 있습니다.
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
	int** clickBuf;
	Size size;
};

/// <summary>
/// 버퍼 데이터를 이용하여 스크린 버퍼를 렌더링하는 함수입니다.
/// <para>스레드에 넣기 위한 함수입니다.</para>
/// </summary>
/// <param name="buf">렌더링할 버퍼</param>
/// <param name="y_start">렌더링을 시작할 y좌표</param>
/// <param name="y_end">렌더링을 끝낼 y좌표</param>
static void renderTH(Buffer bbuf, int y_start, int y_end);

/// <summary>
/// 버퍼 데이터를 이용하여 스크린 버퍼를 렌더링하는 함수입니다.
/// <para>프론트 버퍼와 백 버퍼를 비교하여 출력을 최적화합니다. 화면의 변경부분이 적을 경우 매우 큰 성능상의 이점이 있습니다.</para>
/// <para>스레드에 넣기위한 함수입니다.</para>
/// </summary>
/// <param name="bbuf">렌더링할 버퍼</param>
/// <param name="fbuf">비교할 버퍼</param>
/// <param name="y_start">렌더링을 시작할 y좌표</param>
/// <param name="y_end">렌더링을 끝낼 y좌표</param>
static void renderTH_OP(Buffer bbuf, Buffer fbuf, int y_start, int y_end);

/// <summary>
/// 버퍼의 정보를 이용하여 스크린버퍼를 리프레시합니다.
/// <para>지정한 개수만큼 멀티스레딩을 이용하며, 2개 이상의 스레드 사용시 10% 내외의 성능 향상이 생깁니다.</para>
/// </summary>
/// <param name="bbuf">렌더링할 버퍼</param>
/// <param name="threads">사용할 스레드 수. 반드시 y값의 약수여야 합니다.</param>
/// <returns>성공적으로 렌더링 되었다면 0이 리턴됩니다.</returns>
extern int refreshBuffer(Buffer bbuf, int threads);


/// <summary>
/// 버퍼의 정보를 이용하여 스크린버퍼를 렌더링합니다.
/// <para>백버퍼와 프론트버퍼를 비교하여 최적화하며, 화면의 변경 부분이 작다면 매우 큰 성능상의 이점이 존재합니다.</para>
/// <para>지정한 개수만큼 멀티스레딩을 이용하며, 2개 이상의 스레드 이용시 10%내외의 성능 향상이 있습니다.</para>
/// </summary>
/// <param name="bbuf">렌더링할 버퍼(백 버퍼)</param>
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
/// 버퍼의 스크린버퍼를 리셋합니다.
/// </summary>
/// <param name="buf">리셋할 버퍼</param>
/// <returns>정상적으로 리셋되면 0이 반환됩니다. </returns>
extern int resetScreen(Buffer buf);

/// <summary>
/// 두 버퍼를 동기화합니다. 더블 버퍼링 과정에 필수적입니다.
/// </summary>
/// <param name="client">복사될 버퍼</param>
/// <param name="server">복사할 버퍼</param>
/// <returns>정상적으로 동기화되면 0이 반환됩니다.</returns>
extern int syncroBuffer(Buffer client, Buffer server);


//그래픽 리소스(gres)의 포멧은 아래와 같습니다.
/* 첫줄에는 x방향 크기와 y방향 크기가 주어집니다.
두번째 줄부터 1+y 줄만큼은 색깔 정보가 주어집니다. 16진수 0-E까지이며, 0은 투명을 뜻합니다.
2+y줄부터 1+2y 줄 만큼은 텍스트 정보가 주어집니다. WISYWIG방식으로, 글자 크기와 관계없이 보이는 그대로 출력됩니다.*/

/// <summary>
/// gres 포멧으로 작성된 그래픽 리소스를 불러와서 화면에 출력합니다.
/// </summary>
/// <param name="buf">출력할 버퍼</param>
/// <param name="path">그래픽 리소스 경로</param>
/// <param name="x">출력을 시작할 x좌표</param>
/// <param name="y">출력을 시작할 y좌표</param>
/// <returns>성공적으로 출력되었다면 0,
/// <para>이미지가 화면을 넘어갔다면 1이 반환됩니다.</para></returns>
extern int drawImage(Buffer buf, const wchar* path, int x, int y);

/// <summary>
/// gres 포멧으로 작성된 그래픽 리소스를 불러와서 화면에 출력합니다.
/// <para>추가로, 주어진 아이디를 클릭버퍼에 등록합니다.</para>
/// </summary>
/// <param name="buf">출력할 버퍼</param>
/// <param name="path">그래픽 리소스 경로</param>
/// <param name="x">출력을 시작할 x좌표</param>
/// <param name="y">출력을 시작할 y좌표</param>
/// <param name="click">등록할 클릭 버퍼 id</param>
/// <returns>성공적으로 출력되었다면 0,
/// <para>이미지가 화면을 넘어갔다면 1이 반환됩니다.</para></returns>
extern int drawImage(Buffer buf, const wchar* path, int x, int y, int click);

/// <summary>
/// 클릭버퍼의 내용을 초기값으로 되돌립니다.
/// <para>클릭버퍼의 초기값은 0입니다.</para>
/// </summary>
/// <param name="buf">리셋할 버퍼</param>
/// <returns>정상적으로 리셋되었다면 0이 반환됩니다.</returns>
extern int resetClickBuffer(Buffer buf);

/// <summary>
/// 버퍼를 디더링합니다.
/// </summary>
/// <param name="buf">디더링할 버퍼 (프론트 버퍼)</param>
/// <param name="ditherFactor">디더링할 양</param>
/// <returns>정상적으로 디더링되면 0이 반환됩니다.</returns>
extern int ditherBuffer(Buffer buf, int ditherFactor);
