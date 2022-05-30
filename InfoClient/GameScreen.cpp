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
#include "Tiles.h"

using namespace std;


bool start = false;
bool startTurn = false;

bool killSwitch = false;

int curX;
int curY;
bool displayCur = false;
Tile curT = LR;

//둔 타일 개수
int placed = 0;

//남은 자기 시간
int leftTime = 250;

//하트비트 체크
bool isConnected = true;

//타일 목록
Tile tiles[10][7];

//킬스위치 목록
bool getUDPKillSwitch = false;
bool sendUDPKillSwitch = false;


RecentTile recentTile[3];

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

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 7; j++)
			tiles[i][j] = BLANK;

	tiles[4][3] = RD;
	tiles[5][3] = UL;

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

int resetRecentTiles()
{
	placed = 0;
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

void sendUDPGameState(int *curX, int *curY, bool *displayCur, int* leftTime, Tile *curT, RecentTile* recentTile, int *placed, IPV4 ip, bool* killSwitch)
{
	for (;;)
	{
		if (*killSwitch) break;
		char buffer[256];
		sprintf_s(buffer, 256, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
			*curX, *curY, *displayCur, *leftTime, *curT, recentTile[0].x, recentTile[0].y, recentTile[0].type,
			recentTile[1].x, recentTile[1].y, recentTile[1].type, recentTile[2].x, recentTile[2].y, recentTile[2].type, *placed);
		sendUDPMessage(buffer, ip, 6282);
		Sleep(10);
	}
}

void getUDPGameState(int *curX, int *curY, bool *displayCur, int* leftTime, Tile *curT, RecentTile* recentTile, int* placed, bool* killSwitch)
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
			sscanf_s(buffer, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &a, curY, displayCur, leftTime, curT, &recentTile[0].x, &recentTile[0].y, &recentTile[0].type,&recentTile[1].x, &recentTile[1].y, &recentTile[1].type, &recentTile[2].x, &recentTile[2].y, &recentTile[2].type, placed);
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
		drawImage(buf, L"TileFrameOff.gres", 5 + 7 * i, 2);
	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOff.gres", 5 + 7 * i, 6);
	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOff.gres", 5 + 7 * i, 10);
	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOff.gres", 5 + 7 * i, 14);
	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOff.gres", 5 + 7 * i, 18);
	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOff.gres", 5 + 7 * i, 22);
	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOff.gres",  5 + 7 * i, 26);

	//4 꼭짓점 
	drawTile(tiles[0][0], buf, 0, 0, BLANK, BLANK, tiles[1][0], tiles[0][1], placed);
	drawTile(tiles[9][0], buf, 9, 0, BLANK, tiles[8][0], BLANK, tiles[9][1], placed);
	drawTile(tiles[0][6], buf, 0, 6, tiles[0][5], BLANK, tiles[1][6], BLANK, placed);
	drawTile(tiles[9][6], buf, 9, 6, tiles[9][5], tiles[8][6], BLANK, BLANK, placed);

	//4 모서리
	for (int i = 1; i < 9; i++)
		drawTile(tiles[i][0], buf, i, 0, BLANK, tiles[i - 1][0], tiles[i + 1][0], tiles[i][1], placed);
	for (int i = 1; i < 9; i++)
		drawTile(tiles[i][6], buf, i, 6, tiles[i][5], tiles[i-1][6], tiles[i+1][6], BLANK, placed);
	for (int j = 1; j < 6; j++)
		drawTile(tiles[0][j], buf, 0, j, tiles[0][j - 1], BLANK, tiles[1][j], tiles[0][j + 1], placed);
	for (int j = 1; j < 6; j++)
		drawTile(tiles[9][j], buf, 9, j, tiles[9][j - 1], tiles[8][j], BLANK, tiles[9][j + 1], placed);

	for (int i = 1; i < 9; i++)
		for (int j = 1; j < 6; j++)
			drawTile(tiles[i][j], buf, i, j, tiles[i][j - 1], tiles[i - 1][j], tiles[i + 1][j], tiles[i][j + 1], placed);

	if (placed == 1)
	{
		int cx = recentTile[0].x;
		int cy = recentTile[0].y;

		if(cx>=1)
			if(tiles[cx-1][cy] == BLANK)
			drawImage(buf, L"TileFrameOn.gres", 5 + 7 * (cx-1), 2 + 4 * (cy), 1 + (cx-1) + 10 * (cy));
		if(cx<=8)
			if(tiles[cx+1][cy] == BLANK)
			drawImage(buf, L"TileFrameOn.gres", 5 + 7 * (cx+1), 2 + 4 * (cy), 1 + (cx+1) + 10 * (cy));
		if(cy>=1)
			if(tiles[cx][cy-1] == BLANK)
			drawImage(buf, L"TileFrameOn.gres", 5 + 7 * (cx), 2 + 4 * (cy-1), 1 + (cx) + 10 * (cy-1));
		if(cy<=5)
			if(tiles[cx][cy+1] == BLANK)
			drawImage(buf, L"TileFrameOn.gres", 5 + 7 * (cx), 2 + 4 * (cy+1), 1 + (cx) + 10 * (cy+1));
	}

	if (placed == 2)
	{
		int cx1 = recentTile[0].x;
		int cy1 = recentTile[0].y;
		int cx2 = recentTile[1].x;
		int cy2 = recentTile[1].y;

		if (cx1 == cx2)
		{
			if (cy1 > cy2)
			{
				if(cy1<=5)
					if(tiles[cx1][cy1+1] == BLANK)
					drawImage(buf, L"TileFrameOn.gres", 5 + 7 * (cx1), 2 + 4 * (cy1+1), 1 + (cx1) + 10 * (cy1+1));
				if(cy2>=1)
					if(tiles[cx1][cy2-1] == BLANK)
					drawImage(buf, L"TileFrameOn.gres", 5 + 7 * (cx1), 2 + 4 * (cy2-1), 1 + (cx1) + 10 * (cy2-1));
			}
			else
			{
				if(cy2<=5)
					if(tiles[cx2][cy2+1] == BLANK)
						drawImage(buf, L"TileFrameOn.gres", 5 + 7 * (cx2), 2 + 4 * (cy2+1), 1 + (cx2) + 10 * (cy2+1));
				if(cy1>=1)
					if(tiles[cx2][cy1-1] == BLANK)
						drawImage(buf, L"TileFrameOn.gres", 5 + 7 * (cx2), 2 + 4 * (cy1-1), 1 + (cx2) + 10 * (cy1-1));
			}
		}
		if (cy1 == cy2)
		{
			if (cx1 > cx2)
			{
				if(cx1<=9)
				drawImage(buf, L"TileFrameOn.gres", 5 + 7 * (cx1+1), 2 + 4 * (cy1), 1 + (cx1+1) + 10 * (cy1));
				if(cx2>=1)
				drawImage(buf, L"TileFrameOn.gres", 5 + 7 * (cx2-1), 2 + 4 * (cy2), 1 + (cx2-1) + 10 * (cy2));
			}
			else
			{
				if(cx2<=9)
				drawImage(buf, L"TileFrameOn.gres", 5 + 7 * (cx2+1), 2 + 4 * (cy2), 1 + (cx2+1) + 10 * (cy2));
				if(cx1>=1)
				drawImage(buf, L"TileFrameOn.gres", 5 + 7 * (cx1-1), 2 + 4 * (cy1), 1 + (cx1-1) + 10 * (cy1));
			}
		}
	}

	//이번 턴에 놓은 타일
	for (int i = 0; i < placed; i++)
		drawTileYellow(recentTile[i].type, buf, recentTile[i].x, recentTile[i].y);

	wchar cServerIP[100];
	swprintf_s(cServerIP, 100, L"%d.%d.%d.%d", state.commIP.b1, state.commIP.b2, state.commIP.b3, state.commIP.b4);
	drawText(buf, cServerIP, 1, 30, 100, Color::Gray);

	drawText(buf, L"════════════════════════════════════════════════════════════════════════════════", 0, 31, 80, Color::White);
	drawImage(buf, getTileName(curT), 3, 33, PLACE_TILE);
	if (placed!=0)
		drawImage(buf, L"EndTurnButtonOn.gres", 32, 33, END_TURN);
	else
		drawImage(buf, L"EndTurnButtonOff.gres", 32, 33);
	drawImage(buf, L"ResetButton.gres", 37, 36, RESET);
	drawImage(buf, L"ImpossibleButton.gres",68, 37);
	drawText(buf, L"회로 배치", 3, 37, 100, Color::White);
	drawText(buf, L"Z,X로 회전", 70, 32, 100, Color::Gray);
	drawText(buf, L"C로 뒤집기", 70, 33, 100, Color::Gray);
	drawText(buf, L"ESC로 취소", 70, 34, 100, Color::Gray);

	//손에 있는 타일
	if(displayCur)
		drawImage(buf, getTileNameYellow(curT), curX - 3, curY - 2);

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
			thread send(sendUDPGameState, &curX, &curY, &displayCur, &leftTime, &curT, recentTile, &placed, state->commIP, &sendUDPKillSwitch);
			send.detach();
			startTurn = true;
		}

		curX = getClick().pos.X;
		curY = getClick().pos.Y;
		//MouseClick k = getClickOnce();
		int co = getClickObject(buf);
		
		//클릭
		if (getClickOnce().type == Left)
		{
			if (1 <= co && co <= 70 && placed <= 3)
			{
				recentTile[placed].type = curT;
				recentTile[placed].x = (co - 1) % 10;
				recentTile[placed].y = (co - 1) / 10;
				placed++;
			}

			if(co==PLACE_TILE)
				displayCur = !displayCur;
			if (co == RESET)
				placed = 0;

			//턴 끝내기
			if (co == END_TURN)
			{
				state->turn = false;
				for (int i = 0; i < placed; i++)
					tiles[recentTile[i].x][recentTile[i].y] = recentTile[i].type;
				placed = 0;
			}
		}


		//타일 돌리기
		int key = getKeyOnce();
		switch (key)
		{
		case Key::kX:
			switch (curT)
			{
			case UD: curT = LR; break;
			case LR: curT = UD; break;
			case UR: curT = RD; break;
			case RD: curT = LD; break;
			case LD: curT = UL; break;
			case UL: curT = UR; break;
			}
			break;
		case Key::kZ:
			switch (curT)
			{
			case UD: curT = LR; break;
			case LR: curT = UD; break;
			case UR: curT = UL; break;
			case UL: curT = LD; break;
			case LD: curT = RD; break;
			case RD: curT = UR; break;
			}
			break;
		case Key::kC:
			if (curT == LR || curT == UD)
			{
				curT = UR; break;
			}
			if (curT == UR || curT == RD || curT == LD || curT == UL)
			{
				curT = LR; break;
			}
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
			thread get(getUDPGameState, &curX, &curY, &displayCur, &leftTime, &curT, recentTile, &placed, &getUDPKillSwitch);
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