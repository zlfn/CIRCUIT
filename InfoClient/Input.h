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

//사용자 입력과 관련된 내용을 담은 헤더파일입니다.
/* 다들 콘솔에서 메뉴선택을 방향키나 뭐 그런걸 가지고 입력하는데, 
* 제 키보드 (해피해킹)에는 방향키가 없습니다. 그래서 싫어요.
* 그래서 WindowsAPI를 이용해서 마우스 클릭을 받기로 했습니다.
* 근데 분명 입력버퍼 방식으로 받는 거면 대충 메인 루프 안에 넣어도 연속적인 마우스 입력이 가능해야 할거 같은데,
* 요상하게도 그러지 않더라고요. 그래서 마우스 입력만 무한정 받는 스레드를 만들어서 detach() 하는 방식으로 마우스 클릭을 받습니다.
*/

#pragma once
#include <Windows.h>
#include "Graphic.h"

/// <summary>
/// 마우스 클릭의 종류를 핸들하는 열거형입니다.
/// <para>None: 클릭이 없음</para>
/// <para>Left: 왼쪽 클릭</para>
/// <para>Right: 오른쪽 클릭</para>
/// </summary>
enum MouseClickType
{
	None,
	Left,
	Right
};

/// <summary>
/// 마우스 클릭입력을 보관하는 구조체입니다.
/// <para>pos: 마우스 클릭 좌표</para>
/// <para>MouseClickType: 마우스 클릭의 종류</para>
/// </summary>
struct MouseClick
{
	COORD pos;
	MouseClickType type;
};

/// <summary>
/// <para>입력정보를 가져와서 포인터에 등록하는 것을 무한 반복합니다.</para>
/// <para>스레드에 넣기 위한 함수입니다.</para>
/// </summary>
/// <param name="msPointer">입력을 집어넣을 MouseClick 포인터</param>
static void getClickTH(MouseClick* msPointer);

/// <summary>
/// <para>마우스 입력을 받는 스레드를 생성합니다.</para>
/// <para>스레드는 detach되어 프로그램 종료시까지 유지됩니다.</para>
/// <para>한 프로그램에서 두번 이상 이 함수가 실행되면 에러가 발생할 수 있습니다.</para>
/// </summary>
/// <returns>정상적으로 수행되었다면 0이 반환됩니다.</returns>
extern int startGetClick();

/// <summary>
/// 마우스 입력 값을 받아옵니다.
/// <para>startGetClick()이 반드시 선행되어야합니다.</para>
/// </summary>
/// <returns>마우스 입력 값</returns>
extern MouseClick getClick();
