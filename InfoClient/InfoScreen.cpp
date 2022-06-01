#include "Graphic.h"
#include "GameState.h"
#include "Input.h"

const int BACK_BUTTON = 1;;

int drawInfoScreen(Buffer buf, GameState state)
{
	drawText(buf, L"설명", 40, 1, 100, Color::White);
	drawImage(buf, L"TileRD.gres", 0, 0);
	drawImage(buf, L"TileLD.gres", 73, 0);
	drawImage(buf, L"TileUR.gres", 0, 36);
	drawImage(buf, L"TileUL.gres", 73, 36);
	drawImage(buf, L"BackButton.gres", 68, 37, BACK_BUTTON);
	drawText(buf, L"본 게임은 상대방과 번갈아가며 총 16개의 전선으로 회로를 완성하는 게임이다", 4, 5, 100, Color::LightBlue);
	drawText(buf, L"1. 초기 2개의 연결된 전선이 주어진다.", 4, 8, 100, Color::White);
	drawText(buf, L"2. 자신의 턴에는 150초의 제한시간이 누적감소한다.", 4, 11, 100, Color::White);
	drawText(buf, L"3. 한 턴에          의 전선을 설치 가능하다.", 4, 14, 100, Color::White);
	drawText(buf, L"최대 3개", 15, 14, 100, Color::Yellow);
	drawText(buf, L"4. 전선은 일렬이 되게 설치해야 한다. ", 4, 17, 100, Color::White);
	drawImage(buf, L"TileLR.gres", 41, 15);
	drawImage(buf, L"TileLR.gres", 47, 15);
	drawImage(buf, L"TileLR.gres", 53, 15);
	drawText(buf, L"5. 남은 전선으로 모든 전선을 연결할 수 없다고 판단된다면         을", 4, 20, 100, Color::White);
	drawText(buf, L"   외칠 수 있다.", 4, 21, 100, Color::White);
	drawText(buf, L"불가능!", 61, 20, 100, Color::LightRed);
	drawText(buf, L"6. 불가능을 외친다면 상대방에게 남은 전선으로 회로를 완성할 기회가 주어진다.", 4, 24, 100, Color::White);
	drawText(buf, L"승리조건 ", 4, 28, 100, Color::White);
	drawText(buf, L"- 자신의 턴에 회로를 완성한다. ", 4, 30, 100, Color::White);
	drawText(buf, L"- 상대방의 제한시간이 모두 소진된다.", 4, 32, 100, Color::White);
	drawText(buf, L"- 불가능을 외친후 상대방이 회로를 완성하지 못한다.  ", 4, 34, 100, Color::White);
	return 0;
}

int playInfoScreen(Buffer buf, GameState *state)
{
	if (getClickOnce().type == Left && getClickObject(buf) == BACK_BUTTON)
		state->scene = Main;
	return 0;
}