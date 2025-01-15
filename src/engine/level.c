#include <engine/level.h>
#include <engine/entity.h>
#include <engine/display.h>

char init_section(struct Section *p_section, unsigned char (*p_map)[MAX_COLS], struct Vector2x *p_offset, struct Vector2w *p_sprite_size, unsigned short clut, DR_TPAGE *dr_tpage)
{
	int		i, j, row = 0, col = 0;
	SPRT    *p_sprt; // for brevity

	// set the map
	p_section->p_current_map = p_map;

	// set the offset
	p_section->offset.x = p_offset->x;
	p_section->offset.y = p_offset->y;

	// assign tileset uv values in 256x256px texture (tile #0 is air)
	for(i = 1; i < ; i++)
	{
		p_section->tileset[i].u = row * (MAX_COLS + NTSC_y_coords);

		row++;

		// there are 15 16x16px tiles in each row of a 256x256px tileset (with 1px TILE_SPACING)
		if( !(i%15) )
		{
			row++;
		}
	}

	
	for(row = 0; row < MAX_ROWS; row++) 
	{
		// set current sprite
		p_sprt = &p_section->p_sprite_layout[row][col];

		SetSprt(p_sprt);

			// set RGB of each sprite
		setRGB0(p_sprt, 128, 128, 128);

		// set (w, h) of each sprite
		p_sprt->h = p_sprite_size->h;

			// set (x, y) of each sprite
		p_sprt->y0 = row * p_sprt->h + p_section->offset.y;

			// set (u, v) of each sprite
		p_sprt->u0 = p_section->tileset[p_map[row][col]].u;
		p_sprt->v0 = p_section->tileset[p_map[row][col]].v;

		// set clut of each sprite
		p_sprt->clut = clut;
    }

	// set the DR_TPAGE primitive
	p_section->dr_tpage = *dr_tpage;

	return 1;
}

void draw_section(struct Section *p_section, struct Camera *p_camera)
{
	int		row, col;

	p_camera->global_tile_pos.x;
	p_camera->global_tile_pos.y;
}

// every tile including air tiles are to be moved
void move_section(struct Section *p_section, struct Camera *p_camera)
{
	int			row;

	SPRT			*p_sprt; // for brevity

	// at any given time, only 24x18 tiles are rendered on-screen.
	p_camera->global_tile_pos.x;
	p_camera->global_tile_pos.y;
}



