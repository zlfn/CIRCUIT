#include "Tiles.h"
#include "Chars.h"

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

const wchar* getTileNameYellow(Tile t)
{
	switch (t)
	{
	case LR:
		return L"TileLRY.gres"; break;
	case UD:
		return L"TileUDY.gres"; break;
	case UR:
		return L"TileURY.gres"; break;
	case RD:
		return L"TileRDY.gres"; break;
	case LD:
		return L"TileLDY.gres"; break;
	case UL:
		return L"TileULY.gres"; break;
	}
	return L"";

}

const wchar* getTileOriginName(Tile t)
{
	switch (t)
	{
	case LR:
		return L"TileLR"; break;
	case UD:
		return L"TileUD"; break;
	case UR:
		return L"TileUR"; break;
	case RD:
		return L"TileRD"; break;
	case LD:
		return L"TileLD"; break;
	case UL:
		return L"TileUL"; break;
	}
	return L"";
}

wchar* getTileName(Tile t, Buffer buf, Tile up, Tile left, Tile right, Tile down)
{
	wchar* name = new wchar[100];
	name[0] = L'\0';
	wcscat_s(name, 100, getTileOriginName(t));

	switch (t)
	{
	case LR:
		if (left == UR || left == LR || left == RD)
			wcscat_s(name, 100, L"L");
		if (right == UL || right == LR || right == LD)
			wcscat_s(name, 100, L"R");
		break;
	case UD:
		if (up == LD || up == UD || up == RD)
			wcscat_s(name, 100, L"U");
		if (down == UL || down == UD || down == UR)
			wcscat_s(name, 100, L"D");
		break;
	case UR:
		if (up == LD || up == UD || up == RD)
			wcscat_s(name, 100, L"U");
		if (right == UL || right == LR || right == LD)
			wcscat_s(name, 100, L"R");
		break;
	case RD:
		if (right == UL || right == LR || right == LD)
			wcscat_s(name, 100, L"R");
		if (down == UL || down == UD || down == UR)
			wcscat_s(name, 100, L"D");
		break;
	case LD:
		if (left == UR || left == LR || left == RD)
			wcscat_s(name, 100, L"L");
		if (down == UL || down == UD || down == UR)
			wcscat_s(name, 100, L"D");
		break;
	case UL:
		if (up == LD || up == UD || up == RD)
			wcscat_s(name, 100, L"U");
		if (left == UR || left == LR || left == RD)
			wcscat_s(name, 100, L"L");
		break;
	}

	wcscat_s(name, 100, L".gres");

	return name;
}

int drawTile(Tile t, Buffer buf, int x, int y)
{
	if(t!=BLANK)
		drawImage(buf, getTileName(t), 5 + 7 * x, 2 + 4 * y, 0);
	return 0;
}

int drawTileYellow(Tile t, Buffer buf, int x, int y)
{
	if(t!=BLANK)
		drawImage(buf, getTileNameYellow(t), 5 + 7 * x, 2 + 4 * y, 0);
	return 0;
}

int drawTile(Tile t, Buffer buf, int x, int y, Tile up, Tile left, Tile right, Tile down, int placed)
{
	if (t != BLANK)
		drawImage(buf, getTileName(t, buf, up, left, right, down), 5 + 7 * x, 2 + 4 * y, 0);
	else
		if ((up != BLANK || left != BLANK || right != BLANK || down != BLANK) && placed == 0)
			drawImage(buf, L"TileFrameOn.gres", 5 + 7 * x, 2 + 4 * y, 1 + x + 10 * y);
		
	return 0;
}

