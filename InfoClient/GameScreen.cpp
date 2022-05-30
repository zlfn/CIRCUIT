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

//게임 진행 중 3개의 포트를 이용하게 됩니다.
//UDP HeartBeat 1102 : 1초에 한번씩 상대방에게 하트비트를 보내고, 받습니다. 
//	연속으로 3초 이상 하트비트가 수신되지 않으면 통신 오류로 간주하고 뻗습니다.
//UDP DataTransfer 6282 : 자신의 턴 일때 10ms에 한번씩 현재의 보드상태와 마우스 위치를 상대방에게 보냅니다.
//	몇개 유실되도 큰 문제가 없는 데이터이므로 검증이 없습니다.
//TCP TurnExchange 2005 : 자신의 턴을 끝내면 상대방에게 TCPRequest를 송신합니다.
//	자신의 턴이 끝난 직후부터 TCPServe를 엽니다. 하트비트에 의해 상호 연결이 보장되므로 타임아웃은 없습니다.

#include <thread>
#include <iostream>
#include "Graphic.h"
#include "GameState.h"
#include "Input.h"
#include "Network.h"

using namespace std;

enum Tile
{
	LR,
	UD,
	UR,
	RD,
	LD,
	UL,
	BLANK,
};

bool start = false;
bool startTurn = false;

bool killSwitch = false;

int curX;
int curY;
bool displayCur = false;
Tile curT = LR;

//하나라도 뒀는지 여부
bool placed = false;

//남은 자기 시간
int leftTime = 250;

//하트비트 체크
bool isConnected = true;

//타일 목록
Tile tiles[10][7];

//킬스위치 목록
bool getUDPKillSwitch = false;
bool sendUDPKillSwitch = false;

//자기 턴에 설치한 타일 3개
struct RecentTile
{
	int x;
	int y;
	Tile type;
} recentTile[3];

//타일 1-70
const int PLACE_TILE = 71;
const int END_TURN = 72;
const int RESET = 73;
const int IMPOSSIBLE = 74;

int resetVals()
{
	start = false;
	displayCur = false;
	isConnected = true;
	leftTime = 250;
	placed = false;
	killSwitch = false;
	recentTile[0].x = 0;
	recentTile[0].y = 0;
	recentTile[0].type = BLANK;
	recentTile[1].x = 0;
	recentTile[1].y = 0;
	recentTile[1].type = BLANK;
	recentTile[2].x = 0;
	recentTile[2].y = 0;
	recentTile[2].type = BLANK;
	return 0;
}

void sendUDPGameState(int *curX, int *curY, bool *displayCur, int* leftTime, Tile *curT, RecentTile* recentTile, IPV4 ip, bool* killSwitch)
{
	for (;;)
	{
		if (*killSwitch) break;
		char buffer[256];
		sprintf_s(buffer, 256, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d",
			*curX, *curY, *displayCur, *leftTime, *curT, recentTile[0].x, recentTile[0].y, recentTile[0].type,
			recentTile[1].x, recentTile[1].y, recentTile[1].type, recentTile[2].x, recentTile[2].y, recentTile[2].type);
		sendUDPMessage(buffer, ip, 6282);
		Sleep(10);
	}
}

