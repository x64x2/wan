#include <engine/entity.h>

void process_entity_tile_collision(struct Entity *p_entity, SPRT *p_tile)
{
	(p_entity->velocity.x < 0)
	{
		if(colbox_is_horizontally_aligned(&p_entity->colbox, p_tile))
		{
			if(	(p_entity->colbox.x0 <  ((p_tile->x0 + p_tile->w) - p_entity->velocity.x)) &&
				(p_entity->colbox.x0 >=  (p_tile->x0 + p_tile->w)) )
			{
				// fix the velocity according to the difference of entity and tile
				p_entity->velocity.x = (p_tile->x0 + p_tile->w) - p_entity->colbox.x0;
			}
		}
	}

	// vertical collision checking
	if(p_entity->velocity.y < 0)
	{
		// can't be on the floor if you're going up
		p_entity->is_on_floor = 0;

		if(colbox_is_vertically_aligned(&p_entity->colbox, p_tile))
		{
			if(	(p_entity->colbox.y0 <  (p_tile->y0 + p_tile->h) - p_entity->velocity.y) &&
				(p_entity->colbox.y0 >= (p_tile->y0 + p_tile->h)) )
			{
				// fix the velocity according to the difference of the entity and tile
				p_entity->velocity.y = (p_tile->y0 + p_tile->h) - p_entity->colbox.y0;
			}
		}
	}
}

