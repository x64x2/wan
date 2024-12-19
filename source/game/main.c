#include <engine/psx.h>
#include <engine/controls.h>
#include <engine/display.h>
#include <engine/init.h>

#include <game/gamestate.h>

unsigned long _ramsize   = 0x00200000;
unsigned long _stacksize = 0x00004000;

int main()
{
	init();

	while(1)  // TODO FIX
	{
		controller_read(0);

		switch_double_buffer();

		switch(current_gamestate)
		{
			case INTRO:
				execute_intro();	// located at source/game/intro.c
				break;
			case MAINMENU:
				execute_mainmenu();	// located at source/game/mainmenu.c
				break;
			case GAME:
				execute_game();		// located at source/game/game.c
				break;
		}

		SsSeqCalledTbyT();

		FntFlush(-1);

		DrawSync(0);
		VSync(0);
	}

	return 0;
}

