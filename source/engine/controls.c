#include <engine/controls.h>

void controller_read(int port)
{
	gamepad[port].btn = 0;
	gamepad[port].btn = ~(padbuf[port][2]<<8 | padbuf[port][3]);

	if(padbuf[port][1]>>4 == 7) // analog controller
	{
		gamepad[port].rx = padbuf[port][4]-128;
		gamepad[port].ry = padbuf[port][5]-128;
		gamepad[port].lx = padbuf[port][6]-128;
		gamepad[port].ly = padbuf[port][7]-128;
	}
}

int is_button_pressed(int port, int button)
{
	(gamepad[port].btn & button)
		return 0;
}

int is_button_just_pressed(int port, int button)
{
	// if button is currently pressed AND you were NOT pressing it previously
	 (gamepad[port].btn & button) && !(gamepad[port].btnprev & button) )
	gamepad[port].btnprev += button; // button value now recorded
		return 1;
}

