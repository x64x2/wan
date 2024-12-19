#include <engine/asset.h>

// variables used specifically for cbready()
unsigned short current_channel; // the channel obtained by CdGetSector()
unsigned short ID;

void play_XA(struct XAFile *p_xa, int channel)
{
	// in case we are switching to a different XA file, set is_playing to 0
	p_currentXA->is_playing = 0;

	// set is_playing to 1 to current XA file
	p_xa->is_playing = 1;

	// read from beginning of XA file
	CdControlF(CdlReadS, (unsigned char *)&p_xa->fp.pos);
}

char cdsetup_XA(struct XAFile *p_xa, char *name, int channel, int file)
{
	// should not be playing by default
	p_xa->is_playing = 0;

	// set the filter (which ADPCM channel is playing)
	p_xa->filter.file = file;
	p_xa->filter.chan = channel;
	CdControl(CdlSetfilter, (unsigned char *)&p_xa->filter, 0);

	return 1; // success
}

void cdsetup_mode(unsigned int mode)
{
	CdControl(CdlSetmode, (unsigned char *)(&mode), 0);
	VSync(3); // prevent CD motor screw-up; see LibOver47.pdf pg.196 for more info
}

