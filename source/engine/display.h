#ifndef DISPLAY_H
#define DISPLAY_H

#include <engine/psx.h>

// orange background
#define BG_RED   255
#define BG_GREEN 113
#define BG_BLUE  94

#define OVERSCAN_MARGIN		16

#define SCREEN_WIDTH_NORMAL	320
#define SCREEN_WIDTH_WIDE	384
#define SCREEN_HEIGHT_NTSC	240
#define SCREEN_HEIGHT_PAL	256

void switch_double_buffer();

struct DB
{
	DRAWENV draw;
	DISPENV disp;
};

struct DB db[2];
struct DB *p_cdb;

int		screen_width, screen_height;

volatile int	fps;
volatile int	fps_counter;
volatile int	fps_measure;

//////// variable-time step variables ////////
// (as opposed to constant-time step)
volatile int	vts_cdf;
volatile int	vts_lkdf;
volatile int	delta;

#endif

