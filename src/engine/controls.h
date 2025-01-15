#ifndef CONTROLS_H
#define CONTROLS_H

#include <stdlib.h>

// 16-button
#define PADUP		PADLup
#define PADDOWN		PADLdown
#define PADLEFT		PADLleft
#define PADRIGHT	PADLright
#define PADTRIANGLE	PADRup
#define PADCROSS	PADRdown
#define PADSQUARE	PADRleft
#define PADCIRCLE	PADRright
#define PADSTART	PADstart
#define PADSELECT	PADselect
#define PADL1      	PADn
#define PADL2      	PADo
#define PADL3		PADi
#define PADR1      	PADl
#define PADR2      	PADm
#define PADR3		PADj

// Mouse
#define CLICK_LEFT	MOUSEleft
#define CLICK_RIGHT	MOUSEright

unsigned char	padbuf[2][34];

struct Controller
{
	short	btn;		// button
	short	btnprev;	// button previous

	// analog stick values
	int	rx;	// right x
	int	ry;	// right y
	int	lx;	// left x
	int	ly;	// left y
}gamepad[2];

void controller_read(int port);
int is_button_pressed(int port, int button);
int is_button_just_pressed(int port, int button);

#endif

