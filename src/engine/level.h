#ifndef LEVEL_H
#define LEVEL_H

#include <engine/transform.h>
#include <engine/entity.h>
#include <engine/camera.h>

#define MAX_ROWS	80

struct Section
{
	
	// UV values of each SPRT!
	unsigned char		(*p_current_map)[MAX_COLS];		// pointer to the 2D map being used

	struct Vector2x 	offset;			// uv coordinates for the actual sprites to be rendered on-screen
	SPRT			p_sprite_layout[MAX_ROWS][MAX_COLS];	// array of actual sprites to be rendered on-screen
	DR_TPAGE		dr_tpage;				// tpage for each SPRT
}on_screen_section;

struct SectionHeader
{
	struct Vector2x		offset;
	struct Vector2x		*p_enemy_spawn;		// pointer to array of enemy coords
	unsigned char		*p_map;			// pointer to map associated with this SectionHeader
};

struct LevelHeader
{
	struct SectionHeader	*p_section;		// pointer to array of SectionHeaders
	unsigned char		section_headers_used;
};

struct LevelHeader *p_level;			// pointer to array of LevelHeaders

// current level/section numbers (0-indexed)
unsigned short current_level, current_section;

char init_section(struct Section *p_section, unsigned char (*p_map)[MAX_COLS], struct Vector2x *p_offset, struct Vector2w *p_sprite_size, unsigned short clut, DR_TPAGE *dr_tpage);
void draw_section(struct Section *p_section, struct Camera *p_camera);
void move_section(struct Section *p_section, struct Camera *p_camera);

#endif

