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

#include "Graphic.h"
#include "GameState.h"
#include "Input.h"

const int SHOW_FPS = 1;
const int RENDER_MULTI_THREAD = 2;
const int NO_SPACE_WINDOW = 3;
const int REFRESH = 4;
const int REFRESH_MULTI_THREAD = 5;
const int PARALLEL_REFRESH = 6;
const int FRONT_BUFFER_DITHERING = 7;
const int FAST_DITHER = 8;
const int BACK_BUTTON = 9;


int drawSettingScreen(Buffer buf, GameState state)
{
	drawImage(buf, L"Setting_LOGO.gres",3,0);
	drawText(buf, L"v1.2 - RELEASE", 1, 39, 100, Color::Yellow);
	drawText(buf, L"= FPS ǥ��: ", 5, 10, 100, Color::White);
	if (state.setting.showFPS)
		drawImage(buf, L"On.gres",30, 10, SHOW_FPS);
	else
		drawImage(buf, L"OFF.gres", 30, 10, SHOW_FPS);
	drawText(buf, L"ȭ�� ���� �ϴܿ� FPS�� ǥ���մϴ�.", 7, 11, 100, Color::Gray);


	drawText(buf, L"= ��Ƽ ������ ������: ", 5, 13, 100, Color::White);
	if (state.setting.renderThreadsCount >= 2)
		drawImage(buf, L"On.gres", 30, 13, RENDER_MULTI_THREAD);
	else
		drawImage(buf, L"OFF.gres", 30, 13, RENDER_MULTI_THREAD);
	drawText(buf, L"ȭ�� �������� ��Ƽ�����带 ����մϴ�.", 7, 14, 100, Color::Gray);
	drawText(buf, L"ȭ���� �Ҿ����������� ������ ���˴ϴ�.", 7, 15, 100, Color::Gray);


	drawText(buf, L"= ������� â: ", 5, 17, 100, Color::White);
	if (state.setting.noSpaceWindow)
		drawImage(buf, L"On.gres", 30, 17, NO_SPACE_WINDOW);
	else
		drawImage(buf, L"OFF.gres", 30, 17, NO_SPACE_WINDOW);
	drawText(buf, L"â�� ���� ���ο� ���� �׷����� �����⵵ �մϴ�.", 7, 18, 100, Color::Gray);


	drawText(buf, L"= ��������:", 5, 20, 100, Color::White);
	if (state.setting.refresh)
		drawImage(buf, L"On.gres",30, 20, REFRESH);
	else
		drawImage(buf, L"OFF.gres", 30, 20, REFRESH);
	drawText(buf, L"�ֱ������� ȭ���� �ٽ� �׷� �ܻ��� �����մϴ�.", 7, 21, 100, Color::Gray);
	drawText(buf, L"����Ʈ ���� ������� �Բ� ������� �ʱ⸦ ��õ�մϴ�.", 7, 22, 100, Color::Gray);

	drawText(buf, L"= ��Ƽ ������ ��������:", 5, 24, 100, Color::White);
	if (state.setting.refreshThreadsCount==2)
		drawImage(buf, L"On.gres", 30, 24, REFRESH_MULTI_THREAD);
	else
		drawImage(buf, L"OFF.gres", 30, 24, REFRESH_MULTI_THREAD);
	drawText(buf, L"�������ð����� ��Ƽ�������� ����մϴ�.", 7, 25, 100, Color::Gray);

	drawText(buf, L"= ���� ��������: ", 5, 27, 100, Color::White);
	if (state.setting.parallelRefresh)
		drawImage(buf, L"On.gres", 30, 27, PARALLEL_REFRESH);
	else
		drawImage(buf, L"OFF.gres", 30, 27, PARALLEL_REFRESH);
	drawText(buf, L"�������ý����带 ���� ������� �и��մϴ�.", 7, 28, 100, Color::Gray);

	drawText(buf, L"= ����Ʈ ���� �����:", 5, 30, 100, Color::White);
	if (state.setting.frontBufferDithering)
		drawImage(buf, L"On.gres", 30, 30, FRONT_BUFFER_DITHERING);
	else
		drawImage(buf, L"OFF.gres", 30, 30, FRONT_BUFFER_DITHERING);
	drawText(buf, L"����Ʈ ���ۿ� �������� ������ �߰��Ͽ� �ܻ��� �����մϴ�.", 7, 31, 100, Color::Gray);
	drawText(buf, L"�������ÿ� �Բ� ������� �ʱ⸦ ��õ�մϴ�.", 7, 32, 100, Color::Gray);

	drawText(buf, L"= ���� �����:", 5, 34, 100, Color::White);
	if (state.setting.ditheringSize>=100)
		drawImage(buf, L"On.gres", 30, 34, FAST_DITHER);
	else
		drawImage(buf, L"OFF.gres", 30, 34, FAST_DITHER);
	drawText(buf, L"����� �ӵ��� �������� ��� FPS�� �������ϴ�.", 7, 35, 100, Color::Gray);

	drawImage(buf, L"BackButton.gres", 68, 37, BACK_BUTTON);

	return 0;
}

int playSettingScreen(Buffer buf, GameState* state)
{
	if (getKey() == Key::kESC) state->scene = Main;
	if (getClickOnce().type == Left)
	{
		switch (getClickObject(buf))
		{
		case SHOW_FPS:
			state->setting.showFPS = !state->setting.showFPS;
			break;
		case RENDER_MULTI_THREAD:
			if (state->setting.renderThreadsCount == 1) state->setting.renderThreadsCount = 2;
			else state->setting.renderThreadsCount = 1;
			break;
		case NO_SPACE_WINDOW:
			state->setting.noSpaceWindow = !state->setting.noSpaceWindow;
			Sleep(100);
			break;
		case REFRESH:
			state->setting.refresh = !state->setting.refresh;
			break;
		case REFRESH_MULTI_THREAD:
			if (state->setting.refreshThreadsCount == 1) state->setting.refreshThreadsCount = 2;
			else state->setting.refreshThreadsCount = 1;
			break;
		case PARALLEL_REFRESH:
			state->setting.parallelRefresh = !state->setting.parallelRefresh;
			break;
		case FRONT_BUFFER_DITHERING:
			state->setting.frontBufferDithering = !state->setting.frontBufferDithering;
			break;
		case FAST_DITHER:
			if (state->setting.ditheringSize== 10) state->setting.ditheringSize = 100;
			else state->setting.ditheringSize = 10;
			break;
		case BACK_BUTTON:
			state->scene = Main;
			Sleep(10);
			break;
		}
	}
	return 0;
}
