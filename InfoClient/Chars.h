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

//wchar를 비롯한 문자와 관련된 내용을 모아둔 헤더파일입니다.
/* 터미널에서 한글 출력은 가능하지만, char 변수는 좁디 좁기 때문에 한글은 커녕 텍스트 이모티콘도 표시하기 어렵습니다.
* 하지만 이 게임의 렌더링 방식 특성상 (Graphics.h 참고) 반드시 한글을 변수로써 저장해야하죠.
* 따라서 wchar을 이용합니다. 유니코드, wide character를 저장할 수 있죠.
* 
* 그 외에 터미널 색깔 상수 등 다양한 문자 편의 기능이 이 헤더파일에 정의되어있습니다.*/

#pragma once
#include <wchar.h>

//솔직히 wchar_t는 너무 치기 힘들지 않나요?
typedef wchar_t wchar;

/// <summary>
/// 색깔코드를 모아둔 namespace입니다.
/// <para>이 게임에서 Black은 투명의 역할을 하며, 렌더링 되지 않습니다.</para>
/// </summary>
namespace Color
{
	const int Black = 0;
	const int Blue = 1;
	const int Green = 2;
	const int Aqua = 3;
	const int Red = 4;
	const int Purple = 5;
	const int Yellow = 6;
	const int LightGray = 7;
	const int Gray = 8;
	const int LightBlue = 9;
	const int LightGreen = 10;
	const int LightAqua = 11;
	const int LightRed = 12;
	const int LightPurple = 13;
	const int LightYellow = 14;
	const int White = 15;
}

/// <summary>
/// 콘솔에서 한글을 사용하기 위한 기본 설정을 수행합니다.
/// </summary>
/// <returns>정상적으로 수행되었다면 0을 반환합니다.</returns>
extern int initWchar(void);

/// <summary>
/// 입력된 wchar가 콘솔에서 두칸을 차지하는지 확인합니다.
/// </summary>
/// <param name="letter">판별할 wchar</param>
/// <returns>wchar가 두칸을 차지하면 TRUE, 아니면 FALSE가 반환됩니다.</returns>
extern bool isWide(wchar letter);
