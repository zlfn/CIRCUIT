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

#include <thread>
#include "Graphic.h"
#include "GameState.h"
#include "Network.h"
#include "Scenes.h"
#include "Input.h"

const int FIND_CANCEL = 1;

namespace FindVal
{
	bool start = false;
	IPV4 ip(0, 0, 0, 0);
	bool isConnected = false;
	bool killSwitch = false;
	bool canCancel = true;
}

void resetFindVal() {
	FindVal::start = false;
	IPV4 temp(0, 0, 0, 0);
	FindVal:: ip = temp;
	FindVal::isConnected = false;
	FindVal::killSwitch = false;
	FindVal::canCancel = true;

}

using namespace FindVal;
using namespace std;

//UDP Broadcast : 3141 (파이)
//UDP HandShake : 1102 (별건 아닌데, 제 생일이에요 :)
//핸드셰이크 과정은 HostScreen.cpp에 기술되어 있습니다.

/// <summary>
/// UDP 브로드캐스트를 받아서 핸드셰이크를 시행합니다.
/// </summary>
/// <param name="serverIP">아이피를 저장할 IPV4 포인터</param>
/// <param name="kSwit">작동을 중간에 정지할 킬 스위치 포인터</param>
/// <param name="turn">Host로부터 받은 턴 정보를 저장할 포인터</param>
/// <param name="finished">정상적으로 시행이 끝나면 true가 되는 포인터</param>
void UDPHandShake(IPV4* serverIP, bool* kSwit, bool* canCancel, bool* turn, bool* finished)
{
	for(;;)
	{
		IPV4 t(0, 0, 0, 0);
		*serverIP = t;
		*canCancel = true;
		char temp[2048];

		//브로드캐스트를 받음
		if (*kSwit == true) break;
		receiveUDPMessage(temp, serverIP, 100, 3141);
		if (strcmp(temp, "GAMEHOST") == 0)
		{
			*canCancel = false;

			//브로드캐스트를 받았다는 메시지를 보냄
			sendUDPMessage("OK FOUND", *serverIP, 1102);
			IPV4 t(1, 1, 1, 1);

			//턴 순서를 받음
			receiveUDPMessage(temp, &t, 1000, 1102);
			if (strcmp(temp, "OK MYTURN") == 0)
			{
				*turn = false;
				*finished = true;
				break;
			}
			else if (strcmp(temp, "OK YOURTURN") == 0)
			{
				*turn = true;
				*finished = true;
				break;
			}
			//답을 받지 못했다면 연결 실패로 간주하고 처음으로 돌아감
			else 
			{
				continue;
			}
		}
	}
}

/// DEPRECATED
/// <summary>
/// 메인화면으로 돌아가는 것을 굳이 멀티스레딩을 이용해서 2초 늦춥니다.
/// <para>스파게티 코드의 세계에 오신것을 환영합니다!</para>
/// </summary>
/// <param name="state">변경할 상태변수 포인터</param>
/// <param name="start">돌아갈땐 start 변수도 꺼줘야 해요</param>
/*void delayMoveScreen(GameState* state, bool* start)
{
	Sleep(2000);
	*start = false;
	resetCard();
	state->scene = Main;
}*/

int drawFindScreen(Buffer buf, GameState state)
{
	drawText(buf, L"호스트 찾는중...", 0, 0, 100, Color::White);
	wchar cServerIP[100];
	swprintf_s(cServerIP, 100, L"연결중 : %d.%d.%d.%d", ip.b1, ip.b2, ip.b3, ip.b4);
	drawText(buf, cServerIP, 0, 2, 100, Color::Gray);

	if (isConnected)
	{
		drawText(buf, L"연결완료!", 0, 3, 100, Color::LightGreen);
		if (state.turn)
			drawText(buf, L"MY TURN", 0, 4, 100, Color::Gray);
		else
			drawText(buf, L"YOUR TURN", 0, 4, 100, Color::Gray);
	}

	if(canCancel)
		drawImage(buf, L"CancelButton.gres", 68, 37, FIND_CANCEL);
	return 0;
}


int playFindScreen(Buffer buf, GameState *state)
{
	if (!start)
	{
		isConnected = false;
		canCancel = true;
		killSwitch = false;
		thread handshake(UDPHandShake, &ip, &killSwitch, &canCancel, &state->turn, &isConnected);
		handshake.detach();
		start = true;
	}

	//어쩌다보니 Host의 킬 스위치는 false일때 작동하는데 Find의 킬 스위치는 true일때 작동하게 만들어졌네요
	//디버깅은 미래의 내가 잘 하겠지~
	bool clickCheck = (getClickOnce().type == Left && getClickObject(buf) == FIND_CANCEL);
	if ((getKey() == Key::kESC || clickCheck)&&canCancel)
	{
		resetCard();
		start = false;
		killSwitch = true;
		isConnected = false;


		IPV4 temp(1, 1, 1, 1);
		ip = temp;

		resetFindVal();
		state->scene = Main;
	}

	if (isConnected)
	{
		start = false;
		isConnected = false;

		state->commIP = ip;
		resetFindVal();
		state->scene = Game;
	}

	return 0;
}