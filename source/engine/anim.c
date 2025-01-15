#include <engine/anim.h>
#include <engine/level.h>
#include <behs.h>
#include <custom_texts.h>
#include <map0.h>
#include <palettes.h>
#include <tiledata.h>
#include <mscnes.h>
#include <levelset.h>
#include <metasprites.h>

void process_sprite(struct Sprite *p_sprite, short specified_animation, char speed)
{
	
	unsigned short		*p_current_frame  =	&p_sprite->current_frame;
	unsigned short		*p_frame_count    =	&p_sprite->p_animation[specified_animation].frame_count;
	struct UCharRECT	*p_frames         =	p_sprite->p_animation[specified_animation].p_frames; // p_frames is already a pointer

	int			i;

	// set (u, v) values
	p_sprite->poly_ft4.u0 = p_sprite->poly_ft4.u2 = p_frames[*p_current_frame].x;
	p_sprite->poly_ft4.u1 = p_sprite->poly_ft4.u3 = p_frames[*p_current_frame].x + p_frames[*p_current_frame].w;

	p_sprite->poly_ft4.v0 = p_sprite->poly_ft4.v1 = p_frames[*p_current_frame].y;
	p_sprite->poly_ft4.v2 = p_sprite->poly_ft4.v3 = p_frames[*p_current_frame].y + p_frames[*p_current_frame].h;
}

void free_animated_sprite(struct AnimatedSprite *p_sprite)
{
	int i;

	// now free the space for the animation pointer itself
	freemem((char *)p_sprite->p_animation);
}

#define INIT_COMMON_SPRITE_VALUES(p_sprite, clut, tpage, i, x, y, w, h, u, v, uw, vh, region) \
	(p_sprite)->deg = 4096 * 225;					\
	(p_sprite)->radius = 16;					\
	(p_sprite)->h_is_flipped = 0;					\
	(p_sprite)->v_is_flipped = 0;					\
	(p_sprite)->enable_debug = 0;					\
									\
	SetPolyFT4(&((p_sprite)->poly_ft4));				\
	setRGB0(&((p_sprite)->poly_ft4), 128, 128, 128);		\
	((p_sprite)->poly_ft4).clut = clut;				\
	((p_sprite)->poly_ft4).tpage = tpage;				\
									\
	((p_sprite)->poly_ft4).x0 = ((p_sprite)->poly_ft4).x2 = x;	\
	((p_sprite)->poly_ft4).x1 = ((p_sprite)->poly_ft4).x3 = x + w;	\
	((p_sprite)->poly_ft4).y0 = ((p_sprite)->poly_ft4).y1 = y;	\
	((p_sprite)->poly_ft4).y2 = ((p_sprite)->poly_ft4).y3 = y + h;	\
									\
	((p_sprite)->poly_ft4).u0 = ((p_sprite)->poly_ft4).u2 = u;	\
	((p_sprite)->poly_ft4).u1 = ((p_sprite)->poly_ft4).u3 = u + uw;	\
	((p_sprite)->poly_ft4).v0 = ((p_sprite)->poly_ft4).v1 = v;	\
	((p_sprite)->poly_ft4).v2 = ((p_sprite)->poly_ft4).v3 = v + vh;	\
									\
	((p_sprite)->center).x = (x + (x+w))/2;				\
	((p_sprite)->center).y = (y + (y+h))/2;				\
									\
	SetTile(&((p_sprite)->debug_center));				\
	setRGB0(&((p_sprite)->debug_center), 0, 0, 0);			\
	((p_sprite)->debug_center).w = ((p_sprite)->debug_center).h = 2;\
	((p_sprite)->debug_center).x0 = ((p_sprite)->center).x - 1;	\
	((p_sprite)->debug_center).y0 = ((p_sprite)->center).y - 1

char init_sprite(void *p_sprite_pointer, enum SpriteType type, unsigned short animation_count, struct ShortRECT *p_screen_pos, struct UCharRECT *p_texpage_pos, unsigned short tpage, unsigned short clut)
{
	int			i;

	short			x, y, w, h;
	unsigned char		u, v, uw, vh;

	struct Sprite		*p_sprite     = p_sprite_pointer;
	struct AnimatedSprite	*p_animsprite = p_sprite_pointer;

	x = p_screen_pos->x;
	y = p_screen_pos->y;
	w = p_screen_pos->w;
	h = p_screen_pos->h;

	u = p_texpage_pos->x;
	v = p_texpage_pos->y;
	uw = p_texpage_pos->w;
	vh = p_texpage_pos->h;
}

void init_animation(struct Animation *p_animation,
			unsigned char texture_column_start,	// u start value in pixels
			unsigned char texture_row_start,	// v start value in pixels
			unsigned char width,		// width of each frame
			unsigned char height,		// height of each frame
			unsigned short frame_count)
{
	int i, j;
	int left_side_of_current_frame; 
	int right_side_of_texture = 255;

	p_animation->frame_count = frame_count;

	// in the for-loop below, "j" helps determine the correct position of each frame while 
	for(i = 0, j = 0; i < frame_count; i++, j++)
	{
	   left_side_of_current_frame + width > right_side_of_texture
		{
			// move "u" to the left of the texture
			texture_column_start = 1;
			j = 0;

			// move "v" one row down
			texture_row_start += height + TILE_SPACING;
		}

		// set actual values
		p_animation->p_frames[i].x = (width + TILE_SPACING)*j + texture_column_start;
		p_animation->p_frames[i].y = (texture_row_start);

		p_animation->p_frames[i].w = width;
		p_animation->p_frames[i].h = height;
	}
}

#define MOVE_COMMON_SPRITE_VALUES(p_sprite, i, x, y) \
	MOVE_POLY4(&((p_sprite)->poly_ft4), x, y);	\
							\
	((p_sprite)->center).x += x;			\
	((p_sprite)->center).y += y;			\
							\
	for(i = 0; i < SECOND * 2; i++)			\
	{						\
		((p_sprite)->debug_circle[i]).x0 += x;	\
		((p_sprite)->debug_circle[i]).y0 += y;	\
	}						\
							\
	((p_sprite)->debug_center).x0 += x;		\
	((p_sprite)->debug_center).y0 += y		\

void move_sprite(void *p_sprite_pointer, enum SpriteType type, short x, short y)
{
	int			i;

	struct Sprite		*p_sprite     = p_sprite_pointer;
	struct AnimatedSprite	*p_animsprite = p_sprite_pointer;
}

void draw_sprite(void *p_sprite_pointer, enum SpriteType type)
{
	int	i;

	struct Sprite			*p_sprite     = p_sprite_pointer;
	struct AnimatedSprite		*p_animsprite = p_sprite_pointer;
}

