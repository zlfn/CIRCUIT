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

#include "GameState.h"

/// <summary>
/// ���� ���¸� �����ϴ� ���������Դϴ�.
/// </summary>
static GameState globalGameState;

GameState getGameState()
{
	return globalGameState;
}

int resetGameState()
{
	GameState g;

	//���� �ʱ�ȭ
	g.scene = Main;
	g.setting.noSpaceWindow = true;

	g.setting.refresh = false;
	g.setting.parallelRefresh = true;
	g.setting.refreshInterval = 1;
	g.setting.refreshThreadsCount = 1;

	g.setting.frontBufferDithering = true;
	g.setting.ditheringSize = 10;

	g.setting.renderThreadsCount = 1;
	g.setting.showFPS = false;

	globalGameState = g;
	return 0;
}