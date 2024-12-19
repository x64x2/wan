#ifndef COLLISION_H
#define COLLISION_H
#include <engine/psx.h>
#include <engine/transform.h>

// collision box

struct CollisionBox
{
	short			x0, y0, x1, y1, x2, y2;
	char			enable_debug;
};

// useful for collision between collision boxes and level geometry
char colbox_is_horizontally_aligned(struct CollisionBox *p_colbox, SPRT *p_tile);
char colbox_is_vertically_aligned(struct CollisionBox *p_colbox, SPRT *p_tile);

void init_collision_box(struct CollisionBox *p_colbox, struct ShortRECT *p_rect);
void move_collision_box(struct CollisionBox *p_colbox, short x, short y);
void draw_collision_box(struct CollisionBox *p_colbox);

#endif

