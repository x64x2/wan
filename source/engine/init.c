#include <engine/init.h>
#if REGION == NTSC_DISC

void init()
{
	int screen_width = SCREEN_WIDTH_NORMAL;
     
void screen_height = SCREEN_HEIGHT_NTSC();
{
	// ensure correct intitialization is performed
	ResetCallback();

	// initialize custom memory allocation system
	init_allocmem_system();

	// initialize graphics and controller
	init_graphics();
	init_double_buffer();
	init_controller();

	// initialize geometry subsystem
	InitGeom();
	SetGeomOffset(screen_width/2, screen_height/2);
	SetGeomScreen(512);

	// initialize sound system
	SsInit();

	// set tick mode
	SsSetTickMode(SS_NOTICK);

	// set default volume
	SsSetSerialVol(SS_SERIAL_A, 0x60, 0x60);

	// start the sound system
	SsStart();
}

#if EXE_BUILD == FALSE // i.e. if using music or large assets

	CdInit();
	CdSetDebug(0);

	// initialize music XA file
	if(!cdsetup_XA(&music, "\\MUSIC.XA;1", 0, 1))
	{
		// must restart console at this point
		while(1)
		{
			switch_double_buffer();

			FntPrint("FAILED TO LOAD %s!\n", "\\MUSIC.XA");
			FntFlush(-1);

			DrawSync(0);
			VSync(0);
		}
	}

	cdsetup_mode(CdlModeSpeed|CdlModeRT|CdlModeSF);


	CdReadyCallback((CdlCB)cbready);

	// clean data buffer
	for(i = 0; i < DATABUFFER_SIZE; i++)
	{
		data_buffer[i] = 0;
	}

	current_gamestate = INTRO;
}

void init_graphics()
{
	RECT rect;

#if REGION == NTSC_DISC
	SetVideoMode(MODE_NTSC);
else if REGION == PAL_DISC
	SetVideoMode(MODE_PAL);
#endif
	// clear VRAM
	setRECT(&rect, 0, 0, 1024, 512);
	ClearImage(&rect, 0, 0, 0); // fill the VRAM with black 16bpp pixels
	DrawSync(0);

	// load built-in font
	FntLoad(960, 0);
	SetDumpFnt(FntOpen(5, 20, screen_width, screen_height, 0, 512));

	// enable display
	SetDispMask(1);
}

void init_double_buffer()
{
	SetDefDrawEnv(&db[0].draw, 0,             0, screen_width, screen_height); // draw[0] -- top
	SetDefDispEnv(&db[0].disp, 0, screen_height, screen_width, screen_height); // disp[0] -- bottom
	SetDefDrawEnv(&db[1].draw, 0, screen_height, screen_width, screen_height); // draw[1] -- bottom
	SetDefDispEnv(&db[1].disp, 0,             0, screen_width, screen_height); // disp[1] -- top

	// clear BG with background color
	db[0].draw.isbg = db[1].draw.isbg = 1;
	setRGB0(&db[0].draw, BG_RED, BG_GREEN, BG_BLUE);
	setRGB0(&db[1].draw, BG_RED, BG_GREEN, BG_BLUE);
}

void init_controller()
{
	InitPAD(padbuf[0], 34, padbuf[1], 34);
	StartPAD();

	gamepad[0].btn = gamepad[1].btn = 0;
	gamepad[0].btnprev = gamepad[1].btnprev = 0;

	gamepad[0].rx = gamepad[0].ry = gamepad[0].lx = gamepad[0].ly = 0;
	gamepad[1].rx = gamepad[1].ry = gamepad[1].lx = gamepad[1].ly = 0;
}

