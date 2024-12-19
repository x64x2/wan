#ifndef INIT_H
#define INIT_H

#include <engine/psx.h>
#include <engine/display.h>
#include <engine/controls.h>
#include <engine/asset.h>
#include <engine/allocmem.h>

#include <game/gamestate.h>

void init(); // init

//// display 
void init_graphics();
void init_double_buffer();

// control/
void init_controller();

#endif

