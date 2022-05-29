//GNU GPL 3.0 lisence
/*
 * ���� ������ ����
 * Copyright (C) 2022 ������, ������, ����ȯ
 *
 * �� ���α׷��� ���� ����Ʈ�����Դϴ�. ����Ʈ������ �Ǿ絵�ڴ� ���� ����Ʈ����
 * ����� ��ǥ�� GNU �Ϲ� ���� ��� �㰡�� 3�� Ȥ�� �� ���� ���� ���Ƿ� �����Ͽ�
 * �� ������ ���� ���α׷��� �����ϰų� ������� �� �ֽ��ϴ�.
 *
 * �� ���α׷��� �����ϰ� ���� �� ��������� ������� �����ǰ� ������, Ư����
 * ������ �´� ���ռ� ���γ� �Ǹſ����� ����� �� ��������� �������� ������ ������
 * ��� ������ ������ �������� �ʽ��ϴ�. ���� �ڼ��� ���׿� ���ؼ���
 * GNU �Ϲ� ���� �㰡���� �����Ͻñ� �ٶ��ϴ�.
 *
 * GNU �Ϲ� ���� ��� �㰡���� �� ���α׷��� �Բ� �����˴ϴ�. ���� ������ �����Ǿ��ִٸ�
 * <http://www.gnu.org/licenses/>�� �����Ͻñ� �ٶ��ϴ�.
 */

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
void UDPHandShake(IPV4* serverIP, bool* turn, bool* finished, bool* failed)
{
	char temp[256];
	receiveUDPMessage(temp, serverIP, 5000, 3141);
	if (strcmp(temp, "GAMEHOST") != 0)
	{
		*failed = true;
	}
	else
	{
		sendUDPMessage("OK FOUND", *serverIP, 1102);
		IPV4 t(0, 0, 0, 0);
		receiveUDPMessage(temp, &t, 3000, 1102);
		if (strcmp(temp, "OK MYTURN") == 0)
		{
			*turn = false;
			*finished = true;
		}
		else if (strcmp(temp, "OK YOURTURN") == 0)
		{
			*turn = true;
			*finished = true;
		}
		else 
		{
			*failed = true;
		}
	}
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
	{
		drawText(buf, L"����Ϸ�!", 0, 3, 100, Color::LightGreen);
		if (state.turn)
			drawText(buf, L"MY TURN", 0, 4, 100, Color::Gray);
		else
			drawText(buf, L"YOUR TURN", 0, 4, 100, Color::Gray);
	}
	return 0;
}


int playFindScreen(Buffer buf, GameState *state)
{
	if (!start)
	{
		isConnected = false;
		failed = false;
		failCount = false;
		thread handshake(UDPHandShake,  &ip, &state->turn, &isConnected, &failed);
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

	if (isConnected)
	{
		start = false;
		isConnected = false;

		state->commIP = ip;
		state->scene = Game;
	}

	return 0;
}