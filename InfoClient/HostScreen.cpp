#include <thread>
#include "Graphic.h"
#include "GameState.h"
#include "Network.h"
#include "Input.h"

//UDP 브로드캐스팅을 통한 ip교환 과정
//Host에서 지속적으로 UDP 브로드캐스트 패킷을 보냅니다.
//Find에서 UDP 브로드캐스트 패킷을 받은 다음, 적힌 ip로 핸드셰이크 요청을 보냅니다.
//Host에서 핸드셰이크요청을 수신하고 Find에 답장을 하게 하려 했으나 귀찮아서 만들지 않았습니다.
//이런 과정으로 서로의 ip를 공유하며, ip가 서로 교환되면 바로 게임이 시작되고, TCP 통로를 엽니다.
//이걸 핸드 셰이크라고 부를 수 있나? 몰?루

//예상되는 네트워크 에러
//Host에서 보낸 브로드캐스트가 유실됨 -> Find에서 호스트 찾기가 몇번 실패 할 수 있겠지만 큰 문제는 없습니다.
//Find에서 답장한 UDP 패킷이 유실됨 -> Find는 자신을 찾지 못하는 호스트에 TCP 핸드셰이크 요청을 하겠지만 실패할 것입니다.
//	 ㄴ아마 게임화면에 진입했다가 튕길듯..?


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
//UDP HandShake : 1102 (별건 아닌데, 제 생일이에요 :)

/// <summary>
/// 지속적으로 자신의 아이피를 알리는 브로드캐스트를 송신합니다.
/// </summary>
/// <param name="firstStart">자신의 선공 여부</param>
/// <param name="swit">킬 스위치</param>
void sendIPBroadcast(bool firstStart, bool* swit)
{
	//3초에 한번씩 UDP 브로드캐스트를 쏩니다
	for (;;)
	{
		if (*swit == false) break;
		if (firstStart)
			sendUDPBroadcast("Start MY", 3141);
		else
			sendUDPBroadcast("Start YOUR", 3141);
		Sleep(3000);
	}
}

/// <summary>
/// Find에서 보낼 UDP 핸드셰이크를 대기합니다.
/// </summary>
/// <param name="swit">브로드캐스팅을 끌 킬 스위치</param>
/// <param name="clientIP">클라이언트의 아이피를 받을 포인터</param>
/// <param name="OK">완료 여부</param>
void serveUDPHandshake(bool* swit, IPV4* clientIP, bool* OK)
{
	IPV4 ip(0, 0, 0, 0);
	for (;;)
	{
		char temp[256];
		if (*swit == false) return;
		if (receiveUDPMessage(temp, &ip, 3000, 1102) == 0) break;
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
	drawText(buf, cServerIP, 0, 2, 100, Color::LightRed);

	drawImage(buf, L"HostCancelButton.gres", 68, 37, HOST_CANCEL);

	return 0;
}

int playHostScreen(Buffer buf, GameState *state)
{
	if (!start)
	{
		kSwit = true;
		bool turn = true;
		thread broadcast(sendIPBroadcast, true, &kSwit);
		thread handshake(serveUDPHandshake, &kSwit, &ip, &isConnected);
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



	return 0;
}

