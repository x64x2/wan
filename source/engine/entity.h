#ifndef ENTITY_H
#define ENTITY_H

#include <engine/collision.h>
#include <engine/transform.h>

struct Entity
{
	char				is_on_floor;
	struct CollisionBox		colbox;
	struct Vector2x			velocity;
	struct Vector2x			global_tile_pos;
	struct Vector2x			global_pos;
};

// collision checks are done in a way so that the entity does not clip into a specified tile.
void process_entity_tile_collision(struct Entity *p_entity, SPRT *p_tile);

#endif

