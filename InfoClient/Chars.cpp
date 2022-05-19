﻿//GNU GPL 3.0 lisence
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

//콘솔에서 한글을 포함한 유니코드를 이용하기 위한 함수를 모아놓은 파일입니다.
#include <iostream>
#include <Windows.h>
#include "Chars.h"

int initWchar(void)
{
	setlocale(LC_ALL, "korean");
	return 0;
}

bool isWide(wchar s)
{
	//(옛한글을 제외하면) 한글 유니코드의 첫글자는 가, 끝 글자는 힣입니다.
	const wchar start = L'가';
	const wchar end = L'힣';
	if(s >= start && s <= end) return true;
	if (s == '■') return true;
	if (s == '□') return true;
	return false;
}

