#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <engine/psx.h>
#include <engine/controls.h>

enum GameState
{
	INTRO,
	MAINMENU,
	GAME
}current_gamestate;

// execution code for each gamestate
void execute_intro();
void execute_mainmenu();
void execute_game();

#endif

