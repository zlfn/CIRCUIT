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

