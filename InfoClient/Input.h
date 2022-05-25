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

//����� �Է°� ���õ� ������ ���� ��������Դϴ�.
/* �ٵ� �ֿܼ��� �޴������� ����Ű�� �� �׷��� ������ �Է��ϴµ�, 
* �� Ű���� (������ŷ)���� ����Ű�� �����ϴ�. �׷��� �Ⱦ��.
* �׷��� WindowsAPI�� �̿��ؼ� ���콺 Ŭ���� �ޱ�� �߽��ϴ�.
* �ٵ� �и� �Է¹��� ������� �޴� �Ÿ� ���� ���� ���� �ȿ� �־ �������� ���콺 �Է��� �����ؾ� �Ұ� ������,
* ����ϰԵ� �׷��� �ʴ�����. �׷��� ���콺 �Է¸� ������ �޴� �����带 ���� detach() �ϴ� ������� ���콺 Ŭ���� �޽��ϴ�.
*/

#pragma once
#include <Windows.h>
#include "Graphic.h"

/// <summary>
/// ������ ���� Ű�ڵ� ����Դϴ�.
/// </summary>
namespace Key
{
	const int kEnter = 0x0D;
	const int kShift = 0x10;
	const int kCTRL = 0x11;
	const int kALT = 0x12;

	const int k0 = 0x30;
	const int k1 = 0x31;
	const int k2 = 0x32;
	const int k3 = 0x33;
	const int k4 = 0x34;
	const int k5 = 0x35;
	const int k6 = 0x36;
	const int k7 = 0x37;
	const int k8 = 0x38;
	const int k9 = 0x39;

	const int kA = 0x41;
	const int kB = 0x42;
	const int kC = 0x43;
	const int kD = 0x44;
	const int kE = 0x45;
	const int kF = 0x46;
	const int kG = 0x47;
	const int kH = 0x48;
	const int kI = 0x49;
	const int kJ = 0x4A;
	const int kK = 0x4B;
	const int kL = 0x4C;
	const int kM = 0x4D;
	const int kN = 0x4E;
	const int kO = 0x4F;
	const int kP = 0x50;
	const int kQ = 0x51;
	const int kR = 0x52;
	const int kS = 0x53;
	const int kT = 0x54;
	const int kU = 0x55;
	const int kV = 0x56;
	const int kW = 0x57;
	const int kX = 0x58;
	const int kY = 0x59;
	const int kZ = 0x5A;
}

/// <summary>
/// ���콺 Ŭ���� ������ �ڵ��ϴ� �������Դϴ�.
/// <para>None: Ŭ���� ����</para>
/// <para>Left: ���� Ŭ��</para>
/// <para>Right: ������ Ŭ��</para>
/// </summary>
enum MouseClickType
{
	None,
	Left,
	Right
};

/// <summary>
/// ���콺 Ŭ���Է��� �����ϴ� ����ü�Դϴ�.
/// <para>pos: ���콺 Ŭ�� ��ǥ</para>
/// <para>MouseClickType: ���콺 Ŭ���� ����</para>
/// </summary>
struct MouseClick
{
	COORD pos;
	MouseClickType type;
};

/// <summary>
/// <para>�Է������� �����ͼ� �����Ϳ� ����ϴ� ���� ���� �ݺ��մϴ�.</para>
/// <para>�����忡 �ֱ� ���� �Լ��Դϴ�.</para>
/// </summary>
/// <param name="msPointer">�Է��� ������� MouseClick ������</param>
static void getInputTH(MouseClick* msPointer, int* keyPointer);

/// <summary>
/// <para>���콺 �Է��� �޴� �����带 �����մϴ�.</para>
/// <para>������� detach�Ǿ� ���α׷� ����ñ��� �����˴ϴ�.</para>
/// <para>�� ���α׷����� �ι� �̻� �� �Լ��� ����Ǹ� ������ �߻��� �� �ֽ��ϴ�.</para>
/// </summary>
/// <returns>���������� ����Ǿ��ٸ� 0�� ��ȯ�˴ϴ�.</returns>
extern int startGetInput();

/// <summary>
/// ���콺 �Է� ���� �޾ƿɴϴ�.
/// <para>startGetClick()�� �ݵ�� ����Ǿ���մϴ�.</para>
/// </summary>
/// <returns>���콺 �Է� ��</returns>
extern MouseClick getClick();


extern int getKey();

/// <summary>
/// Ŭ���� ��ǥ�� Ŭ�� ���� id�� �޾ƿɴϴ�.
/// �̸� ���ؼ� Ŭ���� ������Ʈ�� �� �� �ֽ��ϴ�.
/// </summary>
/// <param name="buf">������ Ŭ�����۸� ���� ����</param>
/// <returns>���������� ����Ǿ��ٸ� ������ Ŭ�� ���� id�� ��ȯ�˴ϴ�.
/// <para>�Է� ��ǥ�� ���۹����� �Ѿ�ٸ� -1�� ��ȯ�˴ϴ�.</para></returns>
extern int getClickObject(Buffer buf);
