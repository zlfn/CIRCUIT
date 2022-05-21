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
#include <wchar.h> 
typedef wchar_t wchar;

namespace Color
{
	const int Black = 0;
	const int Blue = 1;
	const int Green = 2;
	const int Aqua = 3;
	const int Red = 4;
	const int Purple = 5;
	const int Yello = 6;
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
/// �Էµ� wchar�� �ѱ����� �Ǵ��մϴ�.
/// </summary>
/// <param name="letter">�Ǻ��� wchar</param>
/// <returns>wchar�� �ѱ��̸� TRUE, �ƴҰ�� FALSE�� ��ȯ�˴ϴ�.</returns>
extern bool isWide(wchar letter);
