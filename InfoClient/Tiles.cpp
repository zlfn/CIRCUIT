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

#include "Tiles.h"
#include "Chars.h"
#include <iostream>

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

bool isCompleted(Tile (* tiles)[7], int placed)
{
	int x = 5, y = 3, cnt = 0;
	Tile cur_tile = tiles[x][y];
	Direction cur_direction = L;
	Direction next_direction = U;
	Direction tile_dir1, tile_dir2;

	while (x!=4 || y!=3)
	{
		switch (next_direction)
		{
		case L: x = x - 1; break;
		case R: x = x + 1; break;
		case U: y = y - 1; break;
		case D: y = y + 1; break;
		}
		if (x < 0 || x > 9 || y < 0 || y > 6) return false;

		cur_tile = tiles[x][y];

		cur_direction = (Direction)((-1) * next_direction);

		if (cur_tile == LR) { tile_dir1 = L; tile_dir2 = R; }
		else if (cur_tile == UD) { tile_dir1 = U; tile_dir2 = D; }
		else if (cur_tile == UR) { tile_dir1 = U; tile_dir2 = R; }
		else if (cur_tile == RD) { tile_dir1 = R; tile_dir2 = D; }
		else if (cur_tile == LD) { tile_dir1 = L; tile_dir2 = D; }
		else if (cur_tile == UL) { tile_dir1 = U; tile_dir2 = L; }
		else if (cur_tile == BLANK) {
			return false;
		}

		if (cur_direction == tile_dir1) next_direction = tile_dir2;
		else if (cur_direction == tile_dir2) next_direction = tile_dir1;
		else return false;

		cnt++;
	}

	if (cnt >= placed+1) return true;
	else return false;
}


