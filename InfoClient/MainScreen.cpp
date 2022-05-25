#include "Graphic.h"
#include "GameState.h"
#include "Input.h"

const int SETTING_BUTTON = 1;

int drawMainScreen(Buffer buf, GameState state)
{
		drawImage(buf, L"GBSLAND_LOGO.gres", 11, 4);
		drawText(buf, L"경기북과학고등학교 정보 수행평가", 37, 10, 100, Color::LightYellow);
		drawText(buf, L"(C) 2022. 박찬웅, 김진서, 박지환", 0/*29*/, 38, 200, Color::Yellow);
		drawText(buf, L"This software distribute under GNU GPL 3.0 license", 0/*30*/, 39, 200, Color::Yellow);
		drawImage(buf, L"SettingButton.gres", 68, 37, SETTING_BUTTON);
		drawText(buf, state.debug, 10, 0, 100, Color::White);

		return 0;
}

int playMainScreen(Buffer buf, GameState *state)
{
	if (getKey() == Key::kEnter) state->debug = L"음";
	if (getKey() == Key::kA) state->debug = L"집에 가고 싶다";
	switch (getClickObject(buf))
	{
	case SETTING_BUTTON:
		throw;
	}
	return 0;
}