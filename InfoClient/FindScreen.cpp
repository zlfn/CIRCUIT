#include <thread>
#include "Graphic.h"
#include "GameState.h"
#include "Network.h"
#include "Scenes.h"

namespace FindVal
{
	bool start = false;
	IPV4 ip(0, 0, 0, 0);
	bool isConnected = false;
	bool failed = false;
	bool failCount = false;
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
/// <param name="finished">정상적으로 시행이 끝나면 true가 되는 포인터</param>
/// <param name="failed">호스트를 감지하지 못하였으면 true가 되는 포인터</param>
void UDPHandShake(IPV4* serverIP, bool* finished, bool* failed)
{
	char temp[256];
	int checker = receiveUDPMessage(temp, serverIP, 5000, 3141);
	if (checker == -1)
		*failed = true;
	else
	{
		sendUDPMessage("OK", *serverIP, 1102);
		IPV4 t(0, 0, 0, 0);
		if (receiveUDPMessage(temp, &t, 3000, 1102) == 0)
			*finished = true;
		else 
		{
			ip = t;
			*failed = true;
		}
	}
}

int drawFindScreen(Buffer buf, GameState state)
{
	if(!failCount)
		drawText(buf, L"호스트 찾는중...", 0, 0, 100, Color::White);
	else
		drawText(buf, L"호스트를 찾지 못했습니다.", 0, 0, 100, Color::White);
	wchar cServerIP[100];
	swprintf_s(cServerIP, 100, L"연결중 : %d.%d.%d.%d", ip.b1, ip.b2, ip.b3, ip.b4);
	drawText(buf, cServerIP, 0, 2, 100, Color::Gray);

	if (isConnected)
		drawText(buf, L"연결완료!", 0, 3, 100, Color::LightGreen);
	return 0;
}

/// <summary>
/// 메인화면으로 돌아가는 것을 굳이 멀티스레딩을 이용해서 2초 늦춥니다.
/// <para>스파게티 코드의 세계에 오신것을 환영합니다!</para>
/// </summary>
/// <param name="state">변경할 상태변수 포인터</param>
/// <param name="start">돌아갈땐 start 변수도 꺼줘야 해요</param>
void delayMoveScreen(GameState* state, bool *start)
{
	Sleep(2000);
	*start = false;
	resetCard();
	state->scene = Main;
}

int playFindScreen(Buffer buf, GameState *state)
{
	if (!start)
	{
		isConnected = false;
		failed = false;
		failCount = false;
		thread handshake(UDPHandShake, &ip, &isConnected, &failed);
		handshake.detach();
		start = true;
	}

	if (failed == true)
	{
		failed = false;
		failCount = true;
		thread delay(delayMoveScreen, state, &start);
		delay.detach();
	}

	return 0;
}