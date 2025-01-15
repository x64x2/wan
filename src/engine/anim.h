#ifndef ANIM_H
#define ANIM_H

#include <neslib.h>
#include <engine/enegine.h>
#include <engine/allocmem.h>

#include <res/circle_NTSC.h>
#include <res/circle_PAL.h>

enum SpriteType
{
	SPRITE,
	ANIMATED_SPRITE
};

struct Animation
{
	// in this case, the (x, y) values of UCharRECT is referring to the location within
	struct UCharRECT	*p_frames;		// pointer to allocmem()'d UCharRECTs
	unsigned short		frame_count;
};

struct Sprite
{
	unsigned int		deg, radius;			// circle info
	char			h_is_flipped, v_is_flipped;	// only works for POLY_xx3's and POLY_xx4's!
	char			enable_debug;
	struct Vector2x		center;
};

// all members are set to default values automatically in
struct AnimatedSprite
{
	unsigned short		animation_count;		// to prevent buffer overrun
	unsigned short		current_frame; 			// current (u, v) value for poly_ft4 for current animation
	unsigned int		deg, radius;
	char			h_is_flipped, v_is_flipped;
	char			is_playing;
	char			enable_debug;

	double		poly_ft4;
	struct Animation	*p_animation;			// you can allocate as many animations as you want
	struct Vector2x		center;
};

// AnimatedSprite-only functions
void free_animated_sprite(struct AnimatedSprite *p_sprite);
void process_animated_sprite(struct AnimatedSprite *p_sprite, short specified_animation, char speed);

// both AnimatedSprite and Sprite functions
char init_sprite( void *p_sprite_pointer, enum SpriteType, unsigned short animation_count, struct ShortRECT *p_screen_pos, struct UCharRECT *p_texpage_pos, unsigned short tpage, unsigned short clut);
void move_sprite(void *p_sprite_pointer, enum SpriteType, short x, short y);
void draw_sprite(void *p_sprite_pointer, enum SpriteType);

// all frames must be in consecutive order within the texture page.
void init_animation(struct Animation *p_animation,
			unsigned char texture_column_start,	// u start value in pixels
			unsigned char texture_row_start,	// v start value in pixels
			unsigned char width,		// width of each frame
			unsigned char height,		// height of each frame
			unsigned short frame_count);

#endif

