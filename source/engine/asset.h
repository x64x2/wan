#ifndef ASSET_H
#define ASSET_H

#include <engine/psx.h>

// CLUT CX and CY values
#define COMMON_CX		768

// error codes
#define VAB_HEADERFAIL		(1<<0)
#define VAB_TRANSBODYFAIL	(1<<1)

#define DATABUFFER_SIZE		2352

struct XAFile
{
	CdlFILE fp;		// location of file
	CdlFILTER filter;	// ADPCM channel
	char is_playing;
};

// p_currentXA is the XA file being played, which is set in play_XA().
struct XAFile *p_currentXA;

// buffers
unsigned char data_buffer[DATABUFFER_SIZE];

// prototypes
void play_XA(struct XAFile *p_xa, int channel);
char cdsetup_XA(struct XAFile *p_xa, char *name, int channel, int file);
void cdsetup_mode(unsigned int mode);

// callbacks
void cbready(int status, unsigned char *p_result);

#endif

