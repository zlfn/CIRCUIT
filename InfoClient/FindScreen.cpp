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

//UDP Broadcast : 3141 (����)
//UDP HandShake : 1102 (���� �ƴѵ�, �� �����̿��� :)
//�ڵ����ũ ������ HostScreen.cpp�� ����Ǿ� �ֽ��ϴ�.

/// <summary>
/// UDP ��ε�ĳ��Ʈ�� �޾Ƽ� �ڵ����ũ�� �����մϴ�.
/// </summary>
/// <param name="serverIP">�����Ǹ� ������ IPV4 ������</param>
/// <param name="kSwit">�۵��� �߰��� ������ ų ����ġ ������</param>
/// <param name="turn">Host�κ��� ���� �� ������ ������ ������</param>
/// <param name="finished">���������� ������ ������ true�� �Ǵ� ������</param>
void UDPHandShake(IPV4* serverIP, bool* kSwit, bool* canCancel, bool* turn, bool* finished)
{
	for(;;)
	{
		IPV4 t(0, 0, 0, 0);
		*serverIP = t;
		*canCancel = true;
		char temp[2048];

		//��ε�ĳ��Ʈ�� ����
		if (*kSwit == true) break;
		receiveUDPMessage(temp, serverIP, 100, 3141);
		if (strcmp(temp, "GAMEHOST") == 0)
		{
			*canCancel = false;

			//��ε�ĳ��Ʈ�� �޾Ҵٴ� �޽����� ����
			sendUDPMessage("OK FOUND", *serverIP, 1102);
			IPV4 t(1, 1, 1, 1);

			//�� ������ ����
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
			//���� ���� ���ߴٸ� ���� ���з� �����ϰ� ó������ ���ư�
			else 
			{
				continue;
			}
		}
	}
}

/// DEPRECATED
/// <summary>
/// ����ȭ������ ���ư��� ���� ���� ��Ƽ�������� �̿��ؼ� 2�� ����ϴ�.
/// <para>���İ�Ƽ �ڵ��� ���迡 ���Ű��� ȯ���մϴ�!</para>
/// </summary>
/// <param name="state">������ ���º��� ������</param>
/// <param name="start">���ư��� start ������ ����� �ؿ�</param>
/*void delayMoveScreen(GameState* state, bool* start)
{
	Sleep(2000);
	*start = false;
	resetCard();
	state->scene = Main;
}*/

int drawFindScreen(Buffer buf, GameState state)
{
	drawText(buf, L"ȣ��Ʈ ã����...", 0, 0, 100, Color::White);
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

	//��¼�ٺ��� Host�� ų ����ġ�� false�϶� �۵��ϴµ� Find�� ų ����ġ�� true�϶� �۵��ϰ� ��������׿�
	//������� �̷��� ���� �� �ϰ���~
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