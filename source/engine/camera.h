#ifndef CAMERA_H
#define CAMERA_H

#include <engine/psx.h>
#include <engine/transform.h>
#include <engine/entity.h>
#include <engine/display.h>

// the job of the camera is to cancel-out the on-screen movement of the player as much as possible and keep the player near the center of the
struct Camera
{
	// if camera is_free, the camera's center_focal_point does NOT try to keep
	// up with its destination_focal_point via velocity. A side effect of this is
	char			is_free;

	// smoothing is how loosely the center_focal_point tries to catch up to the
	char			smoothing;

	// the camera's velocity is what every other game object will move opposite 
	struct Vector2x		global_pos;
	struct Vector2x		global_tile_pos;

	struct Vector2x		center_focal_point;		// center of screen
	struct Vector2x		destination_focal_point;	// center of.. anything else (usually the player)

	// toggle drawing of debug members
	char			enable_debug;
};

void init_camera(struct Camera *p_camera, struct Vector2x *p_section_offset, struct Entity *p_entity);
void process_camera(struct Camera *p_camera, struct Entity *p_entity); // call this before calling move_section or draw_section()
void draw_camera(struct Camera *p_camera);

#endif

