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

//wchar�� ����� ���ڿ� ���õ� ������ ��Ƶ� ��������Դϴ�.
/* �͹̳ο��� �ѱ� ����� ����������, char ������ ���� ���� ������ �ѱ��� Ŀ�� �ؽ�Ʈ �̸�Ƽ�ܵ� ǥ���ϱ� ��ƽ��ϴ�.
* ������ �� ������ ������ ��� Ư���� (Graphics.h ����) �ݵ�� �ѱ��� �����ν� �����ؾ�����.
* ���� wchar�� �̿��մϴ�. �����ڵ�, wide character�� ������ �� ����.
* 
* �� �ܿ� �͹̳� ���� ��� �� �پ��� ���� ���� ����� �� ������Ͽ� ���ǵǾ��ֽ��ϴ�.*/

#pragma once
#include <wchar.h>

//������ wchar_t�� �ʹ� ġ�� ������ �ʳ���?
typedef wchar_t wchar;

/// <summary>
/// �����ڵ带 ��Ƶ� namespace�Դϴ�.
/// <para>�� ���ӿ��� Black�� ������ ������ �ϸ�, ������ ���� �ʽ��ϴ�.</para>
/// </summary>
namespace Color
{
	const int Black = 0;
	const int Blue = 1;
	const int Green = 2;
	const int Aqua = 3;
	const int Red = 4;
	const int Purple = 5;
	const int Yellow = 6;
	const int LightGray = 7;
	const int Gray = 8;
	const int LightBlue = 9;
	const int LightGreen = 10;
	const int LightAqua = 11;
	const int LightRed = 12;
	const int LightPurple = 13;
	const int LightYellow = 14;
	const int White = 15;
}

/// <summary>
/// �ֿܼ��� �ѱ��� ����ϱ� ���� �⺻ ������ �����մϴ�.
/// </summary>
/// <returns>���������� ����Ǿ��ٸ� 0�� ��ȯ�մϴ�.</returns>
extern int initWchar(void);

/// <summary>
/// �Էµ� wchar�� �ֿܼ��� ��ĭ�� �����ϴ��� Ȯ���մϴ�.
/// </summary>
/// <param name="letter">�Ǻ��� wchar</param>
/// <returns>wchar�� ��ĭ�� �����ϸ� TRUE, �ƴϸ� FALSE�� ��ȯ�˴ϴ�.</returns>
extern bool isWide(wchar letter);
