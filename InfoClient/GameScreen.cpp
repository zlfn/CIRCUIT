﻿//GNU GPL 3.0 lisence
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

//게임 진행 중 2개의 포트를 이용하게 됩니다.
//UDP DataTransfer 6282 : 자신의 턴 일때 10ms에 한번씩 현재의 보드상태와 마우스 위치를 상대방에게 보냅니다.
//	몇개 유실되도 큰 문제가 없는 데이터이므로 검증이 없습니다.
//TCP TurnExchange 2005 : 자신의 턴을 끝내면 상대방에게 TCPRequest를 송신합니다.
//	자신의 턴이 끝난 직후부터 TCPServe를 엽니다. 타임아웃은 없습니다. 즉 상대방의 접속이 꺼지면 영원히 무한 루프..
//이 문제를 해결하기 위해 UDP 하트비트를 송/수신하여 연결을 확인하려 하였으나 컴퓨터 환경에 따라서 튕기는 현상이 발견되어 취소되었습니다.

#include <thread>
#include <iostream>
#include "Graphic.h"
#include "Scenes.h"
#include "GameState.h"
#include "Input.h"
#include "Network.h"
#include "Tiles.h"

using namespace std;

//턴 종료 결과
enum Result
{
	PASS,
	YOULOSE,
	YOUWIN,
	IMPOSSIBLE,
} recvResult;

//턴 교환 여부
bool exchanged;

//시동 변수
bool start = false;
bool startTurn = false;

//승리 패배 여부
bool lose = false;
bool win = false;

//불가능선언 여부
bool impossible = false;

//킬 스위치
bool killSwitch = false;

//커서 변수
int curX;
int curY;
bool displayCur = false;
Tile curT = LR;

//둔 타일 개수
int placed = 0;
int receivedPlace = 0;

//남은 타일 개수
int leftTile = 16;

//남은 자기 시간
int leftTime = 150;
int receivedLeftTime = 150;

//하트비트 체크
bool isConnected = true;

//타일 목록
Tile tiles[10][7];

//킬스위치 목록
bool getUDPKillSwitch = false;
bool sendUDPKillSwitch = false;

//자신이 이번턴에 둔 타일과 상대가 이번턴에 둔 타일.
RecentTile recentTile[100];
RecentTile receivedTile[100];

//오브젝트 클릭 테이블
//타일은 1-70을 할당
const int PLACE_TILE = 71;
const int END_TURN = 72;
const int RESET_BUTTON = 73;
const int IMPOSSIBLE_BUTTON = 74;
const int GIVEUP_BUTTON = 75;
const int RESTART_BUTTON = 76;

/// <summary>
/// 이번 턴에 설치한 타일 배열과 개수를 리셋합니다.
/// </summary>
/// <returns>정상적으로 리셋되면 0이 반환됩니다.</returns>
int resetRecentTiles()
{
	placed = 0;
	for (int i = 0; i < 100; i++)
	{
		recentTile[i].x = 0;
		recentTile[i].y = 0;
		recentTile[i].type = BLANK;
	}
	return 0;

}

/// <summary>
/// 게임 변수들을 모두 초기상태로 돌립니다.
/// </summary>
/// <returns>정상적으로 실행되면 0이 반환됩니다.</returns>
int resetVals()
{
	getUDPKillSwitch = true;
	sendUDPKillSwitch = true;
	killSwitch = true;
	start = false;
	impossible = false;
	displayCur = false;
	isConnected = true;
	win = false;
	lose = false;
	leftTime = 150;
	receivedLeftTime = 150;
	recvResult = PASS;
	placed = false;

	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 7; j++)
			tiles[i][j] = BLANK;

	tiles[4][3] = RD;
	tiles[5][3] = UL;

	for (int i = 0; i < 100; i++)
	{
		recentTile[i].x = 0;
		recentTile[i].y = 0;
		recentTile[i].type = BLANK;
	}

	for (int i = 0; i < 100; i++)
	{
		receivedTile[i].x = 0;
		receivedTile[i].y = 0;
		receivedTile[i].type = BLANK;
	}

	return 0;
}


