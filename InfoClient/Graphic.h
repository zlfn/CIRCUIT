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
#include "Chars.h"
#include <windows.h>

struct Size
{
	int x;
	int y;
};
struct Buffer
{
	HANDLE screen;
	wchar** textBuf;
	int** colorBuf;
	Size size;
};

/// <summary>
/// ���� �����͸� �̿��Ͽ� ��ũ�� ���۸� �������ϴ� �Լ��Դϴ�.
/// <para>�����忡 �ֱ� ���� �Լ��Դϴ�.</para>
/// </summary>
/// <param name="buf">�������� ����</param>
/// <param name="y_start">�������� ������ y��ǥ</param>
/// <param name="y_end">�������� ���� y��ǥ</param>
extern void renderTH(Buffer bbuf, int x, int y);

/// <summary>
/// ���� �����͸� �̿��Ͽ� ��ũ�� ���۸� �������ϴ� �Լ��Դϴ�.
/// <para>����Ʈ ���ۿ� �� ���۸� ���Ͽ� ����� ����ȭ�մϴ�. ȭ���� ����κ��� ���� ��� �ſ� ū ���ɻ��� ������ �ֽ��ϴ�.</para>
/// <para>�����忡 �ֱ����� �Լ��Դϴ�.</para>
/// </summary>
/// <param name="buf">�������� ����</param>
/// <param name="y_start">�������� ������ y��ǥ</param>
/// <param name="y_end">�������� ���� y��ǥ</param>
extern void renderTH_OP(Buffer bbuf, Buffer fbuf, int x, int y);

/// <summary>
/// ������ ������ �̿��Ͽ� ��ũ�����۸� �������մϴ�.
/// <para>������ ������ŭ ��Ƽ�������� �̿��ϸ�, 2�� �̻��� ������ ���� 10% ������ ���� ����� ����ϴ�.</para>
/// </summary>
/// <param name="buf">�������� ����</param>
/// <param name="threadSize">����� ������ ��. �ݵ�� y���� ������� �մϴ�.</param>
/// <returns>���������� ������ �Ǿ��ٸ� 0�� ���ϵ˴ϴ�.</returns>
extern int renderBuffer(Buffer bbuf, int threads);


/// <summary>
/// ������ ������ �̿��Ͽ� ��ũ�����۸� �������մϴ�.
/// <para>����ۿ� ����Ʈ���۸� ���Ͽ� ����ȭ�ϸ�, ȭ���� ���� �κ��� �۴ٸ� �ſ� ū ���ɻ��� ������ �����մϴ�.</para>
/// <para>������ ������ŭ ��Ƽ�������� �̿��ϸ�, 2�� �̻��� ������ �̿�� 10%������ ���� ����� �ֽ��ϴ�.</para>
/// </summary>
/// <param name="buf">�������� ����</param>
/// <param name="threadSize">����� ������ ��. �ݵ�� y���� ������� �մϴ�.</param>
/// <returns>���������� ������ �Ǿ��ٸ� 0�� ���ϵ˴ϴ�.</returns>
extern int renderBuffer(Buffer bbuf, Buffer fbuf, int threads);


/// <summary>
/// ���۸��� ���� Buffer������ �����Ҵ��Ͽ� �����ɴϴ�.
/// </summary>
/// <param name="x">��ũ�� ������ x ������</param>
/// <param name="y">��ũ�� ������ y ������</param>
/// <returns>�Ҵ�� Buffer������ ��ȯ�˴ϴ�.</returns>
extern Buffer getBuffer(int x, int y);

/// <summary>
/// ���۸� ȭ�鿡 ǥ���մϴ�.
/// </summary>
/// <param name="bbuf">ǥ���� ����</param>
/// <returns>���������� ǥ�õǾ��ٸ� 0�� ���ϵ˴ϴ�.</returns>
extern int swapBuffer(Buffer buf);

/// <summary>
/// ���۸��� ���� Buffer ������ �����Ҵ��� �����մϴ�.
/// </summary>
/// <param name="buf">������ Buffer ����</param>
/// <returns>���������� �����Ǿ��ٸ� 0�� ��ȯ�˴ϴ�.</returns>
extern int freeBuffer(Buffer buf);

/// <summary>
/// ���ۿ� Ư�� �ʺ��� ���� �����մϴ�.
/// </summary>
/// <param name="buf">���� ����� ����</param>
/// <param name="text">����� �ؽ�Ʈ, �ѱۿ� ������ �������� �ʽ��ϴ�.</param>
/// <param name="x">����� ������ x��ǥ</param>
/// <param name="y">����� ������ y��ǥ</param>
/// <param name="width">�ؽ�Ʈ�� �ʺ�</param>
/// <param name="color">�ؽ�Ʈ�� ����</param>
/// <returns>���������� �����ߴٸ� 0�� ��ȯ�˴ϴ�.
/// <para>�ؽ�Ʈ�� ������ �Ѿ�ٸ� 1�� ��ȯ�˴ϴ�.</para></returns>
extern int drawText(Buffer buf, const wchar* text, int x, int y, int width, int color);

/// <summary>
/// ������ �����͸� �����մϴ�.
/// <para>��ũ�� ���۴� �������� �ʽ��ϴ�.</para>
/// </summary>
/// <param name="buf">������ ����</param>
/// <returns>���������� ���µǸ� 0�� ��ȯ�˴ϴ�. </returns>
extern int resetBuffer(Buffer bbuf);

/// <summary>
/// �� ���۸� ����ȭ�մϴ�. ���� ���۸� ������ �ʼ����Դϴ�.
/// </summary>
/// <param name="buf">����� ����</param>
/// <param name="fbuf">������ ����</param>
/// <returns>���������� ����ȭ�Ǹ� 0�� ��ȯ�˴ϴ�.</returns>
extern int syncroBuffer(Buffer client, Buffer server);
