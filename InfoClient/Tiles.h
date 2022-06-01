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

//���ӿ� ���̴� Ÿ�ϵ鿡 ���� �Լ��� ���������� ���ִ� ����Դϴ�.

#pragma once
#include "Tiles.h"
#include "Graphic.h"
#include "Chars.h"

/// <summary>
/// Ÿ���� ������ �ڵ��ϴ� �������Դϴ�.
/// </summary>
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

/// <summary>
/// �̹��Ͽ� ��ġ�� Ÿ���� �����ϱ� ���� ����ü�Դϴ�.
/// </summary>
struct RecentTile
{
	int x;
	int y;
	Tile type;
};

/// <summary>
/// Ÿ���� �����ִ� ������ �̿��ϱ� ���� �������Դϴ�.
/// </summary>
enum Direction
{
	U = -2, L = -1, R = 1, D = 2
};

/// <summary>
/// ȸ�ΰ� �ϼ��Ǿ����� Ȯ���մϴ�.
/// </summary>
/// <param name="tiles">10*7 Ÿ�� �迭</param>
/// <param name="placed">�� ��ġ�� Ÿ�� ����</param>
/// <returns>ȸ�� �ϼ� ���ΰ� ��ȯ�˴ϴ�.</returns>
bool isCompleted(Tile (*tiles)[7], int placed);

/// <summary>
/// Ÿ���� �׷��� ���ҽ� ���ϸ��� ����ϴ�.
/// </summary>
/// <param name="t">Ÿ�� ����</param>
/// <returns>�׷��� ���ҽ� ���� ��</returns>
const wchar* getTileName(Tile t);

/// <summary>
/// Ÿ���� ��������� �׷��� ���ҽ� ���ϸ��� ����ϴ�.
/// </summary>
/// <param name="t">Ÿ�� ����</param>
/// <returns>�׷��� ���ҽ� ���ϸ�</returns>
const wchar* getTileNameYellow(Tile t);

/// <summary>
/// Ư�� ��ġ�� Ÿ���� �׸��ϴ�.
/// </summary>
/// <param name="t">Ÿ�� ����</param>
/// <param name="buf">�׸� ����</param>
/// <param name="x">Ÿ�� ��ǥ (Ÿ�� ��ǥ�� ����)</param>
/// <param name="y">Ÿ�� ��ǥ (Ÿ�� ��ǥ�� ����)</param>
/// <returns>���������� ����Ǿ��ٸ� 0�� ��ȯ�˴ϴ�.</returns>
int drawTile(Tile t, Buffer buf, int x, int y);

/// <summary>
/// Ư�� ��ġ�� ����� Ÿ���� �׸��ϴ�.
/// </summary>
/// <param name="t">Ÿ�� ����</param>
/// <param name="buf">�׸� ����</param>
/// <param name="x">Ÿ�� ��ǥ (Ÿ�� ��ǥ�� ����)</param>
/// <param name="y">Ÿ�� ��ǥ (Ÿ�� ��ǥ�� ����)</param>
/// <returns>���������� ����Ǿ��ٸ� 0�� ��ȯ�˴ϴ�.</returns>
int drawTileYellow(Tile t, Buffer buf, int x, int y);

/// <summary>
/// Ư�� ��ġ�� Ÿ���� �׸���, Ÿ���� �����¿� Ÿ���� �ν��Ͽ� Ÿ�ϵ��� ���ҽ��� �����մϴ�.
/// </summary>
/// <param name="t">Ÿ�� ����</param>
/// <param name="buf">�׸� ����</param>
/// <param name="x">Ÿ�� ��ǥ</param>
/// <param name="y">Ÿ�� ��ǥ</param>
/// <param name="up">���� Ÿ�� ����</param>
/// <param name="left">���� Ÿ�� ����</param>
/// <param name="right">������ Ÿ�� ����</param>
/// <param name="down">�Ʒ��� Ÿ�� ����</param>
/// <param name="placed">��ġ�� Ÿ�� ���� (��ĭ�� ������Ʈ id �Ҵ� üũ��)</param>
/// <returns>���������� ����Ǿ��ٸ� 0�� ��ȯ�˴ϴ�.</returns>
int drawTile(Tile t, Buffer buf, int x, int y, Tile up, Tile left, Tile right, Tile down, int placed);