/// <summary>
/// 입력 변수들을 리셋합니다.
/// </summary>
/// <returns>정상적으로 종료되면 0이 반환됩니다.</returns>
int resetInputs()
{
	curX = 0;
	curY = 0;
	curT = LR;
	displayCur = false;
	return 0;
}

/// <summary>
/// 자기 턴인 동안 1초에 시간을 1씩 줄입니다.
/// </summary>
/// <param name="time">시간 포인터</param>
/// <param name="turn">자기턴인지 여부</param>
void decreaseTime(int* time, bool* turn)
{
	for (;;)
	{
		if (!*turn) break;
		Sleep(1000);
		*time -= 1;
	}
}

/// <summary>
/// UDP를 통해 상대에게 실시간 게임 상태를 송신합니다.
/// </summary>
/// <param name="curX">마우스 X</param>
/// <param name="curY">마우스 Y</param>
/// <param name="displayCur">타일 집었는지 여부</param>
/// <param name="leftTime">남은 시간</param>
/// <param name="curT">집은 타일 종류</param>
/// <param name="recentTile">이번 턴에 설치한 타일 배열</param>
/// <param name="placed">이번 턴에 설치한 타일 수</param>
/// <param name="ip">보낼 ip주소</param>
/// <param name="killSwitch">킬스위치 포인터</param>
void sendUDPGameState(int *curX, int *curY, bool *displayCur, int* leftTime, Tile *curT, RecentTile* recentTile, int *placed, IPV4 ip, bool* killSwitch)
{
	for (;;)
	{
		if (*killSwitch) break;
		char buffer[2048];
		sprintf_s(buffer, 2048, "%d %d %d %d %d %d",
			*curX, *curY, *displayCur, *leftTime, *curT, *placed);
		char temp[1024];
		for (int i = 0; i < *placed; i++)
		{
			sprintf_s(temp, 1024, " %d %d %d", recentTile[i].x, recentTile[i].y, recentTile[i].type);
			strcat_s(buffer, 2048, temp);
		}

		sendUDPMessage(buffer, ip, 6282);
		Sleep(10);
	}
}

/// <summary>
/// UDP를 통해 상대에게 실시간 게임 상태를 수신합니다.
/// </summary>
/// <param name="curX">마우스 X</param>
/// <param name="curY">마우스 Y</param>
/// <param name="displayCur">타일 집었는지 여부</param>
/// <param name="leftTime">상대방 남은 시간</param>
/// <param name="curT">상대방 집은 타일 종류</param>
/// <param name="recentTile">상대방이 설치한 타일 배열</param>
/// <param name="placed">상대방이 설치한 타일 개수</param>
/// <param name="killSwitch">킬 스위치 포인터</param>
void getUDPGameState(int *curX, int *curY, bool *displayCur, int* leftTime, Tile *curT, RecentTile* recentTile, int* placed, bool* killSwitch)
{
	for (;;)
	{
		if (*killSwitch) break;
		char buffer[2048];
		IPV4 temp(0, 0, 0, 0);
		if (receiveUDPMessage(buffer, &temp, 100, 6282) == 0)
		{
			int a = 0;
			int readText = 0;
			int cur = 0;

			sscanf_s(buffer, "%d %d %d %d %d %d %n", &a, curY, displayCur, leftTime, curT, placed, &readText);
			*curX = a; //<curX를 그대로 sscanf_s로 넣으니까 안되더군요, 1시간 동안의 깊은 고민 끝에 그냥 이따구로 처리하기로 결정했습니다.
			//원인은 모르겠는데 작동하면 됐죠 몰라몰라

			cur += readText;

			for (int i = 0; i < *placed; i++)
			{
				sscanf_s(buffer + cur, "%d %d %d %n", &recentTile[i].x, &recentTile[i].y, &recentTile[i].type ,&readText);
				cur += readText;
			}
		}
	}
}

