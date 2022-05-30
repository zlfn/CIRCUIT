#pragma once
#include "Tiles.h"
#include "Graphic.h"
#include "Chars.h"

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

struct RecentTile
{
	int x;
	int y;
	Tile type;
};

const wchar* getTileName(Tile t);
const wchar* getTileNameYellow(Tile t);
int drawTile(Tile t, Buffer buf, int x, int y);
int drawTileYellow(Tile t, Buffer buf, int x, int y);
int drawTile(Tile t, Buffer buf, int x, int y, Tile up, Tile left, Tile right, Tile down, int placed);
