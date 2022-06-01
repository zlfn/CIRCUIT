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
#include <random>
#include "Graphic.h"
#include "GameState.h"
#include "Network.h"
#include "Input.h"

using namespace std;

//UDP 브로드캐스팅을 통한 ip교환 과정
//Host에서 지속적으로 "GAMEHOST"가 적힌 UDP 브로드캐스트 패킷을 보냅니다.
//Find에서 UDP 브로드캐스트 패킷을 받은 다음, 적힌 ip로 "OK FOUND" 핸드셰이크 요청을 보냅니다.
//Host에서 핸드셰이크요청을 수신하고 Find에 "OK MYTURN" 혹은 "OK YOURTURN" 답장을 하여 통신이 수락되었음을 알립니다.
//이런 과정으로 서로의 ip를 공유하며, ip가 서로 교환되면 바로 게임이 시작되고, TCP 통로를 엽니다.

//예상되는 주요 네트워크 이슈
//* 동시에 두 Find가 한 Host 브로드캐스트를 잡음
//Host는 둘 중에 먼저 들어오는 핸드셰이크 요청만 수신, 통신 수락 요청은 한 Find 밖에 받을 수 없음
//* Find, Host 둘 중 하나가 도중 연결이 끊겨서 둘 중 한 사람만 게임화면에 진입하는 경우 
//게임화면으로 넘어가서 하트비트 보낼 때 통신이 끊기므로 한 사람만 게임화면에 영원히 남는 일은 발생하지 않습니다 :)

namespace HostVal
{
	bool start = false;
	bool kSwit = false;
	bool isConnected = false;
	IPV4 ip(0, 0, 0, 0);
}

using namespace HostVal;
using namespace std;

const int HOST_CANCEL = 1;

//UDP Broadcast : 3141
//UDP HandShake : 1102 

/// <summary>
/// 지속적으로 자신의 아이피를 알리는 브로드캐스트를 송신합니다.
/// </summary>
/// <param name="swit">킬 스위치</param>
void sendIPBroadcast(bool* swit)
{
	//3초에 한번씩 UDP 브로드캐스트를 쏩니다
	for (;;)
	{
		if (*swit == false) break;
		sendUDPBroadcast("GAMEHOST", 3141);
		Sleep(3000);
	}
}

/// <summary>
/// Find에서 보낼 UDP 핸드셰이크를 대기합니다.
/// </summary>
/// <param name="turn">자신의 선공 여부</param>
/// <param name="swit">브로드캐스팅을 끌 킬 스위치</param>
/// <param name="clientIP">클라이언트의 아이피를 받을 포인터</param>
/// <param name="OK">완료 여부를 받을 포인터</param>
void serveUDPHandshake(bool turn, bool* swit, IPV4* clientIP, bool* OK)
{
	IPV4 ip(0, 0, 0, 0);
	for (;;)
	{
		char temp[2048];
		receiveUDPMessage(temp, &ip, 3000, 1102);
		if (*swit == false) return;
		if (strcmp(temp, "OK FOUND")==0)
		{
			Sleep(100); //< Find가 메시지를 보내고 UDP 신호 받기를 대기하기까지 걸리는 시간을 대기

			if (turn)
				sendUDPMessage("OK MYTURN", ip, 1102);
			else
				sendUDPMessage("OK YOURTURN", ip, 1102);
			break;
		}
	}

	*clientIP = ip;
	*OK = true;
	*swit = false;
}

int drawHostScreen(Buffer buf, GameState state)
{
	wchar cServerIP[100];
	drawText(buf, L"호스트중...", 0, 0, 100, Color::White);
	drawText(buf, L"절묘한 타이밍에 취소하면 에러를 낼 수 있을지도?", 0,39, 100, Color::Gray);
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

	drawImage(buf, L"CancelButton.gres", 68, 37, HOST_CANCEL);

	return 0;
}

int playHostScreen(Buffer buf, GameState *state)
{
	if (!start)
	{
		kSwit = true;
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(0,1);
		bool turn = dis(gen);
		state->turn = turn;

		thread broadcast(sendIPBroadcast, &kSwit);
		thread handshake(serveUDPHandshake, state->turn, &kSwit, &ip, &isConnected);
		broadcast.detach();
		handshake.detach();

		start = true;
	}
	
	bool clickCheck = (getClickOnce().type == Left && getClickObject(buf) == HOST_CANCEL);
	if (getKey() == Key::kESC || clickCheck)
	{
		start = false;
		kSwit = false;
		isConnected = false;


		IPV4 temp(0, 0, 0, 0);
		ip = temp;

		state->scene = Main;
	}

	if (isConnected)
	{
		start = false;
		kSwit = false;
		isConnected = false;
		state->commIP = ip;

		state->scene = Game;
	}


	return 0;
}