/// <summary>
/// 자기 턴이 아닐 때 상대방이 보내주는 턴을 대기합니다.
/// </summary>
/// <param name="result">승리여부등이 들어가는 Result 변수 포인터</param>
/// <param name="recentTile">상대방이 설치한 타일 배열</param>
/// <param name="placed">상대방이 설치한 타일 개수</param>
/// <param name="exchanged">턴을 받았는지 여부 포인터</param>
void waitTurn(Result* result, RecentTile* recentTile, int* placed, bool* exchanged)
{
	char buffer[256];
	IPV4 tip(0, 0, 0, 0);
	serveTCP("OK", buffer, 256, &tip, 2005);
	int cur = 0;
	int readText;
	sscanf_s(buffer, "%d %d %n", result, placed, &readText);
	cur += readText;
	for (int i = 0; i < *placed; i++)
	{
		sscanf_s(buffer + cur, "%d %d %d %n", &recentTile[i].x, &recentTile[i].y, &recentTile[i].type, &readText);
		cur += readText;
	}
	*exchanged = true;
}

/// <summary>
/// (자기 턴을 종료할 때) 상대방에게 턴을 보냅니다.
/// </summary>
/// <param name="result">턴 결과 (승리, 패배, 패스, 불가능 선언)</param>
/// <param name="recentTile">자기 턴에 설치한 타일 배열</param>
/// <param name="placed">자기 턴에 설치한 타일 개수</param>
/// <param name="ip">보낼 ip 주소</param>
void sendTurn(Result result, RecentTile* recentTile, int placed, IPV4 ip)
{
	char buffer[2048];
	sprintf_s(buffer, 2048, "%d %d", result, placed);
	char temp[1024];
	for (int i = 0; i < placed; i++)
	{
		sprintf_s(temp, 1024, " %d %d %d", recentTile[i].x, recentTile[i].y, recentTile[i].type);
		strcat_s(buffer, 2048, temp);
	}
	char recv[2048];
	requestTCP(buffer, recv, 256, ip, 2005);
}

int drawGameScreen(Buffer buf, GameState state)
{
	wchar leftTimeS[100];
	if(state.turn)
		swprintf_s(leftTimeS, 100, L" %03d ", leftTime);
	else
		swprintf_s(leftTimeS, 100, L" %03d ", receivedLeftTime);

	if (impossible)
	{
		if (state.turn)
		{
			drawText(buf, L"   불가능    ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒", 0, 0, 100, Color::Yellow);
			drawText(buf, leftTimeS, 74, 0, 100, Color::LightRed);
		}
		else
		{
			drawText(buf, L" 상대방 차례 ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒", 0, 0, 100, Color::Yellow);
			drawText(buf, leftTimeS, 74, 0, 100, Color::Gray);
		}

	}
	else
	{
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
					if(tiles[cx1+1][cy1] == BLANK)
						drawImage(buf, L"TileFrameOn.gres", 5 + 7 * (cx1+1), 2 + 4 * (cy1), 1 + (cx1+1) + 10 * (cy1));
				if(cx2>=1)
					if(tiles[cx2-1][cy2] == BLANK)
						drawImage(buf, L"TileFrameOn.gres", 5 + 7 * (cx2-1), 2 + 4 * (cy2), 1 + (cx2-1) + 10 * (cy2));
			}
			else
			{
				if(cx2<=9)
					if(tiles[cx2+1][cy2] == BLANK)
						drawImage(buf, L"TileFrameOn.gres", 5 + 7 * (cx2+1), 2 + 4 * (cy2), 1 + (cx2+1) + 10 * (cy2));
				if(cx1>=1)
					if(tiles[cx1-1][cy1] == BLANK)
						drawImage(buf, L"TileFrameOn.gres", 5 + 7 * (cx1-1), 2 + 4 * (cy1), 1 + (cx1-1) + 10 * (cy1));
			}
		}
	}

	if (impossible)
	{
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 7; j++)
				if(tiles[i][j] == BLANK)
				drawImage(buf, L"TileFrameOn.gres", 5 + 7 * i, 2 + 4 * j, 1 + i + 10 * j);
	}

	//이번 턴에 놓은 타일
	for (int i = 0; i < placed; i++)
		drawTileYellow(recentTile[i].type, buf, recentTile[i].x, recentTile[i].y);

	wchar cServerIP[100];
	swprintf_s(cServerIP, 100, L"%d.%d.%d.%d", state.commIP.b1, state.commIP.b2, state.commIP.b3, state.commIP.b4);
	drawText(buf, cServerIP, 1, 30, 100, Color::Gray);

	drawText(buf, L"════════════════════════════════════════════════════════════════════════════════", 0, 31, 80, Color::White);

	drawText(buf, L"회로 배치", 3, 33, 100, Color::White);
	drawImage(buf, getTileName(curT), 3, 34, PLACE_TILE);
	wchar leftTileS[100];
	swprintf_s(leftTileS, 100, L"%d", leftTile - placed);
	if(leftTile-placed>=10)
		drawText(buf, leftTileS, 8, 37, 100, Color::LightGray);
	else
		drawText(buf, leftTileS, 9, 37, 100, Color::LightGray);

	if (placed!=0)
		drawImage(buf, L"EndTurnButtonOn.gres", 32, 33, END_TURN);
	else
		drawImage(buf, L"EndTurnButtonOff.gres", 32, 33);

	drawImage(buf, L"ResetButton.gres", 37, 36, RESET_BUTTON);
	if(impossible)
		drawImage(buf, L"GiveupButton.gres",68, 37, GIVEUP_BUTTON);
	else
		drawImage(buf, L"ImpossibleButton.gres",68, 37, IMPOSSIBLE_BUTTON);

	drawText(buf, L"Z,X로 회전", 70, 32, 100, Color::Gray);
	drawText(buf, L"C로 뒤집기", 70, 33, 100, Color::Gray);
	drawText(buf, L"ESC로 취소", 70, 34, 100, Color::Gray);

	//손에 있는 타일
	if (displayCur)
		drawImage(buf, getTileNameYellow(curT), curX - 3, curY - 2);
	//커서
	else if (!state.turn)
		drawImage(buf, L"Cursor.gres", curX, curY);

	if (win)
		drawImage(buf, L"Win.gres", 0, 0);
	if (lose)
		drawImage(buf, L"Lose.gres", 0, 0);

	//재시작은 없는걸로
	/*if (win || lose)
		drawImage(buf, L"RestartButton.gres", 35, 20, RESTART_BUTTON);*/
	
	return 0;
}

