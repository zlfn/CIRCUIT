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

//UDP Broadcast : 3141 (����)
//UDP HandShake : 1102 (���� �ƴѵ�, �� �����̿��� :)
//�ڵ����ũ ������ HostScreen.cpp�� ����Ǿ� �ֽ��ϴ�.

/// <summary>
/// UDP ��ε�ĳ��Ʈ�� �޾Ƽ� �ڵ����ũ�� �����մϴ�.
/// </summary>
/// <param name="serverIP">�����Ǹ� ������ IPV4 ������</param>
/// <param name="finished">���������� ������ ������ true�� �Ǵ� ������</param>
/// <param name="failed">ȣ��Ʈ�� �������� ���Ͽ����� true�� �Ǵ� ������</param>
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
		drawText(buf, L"ȣ��Ʈ ã����...", 0, 0, 100, Color::White);
	else
		drawText(buf, L"ȣ��Ʈ�� ã�� ���߽��ϴ�.", 0, 0, 100, Color::White);
	wchar cServerIP[100];
	swprintf_s(cServerIP, 100, L"������ : %d.%d.%d.%d", ip.b1, ip.b2, ip.b3, ip.b4);
	drawText(buf, cServerIP, 0, 2, 100, Color::Gray);

	if (isConnected)
		drawText(buf, L"����Ϸ�!", 0, 3, 100, Color::LightGreen);
	return 0;
}

/// <summary>
/// ����ȭ������ ���ư��� ���� ���� ��Ƽ�������� �̿��ؼ� 2�� ����ϴ�.
/// <para>���İ�Ƽ �ڵ��� ���迡 ���Ű��� ȯ���մϴ�!</para>
/// </summary>
/// <param name="state">������ ���º��� ������</param>
/// <param name="start">���ư��� start ������ ����� �ؿ�</param>
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