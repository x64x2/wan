#include <engine/collision.h>

char colbox_is_horizontally_aligned(struct CollisionBox *p_colbox, SPRT *p_tile)
{
	if((p_colbox->y0 > p_tile->y0) && (p_colbox->y0 < p_tile->y0 + p_tile->h)) ||	// top of colbox is between tile horizontally OR
		((p_colbox->y1 > p_tile->y0) && (p_colbox->y1 < p_tile->y0 + p_tile->h)) ||	// middle of colbox is between tile horizontally OR
		((p_colbox->y2 > p_tile->y0) && (p_colbox->y2 < p_tile->y0 + p_tile->h)) )	// bottom of colbox is between tile horizontally
	{  
		return 1;
	}
}

char colbox_is_vertically_aligned(struct CollisionBox *p_colbox, SPRT *p_tile)
{
	if(     ((p_colbox->x0 > p_tile->x0) && (p_colbox->x0 < p_tile->x0 + p_tile->w)) ||		// left of colbox is between tile vertically OR
		((p_colbox->x1 > p_tile->x0) && (p_colbox->x1 < p_tile->x0 + p_tile->w)) ||		// middle of colbox is between tile vertically OR
		((p_colbox->x2 > p_tile->x0) && (p_colbox->x2 < p_tile->x0 + p_tile->w)) )		// right of colbox is between tile vertically
	{
		return 1;
	}
}

void init_collision_box(struct CollisionBox *p_colbox, struct ShortRECT *p_rect)
{
	// for brevity
	short	*x0, *y0, *x1, *y1, *x2, *y2;

	x0 = &p_colbox->x0; y0 = &p_colbox->y0;
	x1 = &p_colbox->x1; y1 = &p_colbox->y1;
	x2 = &p_colbox->x2; y2 = &p_colbox->y2;

	//// initialize members ////
	p_colbox->enable_debug = 0;

	*x0 = p_rect->x;		// left of colbox
	*x2 = p_rect->x + p_rect->w;	// right of colbox
	*x1 = *x0 + (*x2 - *x0)/2;	// middle of colbox

	*y0 = p_rect->y;		// top of colbox
	*y2 = p_rect->y + p_rect->h;	// bottom of colbox
	*y1 = *y0 + (*y2 - *y0)/2;	// middle of colbox

	// init left_boundary (red)
	SetLineF2(&p_colbox->left_boundary);

	setRGB0(&p_colbox->left_boundary, 255, 0, 0);
	setXY2(&p_colbox->left_boundary, *x0, *y0, *x0, *y2);

	// init boundary (red)
	SetLineF4(&p_colbox->boundary);

	setRGB0(&p_colbox->boundary, 255, 0, 0);
	setXY4(&p_colbox->boundary, *x0, *y0, *x2, *y0, *x2, *y2, *x0, *y2);

	// init debug lines (green)
	SetLineF2(&p_colbox->debug_x0); SetLineF2(&p_colbox->debug_y0);
	SetLineF2(&p_colbox->debug_x1); SetLineF2(&p_colbox->debug_y1);
	SetLineF2(&p_colbox->debug_x2); SetLineF2(&p_colbox->debug_y2);

	setRGB0(&p_colbox->debug_x0, 0, 255, 0); setRGB0(&p_colbox->debug_y0, 0, 255, 0);
	setRGB0(&p_colbox->debug_x1, 0, 255, 0); setRGB0(&p_colbox->debug_y1, 0, 255, 0);
	setRGB0(&p_colbox->debug_x2, 0, 255, 0); setRGB0(&p_colbox->debug_y2, 0, 255, 0);

	// set positions for all debug vertical lines
	setXY2(&p_colbox->debug_x0, *x0, *y0-10, *x0, *y2+10);
	setXY2(&p_colbox->debug_x1, *x1, *y0-10, *x1, *y2+10);
	setXY2(&p_colbox->debug_x2, *x2, *y0-10, *x2, *y2+10);

	// set positions for all debug horizontal lines
	setXY2(&p_colbox->debug_y0, *x0-10, *y0, *x2+10, *y0);
	setXY2(&p_colbox->debug_y1, *x0-10, *y1, *x2+10, *y1);
	setXY2(&p_colbox->debug_y2, *x0-10, *y2, *x2+10, *y2);
}

void move_collision_box(struct CollisionBox *p_colbox, short x, short y)
{
	p_colbox->x0 += x;
	p_colbox->x1 += x;
	p_colbox->x2 += x;

	p_colbox->y0 += y;
	p_colbox->y1 += y;
	p_colbox->y2 += y;

	// move debug members
	MOVE_LINE2(&p_colbox->left_boundary, x, y);
	MOVE_LINE4(&p_colbox->boundary, x, y);

	MOVE_LINE2(&p_colbox->debug_x0, x, y);
	MOVE_LINE2(&p_colbox->debug_x1, x, y);
	MOVE_LINE2(&p_colbox->debug_x2, x, y);

	MOVE_LINE2(&p_colbox->debug_y0, x, y);
	MOVE_LINE2(&p_colbox->debug_y1, x, y);
	MOVE_LINE2(&p_colbox->debug_y2, x, y);
}

// the collision box is meant to be invisible, so
void draw_collision_box(struct CollisionBox *p_colbox)
{
	if(p_colbox->enable_debug)
	{
		DrawPrim(&p_colbox->debug_x0); DrawPrim(&p_colbox->debug_y0);
		DrawPrim(&p_colbox->debug_x1); DrawPrim(&p_colbox->debug_y1);
		DrawPrim(&p_colbox->debug_x2); DrawPrim(&p_colbox->debug_y2);

		DrawPrim(&p_colbox->left_boundary);
		DrawPrim(&p_colbox->boundary);
	}
}

