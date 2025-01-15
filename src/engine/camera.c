#include <engine/camera.h>
#include <engine/level.h>

void init_camera(struct Camera *p_camera, struct Vector2x *p_section_offset, struct Entity *p_entity)
{
	p_camera->is_free = 0;

	p_camera->smoothing = 1;
	p_camera->enable_debug = 0;

	p_camera->velocity.x = 0;
	p_camera->velocity.y = 0;

	p_camera->global_pos.x = -p_section_offset->x;
	p_camera->global_pos.y = -p_section_offset->y;
	p_camera->global_tile_pos.x = p_camera->global_pos.x/TILE_SPRITE_SIZE;
	p_camera->global_tile_pos.y = p_camera->global_pos.y/TILE_SPRITE_SIZE;

	p_camera->center_focal_point.x = screen_width/2;
	p_camera->center_focal_point.y = screen_height/2;

	// an Entity's collision box also represents where the entity is on-screen!
	p_camera->destination_focal_point.x = p_entity->colbox.x0;
	p_camera->destination_focal_point.y = p_entity->colbox.y0;

	// initialize debug members
	SetLineF2(&p_camera->debug_distance);
	setRGB0(&p_camera->debug_distance, 255, 0, 0);

	setXY2(&p_camera->debug_distance,	p_camera->center_focal_point.x,
						p_camera->center_focal_point.y,
						p_camera->destination_focal_point.x,
						p_camera->destination_focal_point.y);

	SetLineF2(&p_camera->debug_velocity);
	setRGB0(&p_camera->debug_velocity, 0, 255, 0);

	setXY2(&p_camera->debug_velocity,	p_camera->center_focal_point.x,
						p_camera->center_focal_point.y,
						p_camera->center_focal_point.x + p_camera->velocity.x,
						p_camera->center_focal_point.y + p_camera->velocity.y);

	// init debug center focal point
	SetTile(&p_camera->debug_center_focal_point);
	setRGB0(&p_camera->debug_center_focal_point, 128, 128, 128);

	setWH(&p_camera->debug_center_focal_point, 2, 2);
	setXY0(&p_camera->debug_center_focal_point,	(screen_width/2)-1,
							(screen_height/2)-1);

	// init debug destination focal point
	SetTile(&p_camera->debug_destination_focal_point);
	setRGB0(&p_camera->debug_destination_focal_point, 128, 128, 128);

	setWH(&p_camera->debug_destination_focal_point, 2, 2);
	setXY0(&p_camera->debug_destination_focal_point, p_entity->colbox.x1, p_entity->colbox.y1);
}

// the destination_focal_point is the center of entity's collision box
void process_camera(struct Camera *p_camera, struct Entity *p_entity)
{
	struct Vector2x change;

	if(p_camera->is_free)
	{
		change.x = change.y = 0;

		p_camera->velocity.x = 0;
		p_camera->velocity.y = 0;
	}
	else
	{
		change.x = change.y = 0;

		// update camera's destination focal point
		p_camera->destination_focal_point.x = p_entity->colbox.x1;
		p_camera->destination_focal_point.y = p_entity->colbox.y1;

		// prevent divide-by-zero error
		if(!p_camera->smoothing)
		{
			while(1)
			{
				printf("THE CAMERA'S SMOOTHING MUST NOT EQUAL 0!\n");
			}
		}

		// smoothing
		change.x = (p_camera->destination_focal_point.x - p_camera->center_focal_point.x)/p_camera->smoothing;
		change.y = (p_camera->destination_focal_point.y - p_camera->center_focal_point.y)/p_camera->smoothing;

		// if there is no horizontal change, destination is at center
		if(!change.x)
		{
			p_camera->velocity.x = 0;
			p_camera->destination_focal_point.x = p_camera->center_focal_point.x;
		}

		// otherwise, apply change!
		else
		{
			p_camera->velocity.x = change.x;
		}

		// if there is no vertical change, destination is at center
		if(!change.y)
		{
			p_camera->velocity.y = 0;
			p_camera->destination_focal_point.y = p_camera->center_focal_point.y;
		}
		else
		{
			p_camera->velocity.y = change.y;
		}

	}

	// move the destination focal point opposite to camera velocity
	p_camera->destination_focal_point.x += -p_camera->velocity.x;
	p_camera->destination_focal_point.y += -p_camera->velocity.y;

	// don't forget to update debug members!
	p_camera->debug_destination_focal_point.x0 = p_camera->destination_focal_point.x;
	p_camera->debug_destination_focal_point.y0 = p_camera->destination_focal_point.y;

	setXY2(&p_camera->debug_distance, p_camera->center_focal_point.x,
						p_camera->center_focal_point.y,
						p_camera->destination_focal_point.x,
						p_camera->destination_focal_point.y);

	setXY2(&p_camera->debug_velocity, p_camera->center_focal_point.x,
						p_camera->center_focal_point.y,
						p_camera->center_focal_point.x + p_camera->velocity.x,
						p_camera->center_focal_point.y + p_camera->velocity.y);

	// apply velocities to camera's global position
	p_camera->global_pos.x += p_camera->velocity.x;
	p_camera->global_pos.y += p_camera->velocity.y;
	p_camera->global_tile_pos.x = p_camera->global_pos.x/TILE_SPRITE_SIZE;
	p_camera->global_tile_pos.y = p_camera->global_pos.y/TILE_SPRITE_SIZE;

}

void draw_camera(struct Camera *p_camera)
{
	if(p_camera->enable_debug)
	{
		DrawPrim(&p_camera->debug_center_focal_point);
		DrawPrim(&p_camera->debug_destination_focal_point);
		DrawPrim(&p_camera->debug_distance);
		DrawPrim(&p_camera->debug_velocity);
	}
}

