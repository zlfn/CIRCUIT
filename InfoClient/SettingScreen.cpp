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
	drawText(buf, L"= FPS 표시: ", 5, 10, 100, Color::White);
	if (state.setting.showFPS)
		drawImage(buf, L"On.gres",30, 10, SHOW_FPS);
	else
		drawImage(buf, L"OFF.gres", 30, 10, SHOW_FPS);
	drawText(buf, L"화면 왼쪽 하단에 FPS를 표시합니다.", 7, 11, 100, Color::Gray);


	drawText(buf, L"= 멀티 스레드 렌더링: ", 5, 13, 100, Color::White);
	if (state.setting.renderThreadsCount >= 2)
		drawImage(buf, L"On.gres", 30, 13, RENDER_MULTI_THREAD);
	else
		drawImage(buf, L"OFF.gres", 30, 13, RENDER_MULTI_THREAD);
	drawText(buf, L"화면 렌더링에 멀티스레드를 사용합니다.", 7, 14, 100, Color::Gray);
	drawText(buf, L"화면이 불안정해지지만 성능이 향상됩니다.", 7, 15, 100, Color::Gray);


	drawText(buf, L"= 여백없는 창: ", 5, 17, 100, Color::White);
	if (state.setting.noSpaceWindow)
		drawImage(buf, L"On.gres", 30, 17, NO_SPACE_WINDOW);
	else
		drawImage(buf, L"OFF.gres", 30, 17, NO_SPACE_WINDOW);
	drawText(buf, L"창의 여백 여부에 따라 그래픽이 깨지기도 합니다.", 7, 18, 100, Color::Gray);


	drawText(buf, L"= 리프레시:", 5, 20, 100, Color::White);
	if (state.setting.refresh)
		drawImage(buf, L"On.gres",30, 20, REFRESH);
	else
		drawImage(buf, L"OFF.gres", 30, 20, REFRESH);
	drawText(buf, L"주기적으로 화면을 다시 그려 잔상을 제거합니다.", 7, 21, 100, Color::Gray);
	drawText(buf, L"프론트 버퍼 디더링과 함께 사용하지 않기를 추천합니다.", 7, 22, 100, Color::Gray);

	drawText(buf, L"= 멀티 스레드 리프레싱:", 5, 24, 100, Color::White);
	if (state.setting.refreshThreadsCount==2)
		drawImage(buf, L"On.gres", 30, 24, REFRESH_MULTI_THREAD);
	else
		drawImage(buf, L"OFF.gres", 30, 24, REFRESH_MULTI_THREAD);
	drawText(buf, L"리프레시과정에 멀티스레딩을 사용합니다.", 7, 25, 100, Color::Gray);

	drawText(buf, L"= 병렬 리프레싱: ", 5, 27, 100, Color::White);
	if (state.setting.parallelRefresh)
		drawImage(buf, L"On.gres", 30, 27, PARALLEL_REFRESH);
	else
		drawImage(buf, L"OFF.gres", 30, 27, PARALLEL_REFRESH);
	drawText(buf, L"리프레시스레드를 메인 스레드와 분리합니다.", 7, 28, 100, Color::Gray);

	drawText(buf, L"= 프론트 버퍼 디더링:", 5, 30, 100, Color::White);
	if (state.setting.frontBufferDithering)
		drawImage(buf, L"On.gres", 30, 30, FRONT_BUFFER_DITHERING);
	else
		drawImage(buf, L"OFF.gres", 30, 30, FRONT_BUFFER_DITHERING);
	drawText(buf, L"프론트 버퍼에 인위적인 잡음을 추가하여 잔상을 제거합니다.", 7, 31, 100, Color::Gray);
	drawText(buf, L"리프레시와 함께 사용하지 않기를 추천합니다.", 7, 32, 100, Color::Gray);

	drawText(buf, L"= 빠른 디더링:", 5, 34, 100, Color::White);
	if (state.setting.ditheringSize>=100)
		drawImage(buf, L"On.gres", 30, 34, FAST_DITHER);
	else
		drawImage(buf, L"OFF.gres", 30, 34, FAST_DITHER);
	drawText(buf, L"디더링 속도가 빨라지는 대신 FPS가 낮아집니다.", 7, 35, 100, Color::Gray);

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
