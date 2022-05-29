#include "Graphic.h"
#include "GameState.h"
#include "Input.h"
#include "Network.h"

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

int curX;
int curY;
Tile curT = LR;


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
	if (state.turn)
		drawText(buf, L"   내 차례   ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒", 0, 0, 100, Color::Green);
	else
		drawText(buf, L" 상대방 차례 ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒", 0, 0, 100, Color::Gray);

	drawText(buf, L" 25 ", 74, 0, 100, Color::Green);

	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOff.gres", 5 + 7 * i, 3);
	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOff.gres", 5 + 7 * i, 7);
	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOff.gres", 5 + 7 * i, 11);
	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOff.gres", 5 + 7 * i, 15);
	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOff.gres", 5 + 7 * i, 19);
	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOff.gres", 5 + 7 * i, 23);
	for (int i = 0; i < 10; i++)
		drawImage(buf, L"TileFrameOff.gres",  5 + 7 * i, 27);
	drawImage(buf, L"TileRDR.gres",33,15);
	drawImage(buf, L"TileULL.gres",40,15);

	drawImage(buf, getTileName(curT), curX - 3, curY - 2);

	wchar cServerIP[100];
	swprintf_s(cServerIP, 100, L"통신중: %d.%d.%d.%d", state.commIP.b1, state.commIP.b2, state.commIP.b3, state.commIP.b4);
	drawText(buf, cServerIP, 0, 39, 100, Color::Gray);
	return 0;
}

int playGameScreen(Buffer buf, GameState* state)
{
	curX = getClick().pos.X;
	curY = getClick().pos.Y;

	if (getKeyOnce() == Key::kX)
		curT = UD;
	if (getKeyOnce() == Key::kZ)
		curT = LR;


	return 0;
}