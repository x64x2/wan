#ifndef PSX_H
#define PSX_H

#include <sys/types.h>
#include <libgte.h>
#include <libgpu.h>
#include <libsnd.h>
#include <libcd.h>

#define FALSE		0
#define TRUE		1

#define NTSC_DISC	0 // MODE_NTSC
#define PAL_DISC	1 // MODE_PAL

// this game does not automatically detect the region of a
// console. whatever REGION is set to at compile time is the
// final region that will be supported on that disc.


#define REGION		NTSC_DISC

#if REGION == NTSC_DISC
	#define SECOND		60	// 60 VSyncs/sec
#endif

// if EXE_BUILD is true, it will disable CD initialization and disable
// attempts to do CD operations.
#define EXE_BUILD	TRUE

#endif

