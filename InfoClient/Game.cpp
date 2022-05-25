#include "Graphic.h"
#include "GameState.h"
#include "Scenes.h"


int drawGame(Buffer buf, GameState state)
{
	switch (state.scene)
	{
	case Main:
		drawMainScreen(buf, state);
	}
	return 0;
}

int playGame(Buffer buf, GameState* state)
{
	switch (state->scene)
	{
	case Main:
		playMainScreen(buf, state);
	}
	return 0;
}