int playGameScreen(Buffer buf, GameState* state)
{
	//장면 시작할 때
	if (!start)
	{
		resetVals();
		start = true;
	}

	//자기 턴일때
	else if (state->turn && !win && !lose) 
	{
		//턴 시작할 때
		if (!startTurn)
		{
			resetRecentTiles();
			sendUDPKillSwitch = false;
			getUDPKillSwitch = true;
			thread send(sendUDPGameState, &curX, &curY, &displayCur, &leftTime, &curT, recentTile, &placed, state->commIP, &sendUDPKillSwitch);
			thread time(decreaseTime, &leftTime, &state->turn);
			send.detach();
			time.detach();
			startTurn = true;
		}

		//시간 오버
		if (leftTime <= 0)
		{
				state->turn = false;
				sendUDPKillSwitch = true;
				getUDPKillSwitch = true;
				for (int i = 0; i < placed; i++)
					tiles[recentTile[i].x][recentTile[i].y] = recentTile[i].type;

				lose = true;
				sendTurn(YOUWIN, recentTile, placed, state->commIP);
				resetRecentTiles();
				resetInputs();
		}


		if (leftTile - placed <= 0)
			displayCur = false;

		curX = getClick().pos.X;
		curY = getClick().pos.Y;
		//MouseClick k = getClickOnce();
		int co = getClickObject(buf);
		
		//클릭
		if (getClickOnce().type == Left)
		{
			//타일 배치
			if (1 <= co && co <= 70 && (placed <= 2 || impossible) && displayCur)
			{
				recentTile[placed].type = curT;
				recentTile[placed].x = (co - 1) % 10;
				recentTile[placed].y = (co - 1) / 10;
				placed++;
			}

			//타일 버튼
			if(co==PLACE_TILE)
				displayCur = !displayCur;

			//타일 리셋
			if (co == RESET_BUTTON)
				placed = 0;

			//턴 끝내기
			if (co == END_TURN)
			{
				state->turn = false;
				startTurn = false;
				leftTile -= placed;
				sendUDPKillSwitch = true;
				getUDPKillSwitch = false;
				for (int i = 0; i < placed; i++)
					tiles[recentTile[i].x][recentTile[i].y] = recentTile[i].type;

				if (isCompleted(tiles, 16 - leftTile))
				{
					win = true;
					sendTurn(YOULOSE, recentTile, placed, state->commIP);
					resetRecentTiles();
					resetInputs();
				}
				else
				{
					if (!impossible) {
						sendTurn(PASS, recentTile, placed, state->commIP);
						resetRecentTiles();
						resetInputs();
					}
					else {
						lose = true;
						sendTurn(YOUWIN, recentTile, placed, state->commIP);
						resetRecentTiles();
						resetInputs();
					}
				}
			}
			
			//불가능 선언
			if (co == IMPOSSIBLE_BUTTON)
			{
				state->turn = false;
				startTurn = false;
				sendUDPKillSwitch = true;
				getUDPKillSwitch = false;
				impossible = true;
				sendTurn(IMPOSSIBLE, recentTile, placed, state->commIP);
				resetRecentTiles();
				resetInputs();
			}

			if (co == GIVEUP_BUTTON)
			{
				state->turn = false;
				sendUDPKillSwitch = true;
				getUDPKillSwitch = true;
				for (int i = 0; i < placed; i++)
					tiles[recentTile[i].x][recentTile[i].y] = recentTile[i].type;

				lose = true;
				sendTurn(YOUWIN, recentTile, placed, state->commIP);
				resetRecentTiles();
				resetInputs();
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
	else if (!win && !lose)//상대 턴일때
	{
		//턴 시작할때
		if (!startTurn)
		{
			resetRecentTiles();
			sendUDPKillSwitch = true;
			getUDPKillSwitch = false;
			thread get(getUDPGameState, &curX, &curY, &displayCur, &receivedLeftTime, &curT, recentTile, &placed, &getUDPKillSwitch);
			get.detach();
			thread wait(waitTurn, &recvResult, receivedTile, &receivedPlace, &exchanged);
			wait.detach();
			startTurn = true;
		}

		//턴 교환되면
		if (exchanged)
		{
			//별일 없을 때
			if (recvResult == PASS)
			{
				sendUDPKillSwitch = false;
				getUDPKillSwitch = true;
				leftTile -= placed;
				for (int i = 0; i < receivedPlace; i++)
					tiles[receivedTile[i].x][receivedTile[i].y] = receivedTile[i].type;
				resetRecentTiles();
				resetInputs();
				startTurn = false;
				receivedPlace = 0;
				exchanged = false;
				state->turn = true;
			}

			//불가능 선언 받았을 때
			if (recvResult == IMPOSSIBLE)
			{
				sendUDPKillSwitch = false;
				getUDPKillSwitch = true;
				resetRecentTiles();
				resetInputs();
				startTurn = false;
				receivedPlace = 0;
				leftTime = 150;
				exchanged = false;
				impossible = true;
				state->turn = true;
			}

			//이겼을 때
			if (recvResult == YOUWIN)
			{
				for (int i = 0; i < receivedPlace; i++)
					tiles[receivedTile[i].x][receivedTile[i].y] = receivedTile[i].type;
				resetRecentTiles();
				resetInputs();
				sendUDPKillSwitch = true;
				getUDPKillSwitch = true;
				win = true;
			}

			//졌을 때
			if (recvResult == YOULOSE)
			{
				for (int i = 0; i < placed; i++)
					tiles[recentTile[i].x][recentTile[i].y] = recentTile[i].type;
				resetRecentTiles();
				resetInputs();
				sendUDPKillSwitch = true;
				getUDPKillSwitch = true;
				lose = true;
			}
		}
	}
	else
	{
		int co = getClickObject(buf);
		
		//클릭
		if (getClickOnce().type == Left && co == RESTART_BUTTON) 
		{
			resetVals();
			resetGameState();
		}
			
	}


	//연결 끊겼을 때 (미사용)
	if (isConnected == false)
	{
		killSwitch = true;
		state->scene = Error;
	}

	return 0;
}