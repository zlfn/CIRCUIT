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

#pragma once
#include <Windows.h>
#include "Graphic.h"

enum MouseClickType
{
	None,
	Left,
	Right
};

struct MouseClick
{
	COORD pos;
	MouseClickType type;
};

/// <summary>
/// <para>�Է������� �����ͼ� �����Ϳ� ����ϴ� ���� ���� �ݺ��մϴ�.</para>
/// <para>�����忡 �ֱ� ���� �Լ��Դϴ�.</para>
/// </summary>
/// <param name="c">�Է��� ������� MouseClick ������</param>
void getClickTH(MouseClick* msPointer);

/// <summary>
/// <para>���콺 �Է��� �޴� �����带 �����մϴ�.</para>
/// <para>������� detach�Ǿ� ���α׷� ����ñ��� �����˴ϴ�.</para>
/// <para>�� ���α׷����� �ι� �̻� �� �Լ��� ����Ǹ� ������ �߻��� �� �ֽ��ϴ�.</para>
/// </summary>
/// <returns>���������� ����Ǿ��ٸ� 0�� ��ȯ�˴ϴ�.</returns>
int startGetClick();

/// <summary>
/// ���콺 �Է� ���� �޾ƿɴϴ�.
/// <para>startGetClick()�� �ݵ�� ����Ǿ���մϴ�.</para>
/// </summary>
/// <returns>���콺 �Է� ��</returns>
MouseClick getClick();
