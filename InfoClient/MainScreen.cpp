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

#include "Graphic.h"
#include "GameState.h"
#include "Input.h"

const int SETTING_BUTTON = 1;
const int Card1 = 2;
const int Card2 = 3;
const int LINK_BUTTON = 4;
const int HOST_BUTTON = 5;
const int FIND_BUTTON = 6;
const int MANUAL_BUTTON = 7;


int Card1X = 60;
int Card1Y = 7;
int Card2X = 12;
int Card2Y = 2;

//1번 카드가 위에 있는 여부
bool stack = false;

int resetCard()
{
	Card1X = 60;
	Card1Y = 7;
	Card2X = 12;
	Card2Y = 2;
	return 0;
}

int drawMainScreen(Buffer buf, GameState state)
{
	drawImage(buf, L"SettingButton.gres", 68, 37, SETTING_BUTTON);
	drawImage(buf, L"GiftButton.gres", 56, 37, LINK_BUTTON);
	drawImage(buf, L"HostButton.gres", 28, 16, HOST_BUTTON);
	drawText(buf, L"게임 호스트", 40, 21, 100, Color::Gray);
	drawImage(buf, L"FindButton.gres", 28, 23, FIND_BUTTON);
	drawText(buf, L"호스트 찾기", 40, 28, 100, Color::Gray);
	drawImage(buf, L"ManualButton.gres", 28, 30, MANUAL_BUTTON);

	if (stack)
	{
		drawImage(buf, L"Card2.gres", Card2X, Card2Y, Card2);
		drawImage(buf, L"Card1.gres", Card1X, Card1Y, Card1);
	}
	else
	{
		drawImage(buf, L"Card1.gres", Card1X, Card1Y, Card1);
		drawImage(buf, L"Card2.gres", Card2X, Card2Y, Card2);
	}

	drawImage(buf, L"CIRCUIT_LOGO.gres", 16, 4);
	drawText(buf, L"언리버를 뛰어넘는", 0, 0, 200, Color::Gray);
	drawText(buf, L"최고의 수학동아리", 18, 0, 200, Color::LightGreen);
	drawText(buf, L"\"라온제나\"", 36, 0, 200, Color::LightYellow);
	drawText(buf, L"부스도 와주세요! - 박지환", 47, 0, 200, Color::LightGreen);
	drawText(buf, L"그런 라온제나를 뛰어넘는 차원이 다른 수학동아리", 0, 1, 200, Color::LightGreen);
	drawText(buf, L"\"LIMES\"", 48, 1, 200, Color::LightYellow);
	drawText(buf, L"로 오세요! - 김진서", 56, 1, 200, Color::LightGreen);
	drawText(buf, L"언리버 과학동아리 학술 발표회 2022", 32, 10, 100, Color::LightGreen);
	drawText(buf, L"(C) 2022. 박찬웅, 김진서, 박지환", 0/*29*/, 38, 200, Color::Green);
	drawText(buf, L"This software distributed under GNU GPL 3.0 license", 0/*30*/, 39, 200, Color::Green);

	return 0;
}

int playMainScreen(Buffer buf, GameState* state)
{
	if (getClickOnce().type == Left)
	{
		switch (getClickObject(buf))
		{
		case SETTING_BUTTON:
			state->scene = Setting;
			break;
		case HOST_BUTTON:
			state->scene = Host;
			break;
		case FIND_BUTTON:
			state->scene = Find;
		case Card1:
			Card1X = getClick().pos.X - 3;
			Card1Y = getClick().pos.Y - 3;
			stack = true;
			break;
		case Card2:
			Card2X = getClick().pos.X - 3;
			Card2Y = getClick().pos.Y - 3;
			stack = false;
			break;
		case MANUAL_BUTTON:
			state->scene = Info;
		}
	}
	return 0;
}