void getUDPGameState(int *curX, int *curY, bool *displayCur, int* leftTime, Tile *curT, RecentTile* recentTile, bool* killSwitch)
{
	for (;;)
	{
		if (*killSwitch) break;
		char buffer[256];
		IPV4 temp(0, 0, 0, 0);
		if (receiveUDPMessage(buffer, &temp, 100, 6282) == 0)
		{
			int a = 0;
			cout << buffer << endl;
			sscanf_s(buffer, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d", &a, curY, displayCur, leftTime, curT, &recentTile[0].x, &recentTile[0].y, &recentTile[0].type,&recentTile[1].x, &recentTile[1].y, &recentTile[1].type, &recentTile[2].x, &recentTile[2].y, &recentTile[2].type);
			*curX = a; //<curX를 그대로 sscanf_s로 넣으니까 안되더군요, 1시간 동안의 깊은 고민 끝에 그냥 이따구로 처리하기로 결정했습니다.
			//원인은 모르겠는데 작동하면 됐죠 몰라몰라
		}
	}
}

void sendHeartbeat(bool* kSwit, IPV4 ip)
{
	for (;;)
	{
		if (*kSwit) break;
		sendUDPMessage("HAY", ip, 1102);
		Sleep(1000);
	}
}

void getHeartbeat(bool* kSwit, bool* connected)
{
	int check = 0;
	for (;;)
	{
		char buffer[256]; IPV4 ip(1, 1, 1, 1);
		if (*kSwit) break;
		receiveUDPMessage(buffer, &ip, 1000, 1102);

		if (strcmp(buffer, "HAY") == 0)
			check = 0;
		else
			check++;

		if (check >= 3) { *connected = false; break; }
	}
}

const wchar* getTileName(Tile t)
{
	switch (t)
	{
	case LR:
		return L"TileLR.gres"; break;
	case UD:
		return L"TileUD.gres"; break;
	case UR:
		return L"TileUR.gres"; break;
	case RD:
		return L"TileRD.gres"; break;
	case LD:
		return L"TileLD.gres"; break;
	case UL:
		return L"TileUL.gres"; break;
	}
	return L"";
}

int drawGameScreen(Buffer buf, GameState state)
{
	wchar leftTimeS[100];
	swprintf_s(leftTimeS, 100, L" %03d ", leftTime);
	if (state.turn)
	{
		drawText(buf, L"   내 차례   ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒", 0, 0, 100, Color::Green);
		drawText(buf, leftTimeS, 74, 0, 100, Color::Green);
	}
	else
	{
		drawText(buf, L" 상대방 차례 ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒", 0, 0, 100, Color::Gray);
		drawText(buf, leftTimeS, 74, 0, 100, Color::Gray);
	}
	
	//타일들은 1~70의 오브젝트 아이디를 부여받게 됩니다.
	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOn.gres", 5 + 7 * i, 2, i+1);
	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOn.gres", 5 + 7 * i, 6, i+11);
	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOn.gres", 5 + 7 * i, 10, i+21);
	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOn.gres", 5 + 7 * i, 14, i+31);
	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOn.gres", 5 + 7 * i, 18, i+41);
	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOn.gres", 5 + 7 * i, 22, i+51);
	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOn.gres",  5 + 7 * i, 26, i+61);

	wchar cServerIP[100];
	swprintf_s(cServerIP, 100, L"%d.%d.%d.%d", state.commIP.b1, state.commIP.b2, state.commIP.b3, state.commIP.b4);
	drawText(buf, cServerIP, 1, 30, 100, Color::Gray);

	drawText(buf, L"════════════════════════════════════════════════════════════════════════════════", 0, 31, 80, Color::White);
	drawImage(buf, getTileName(curT), 3, 33, PLACE_TILE);
	if (placed)
		drawImage(buf, L"EndTurnButtonOn.gres", 32, 33);
	else
		drawImage(buf, L"EndTurnButtonOff.gres", 32, 33, END_TURN);
	drawImage(buf, L"ResetButton.gres", 37, 36);
	drawImage(buf, L"ImpossibleButton.gres",68, 37);
	drawText(buf, L"회로 배치", 3, 37, 100, Color::White);
	drawText(buf, L"Z,X로 회전", 70, 32, 100, Color::Gray);
	drawText(buf, L"C로 뒤집기", 70, 33, 100, Color::Gray);
	drawText(buf, L"ESC로 취소", 70, 34, 100, Color::Gray);


	if(displayCur)
		drawImage(buf, getTileName(curT), curX - 3, curY - 2);

	return 0;
}

int playGameScreen(Buffer buf, GameState* state)
{
	if (!start)
	{
		resetVals();
		//thread s(sendHeartbeat, &killSwitch, state->commIP);
		//thread g(getHeartbeat, &killSwitch, &isConnected);
		//s.detach(); g.detach();
		start = true;
	}

	//입력 (자기 턴일떄만)
	if (state->turn)
	{
		if (!startTurn)
		{
			sendUDPKillSwitch = false;
			getUDPKillSwitch = true;
			thread send(sendUDPGameState, &curX, &curY, &displayCur, &leftTime, &curT, recentTile, state->commIP, &sendUDPKillSwitch);
			send.detach();
			startTurn = true;
		}

		curX = getClick().pos.X;
		curY = getClick().pos.Y;
		MouseClick k = getClickOnce();
		if (getClickObject(buf) == PLACE_TILE && k.type == Left)
			displayCur = true;

		int key = getKeyOnce();
		switch (key)
		{
		case Key::kX:
			curT = UD; break;
		case Key::kZ:
			curT = LR; break;
		case Key::kESC:
			displayCur = false; break;
		}
	}
	else
	{
		if (!startTurn)
		{
			sendUDPKillSwitch = true;
			getUDPKillSwitch = false;
			thread get(getUDPGameState, &curX, &curY, &displayCur, &leftTime, &curT, recentTile, &getUDPKillSwitch);
			get.detach();
			startTurn = true;
		}
	}

	if (isConnected == false)
	{
		killSwitch = true;
		state->scene = Error;
	}

	return 0;
}