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

#include "Graphic.h"

/// <summary>
/// 스크린 버퍼를 버퍼크기에 맞춥니다.
/// </summary>
/// <param name="buf">크기를 조정할 스크린 버퍼</param>
/// <param name="x">x방향 크기</param>
/// <param name="y">y방향 크기</param>
/// <param name="noSpace">여백없는 출력여부 (릴리스에서 문제가 발생하였음)</param>
/// <returns>정상적으로 조절되었다면 0이 리턴됩니다.</returns>
extern int setWindow(Buffer buf,bool noSpace);
