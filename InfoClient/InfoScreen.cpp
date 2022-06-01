#include "Graphic.h"
#include "GameState.h"
#include "Input.h"

const int BACK_BUTTON = 1;;

int drawInfoScreen(Buffer buf, GameState state)
{
	drawText(buf, L"����", 40, 1, 100, Color::White);
	drawImage(buf, L"TileRD.gres", 0, 0);
	drawImage(buf, L"TileLD.gres", 73, 0);
	drawImage(buf, L"TileUR.gres", 0, 36);
	drawImage(buf, L"TileUL.gres", 73, 36);
	drawImage(buf, L"BackButton.gres", 68, 37, BACK_BUTTON);
	drawText(buf, L"�� ������ ����� �����ư��� �� 16���� �������� ȸ�θ� �ϼ��ϴ� �����̴�", 4, 5, 100, Color::LightBlue);
	drawText(buf, L"1. �ʱ� 2���� ����� ������ �־�����.", 4, 8, 100, Color::White);
	drawText(buf, L"2. �ڽ��� �Ͽ��� 150���� ���ѽð��� ���������Ѵ�.", 4, 11, 100, Color::White);
	drawText(buf, L"3. �� �Ͽ�          �� ������ ��ġ �����ϴ�.", 4, 14, 100, Color::White);
	drawText(buf, L"�ִ� 3��", 15, 14, 100, Color::Yellow);
	drawText(buf, L"4. ������ �Ϸ��� �ǰ� ��ġ�ؾ� �Ѵ�. ", 4, 17, 100, Color::White);
	drawImage(buf, L"TileLR.gres", 41, 15);
	drawImage(buf, L"TileLR.gres", 47, 15);
	drawImage(buf, L"TileLR.gres", 53, 15);
	drawText(buf, L"5. ���� �������� ��� ������ ������ �� ���ٰ� �Ǵܵȴٸ�         ��", 4, 20, 100, Color::White);
	drawText(buf, L"   ��ĥ �� �ִ�.", 4, 21, 100, Color::White);
	drawText(buf, L"�Ұ���!", 61, 20, 100, Color::LightRed);
	drawText(buf, L"6. �Ұ����� ��ģ�ٸ� ���濡�� ���� �������� ȸ�θ� �ϼ��� ��ȸ�� �־�����.", 4, 24, 100, Color::White);
	drawText(buf, L"�¸����� ", 4, 28, 100, Color::White);
	drawText(buf, L"- �ڽ��� �Ͽ� ȸ�θ� �ϼ��Ѵ�. ", 4, 30, 100, Color::White);
	drawText(buf, L"- ������ ���ѽð��� ��� �����ȴ�.", 4, 32, 100, Color::White);
	drawText(buf, L"- �Ұ����� ��ģ�� ������ ȸ�θ� �ϼ����� ���Ѵ�.  ", 4, 34, 100, Color::White);
	return 0;
}

int playInfoScreen(Buffer buf, GameState *state)
{
	if (getClickOnce().type == Left && getClickObject(buf) == BACK_BUTTON)
		state->scene = Main;
	return 0;
}