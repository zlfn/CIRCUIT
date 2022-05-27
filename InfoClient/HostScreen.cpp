#include <thread>
#include "Graphic.h"
#include "GameState.h"
#include "Network.h"
#include "Input.h"

//UDP ��ε�ĳ������ ���� ip��ȯ ����
//Host���� ���������� UDP ��ε�ĳ��Ʈ ��Ŷ�� �����ϴ�.
//Find���� UDP ��ε�ĳ��Ʈ ��Ŷ�� ���� ����, ���� ip�� �ڵ����ũ ��û�� �����ϴ�.
//Host���� �ڵ����ũ��û�� �����ϰ� Find�� ������ �ϰ� �Ϸ� ������ �����Ƽ� ������ �ʾҽ��ϴ�.
//�̷� �������� ������ ip�� �����ϸ�, ip�� ���� ��ȯ�Ǹ� �ٷ� ������ ���۵ǰ�, TCP ��θ� ���ϴ�.
//�̰� �ڵ� ����ũ��� �θ� �� �ֳ�? ��?��

//����Ǵ� ��Ʈ��ũ ����
//Host���� ���� ��ε�ĳ��Ʈ�� ���ǵ� -> Find���� ȣ��Ʈ ã�Ⱑ ��� ���� �� �� �ְ����� ū ������ �����ϴ�.
//Find���� ������ UDP ��Ŷ�� ���ǵ� -> Find�� �ڽ��� ã�� ���ϴ� ȣ��Ʈ�� TCP �ڵ����ũ ��û�� �ϰ����� ������ ���Դϴ�.
//	 ���Ƹ� ����ȭ�鿡 �����ߴٰ� ƨ���..?


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
//UDP HandShake : 1102 (���� �ƴѵ�, �� �����̿��� :)

/// <summary>
/// ���������� �ڽ��� �����Ǹ� �˸��� ��ε�ĳ��Ʈ�� �۽��մϴ�.
/// </summary>
/// <param name="firstStart">�ڽ��� ���� ����</param>
/// <param name="swit">ų ����ġ</param>
void sendIPBroadcast(bool firstStart, bool* swit)
{
	//3�ʿ� �ѹ��� UDP ��ε�ĳ��Ʈ�� ���ϴ�
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
/// Find���� ���� UDP �ڵ����ũ�� ����մϴ�.
/// </summary>
/// <param name="swit">��ε�ĳ������ �� ų ����ġ</param>
/// <param name="clientIP">Ŭ���̾�Ʈ�� �����Ǹ� ���� ������</param>
/// <param name="OK">�Ϸ� ����</param>
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
	drawText(buf, L"ȣ��Ʈ��...", 0, 0, 100, Color::White);
	drawText(buf, L"������ Ÿ�ֿ̹� ����ϸ� ������ �� �� ��������?", 0,39, 100, Color::Gray);
	swprintf_s(cServerIP, 100, L"������ : %d.%d.%d.%d", ip.b1, ip.b2, ip.b3, ip.b4);
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

