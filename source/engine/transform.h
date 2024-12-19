#include <engine/psx.h>
#ifndef TRANSFORM_H
#define TRANSFORM_H

#define MOVE_POLY3(p, x, y) \
	(p)->x0 += x, (p)->x1 += x, (p)->x2 += x, \
	(p)->y0 += y, (p)->y1 += y, (p)->y2 += y

// FIXME: do not use SCALE_POLY3
#define SCALE_POLY3(p, n) \
	(p)->x0 +=  0, (p)->x1 +=  n, (p)->x2 += -n, \
	(p)->y0 += -n

#define HFLIP_POLY3(p, bufx, bufy) \
	bufx = (p)->x1, bufy = (p)->y1, \
	(p)->x1 = (p)->x2, \
	(p)->y1 = (p)->y2, \
	(p)->x2 = bufx, \
	(p)->y2 = bufy

// VFLIP_POLY3 is: (x values are unchanged)
#define VFLIP_POLY3(p, bufy) \
	bufy = (p)->y0, \
	(p)->y0 = (p)->y1, \
	(p)->y1 = bufy, \
	(p)->y2 = bufy

//// POLY_xx4 transformations (4 vertices) //////
#define MOVE_POLY4(p, x, y) \
	(p)->x0 += x, (p)->x1 += x, (p)->x2 += x, (p)->x3 += x, \
	(p)->y0 += y, (p)->y1 += y, (p)->y2 += y, (p)->y3 += y

#define SCALE_POLY4(p, n) \
	(p)->x0 += -n, (p)->x1 +=  n, (p)->x2 += -n, (p)->x3 += n, \
	(p)->y0 += -n, (p)->y1 += -n, (p)->y2 +=  n, (p)->y3 += n

#define HFLIP_POLY4(p, bufx, bufy) \
	bufx = (p)->x0, bufy = (p)->y0, \
	(p)->x0 = (p)->x1, \
	(p)->y0 = (p)->y1, \
	(p)->x1 = bufx, \
	(p)->y1 = bufy, \
	bufx = (p)->x2, bufy = (p)->y2, \
	(p)->x2 = (p)->x3, \
	(p)->y2 = (p)->y3, \
	(p)->x3 = bufx, \
	(p)->y3 = bufy

#define VFLIP_POLY4(p, bufx, bufy) \
	bufx = (p)->x0, bufy = (p)->y0, \
	(p)->x0 = (p)->x2, \
	(p)->y0 = (p)->y2, \
	(p)->x2 = bufx, \
	(p)->y2 = bufy, \
	bufx = (p)->x1, bufy = (p)->y1, \
	(p)->x1 = (p)->x3, \
	(p)->y1 = (p)->y3, \
	(p)->x3 = bufx, \
	(p)->y3 = bufy

#define MOVE_LINE2(p, x, y) \
	(p)->x0 += x, (p)->y0 += y, \
	(p)->x1 += x, (p)->y1 += y

#define MOVE_LINE3 MOVE_POLY3
#define MOVE_LINE4 MOVE_POLY4

struct CharRECT
{
	char x, y;
	char w, h;
};

struct UCharRECT
{
	unsigned char x, y;
	unsigned char w, h;
};

struct ShortRECT
{
	short x, y;
	short w, h;
};

struct UShortRECT
{
	unsigned short x, y;
	unsigned short w, h;
};

struct IntRECT
{
	int x, y;
	int w, h;
};

struct UIntRECT
{
	unsigned int x, y;
	unsigned int w, h;
};

struct Vector2x
{
	short x, y;
};

struct Vector2u
{
	short u, v;
};

struct Vector2w
{
	short w, h;
};

// radius must be a multiple of 16 unfortunately
void ROTATE_POLYFT4(POLY_FT4 *p_poly, struct Vector2x *p_center, char v_is_flipped, char h_is_flipped, unsigned int deg, unsigned int radius);

#endif
