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
#include <random>
#include "Graphic.h"
#include "GameState.h"
#include "Network.h"
#include "Input.h"

using namespace std;

//UDP ��ε�ĳ������ ���� ip��ȯ ����
//Host���� ���������� "GAMEHOST"�� ���� UDP ��ε�ĳ��Ʈ ��Ŷ�� �����ϴ�.
//Find���� UDP ��ε�ĳ��Ʈ ��Ŷ�� ���� ����, ���� ip�� "OK FOUND" �ڵ����ũ ��û�� �����ϴ�.
//Host���� �ڵ����ũ��û�� �����ϰ� Find�� "OK MYTURN" Ȥ�� "OK YOURTURN" ������ �Ͽ� ����� �����Ǿ����� �˸��ϴ�.
//�̷� �������� ������ ip�� �����ϸ�, ip�� ���� ��ȯ�Ǹ� �ٷ� ������ ���۵ǰ�, TCP ��θ� ���ϴ�.

//����Ǵ� �ֿ� ��Ʈ��ũ �̽�
//* ���ÿ� �� Find�� �� Host ��ε�ĳ��Ʈ�� ����
//Host�� �� �߿� ���� ������ �ڵ����ũ ��û�� ����, ��� ���� ��û�� �� Find �ۿ� ���� �� ����
//* Find, Host �� �� �ϳ��� ���� ������ ���ܼ� �� �� �� ����� ����ȭ�鿡 �����ϴ� ��� 
//����ȭ������ �Ѿ�� ��Ʈ��Ʈ ���� �� ����� ����Ƿ� �� ����� ����ȭ�鿡 ������ ���� ���� �߻����� �ʽ��ϴ� :)

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
/// ���������� �ڽ��� �����Ǹ� �˸��� ��ε�ĳ��Ʈ�� �۽��մϴ�.
/// </summary>
/// <param name="swit">ų ����ġ</param>
void sendIPBroadcast(bool* swit)
{
	//3�ʿ� �ѹ��� UDP ��ε�ĳ��Ʈ�� ���ϴ�
	for (;;)
	{
		if (*swit == false) break;
		sendUDPBroadcast("GAMEHOST", 3141);
		Sleep(3000);
	}
}

/// <summary>
/// Find���� ���� UDP �ڵ����ũ�� ����մϴ�.
/// </summary>
/// <param name="turn">�ڽ��� ���� ����</param>
/// <param name="swit">��ε�ĳ������ �� ų ����ġ</param>
/// <param name="clientIP">Ŭ���̾�Ʈ�� �����Ǹ� ���� ������</param>
/// <param name="OK">�Ϸ� ���θ� ���� ������</param>
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
			Sleep(100); //< Find�� �޽����� ������ UDP ��ȣ �ޱ⸦ ����ϱ���� �ɸ��� �ð��� ���

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
	drawText(buf, L"ȣ��Ʈ��...", 0, 0, 100, Color::White);
	drawText(buf, L"������ Ÿ�ֿ̹� ����ϸ� ������ �� �� ��������?", 0,39, 100, Color::Gray);
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
