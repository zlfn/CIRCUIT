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
/// 윈도우 가상 키코드 상수입니다.
/// </summary>
namespace Key
{
	const int kBACKSPACE = 0x08;
	const int kTAB = 0x09;
	const int kEnter = 0x0D;
	const int kShift = 0x10;
	const int kCTRL = 0x11;
	const int kALT = 0x12;
	const int kCAPSLOCK = 0x14;
	const int kESC = 0x1B;
	const int kSPACE = 0x20;

	const int kLEFT = 0x25;
	const int kUP = 0x26;
	const int kRIGHT = 0x27;
	const int kDOWN = 0x27;

	const int kINSERT = 0x2D;
	const int kDELETE = 0x2E;

	const int k0 = 0x30;
	const int k1 = 0x31;
	const int k2 = 0x32;
	const int k3 = 0x33;
	const int k4 = 0x34;
	const int k5 = 0x35;
	const int k6 = 0x36;
	const int k7 = 0x37;
	const int k8 = 0x38;
	const int k9 = 0x39;

	const int kA = 0x41;
	const int kB = 0x42;
	const int kC = 0x43;
	const int kD = 0x44;
	const int kE = 0x45;
	const int kF = 0x46;
	const int kG = 0x47;
	const int kH = 0x48;
	const int kI = 0x49;
	const int kJ = 0x4A;
	const int kK = 0x4B;
	const int kL = 0x4C;
	const int kM = 0x4D;
	const int kN = 0x4E;
	const int kO = 0x4F;
	const int kP = 0x50;
	const int kQ = 0x51;
	const int kR = 0x52;
	const int kS = 0x53;
	const int kT = 0x54;
	const int kU = 0x55;
	const int kV = 0x56;
	const int kW = 0x57;
	const int kX = 0x58;
	const int kY = 0x59;
	const int kZ = 0x5A;
}

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
static void getInputTH(MouseClick* msPointer, int* keyPointer);

/// <summary>
/// <para>마우스 입력을 받는 스레드를 생성합니다.</para>
/// <para>스레드는 detach되어 프로그램 종료시까지 유지됩니다.</para>
/// <para>한 프로그램에서 두번 이상 이 함수가 실행되면 에러가 발생할 수 있습니다.</para>
/// </summary>
/// <returns>정상적으로 수행되었다면 0이 반환됩니다.</returns>
extern int startGetInput();

/// <summary>
/// 마우스 입력 값을 받아옵니다.
/// <para>startGetClick()이 반드시 선행되어야합니다.</para>
/// </summary>
/// <returns>마우스 입력 값</returns>
extern MouseClick getClick();

/// <summary>
/// 마우스 입력 값을 받아오고, 입력값을 삭제합니다.
/// <para>한 사이클에 한번 씩만 유효한 값이 반환됩니다.</para>
/// </summary>
/// <returns>마우스 입력 값</returns>
extern MouseClick getClickOnce();

/// <summary>
/// 눌린 키의 키 코드를 받아옵니다.
/// </summary>
/// <returns>키 코드</returns>
extern int getKey();

/// <summary>
/// 클릭한 좌표의 클릭 버퍼 id를 받아옵니다.
/// 이를 통해서 클릭한 오브젝트를 얻어낼 수 있습니다.
/// </summary>
/// <param name="buf">참조할 클릭버퍼를 담은 버퍼</param>
/// <returns>정상적으로 실행되었다면 버퍼의 클릭 버퍼 id가 반환됩니다.
/// <para>입력 좌표가 버퍼범위를 넘어선다면 -1이 반환됩니다.</para></returns>
extern int getClickObject(Buffer buf);
