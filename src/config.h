#define GAME_AREA_MIDDLE
#define GAME_AREA_BOTTOM
#define GAME_AREA_CUSTOM

#define MAP_CLEAR_LIST_ADDRESS	0x2c40	// To store map persistence in VRAM.
										// Each screen takes 24 bytes. move it where
										// It fits. Below 2c40 you may "see" the buffer


//define map dimmensions, initial and authomatic ending conditions, etc.

#define MAP_W 					3		//
#define MAP_H					3		// Map dimensions in screens
#define MAP_SIZE				MAP_W*MAP_H

#define PLAYER_LIFE				5		// Max / Initial life
#define PLAYER_REFILL			1		// Life refill

//#define MULTI_LEVEL					// Comment for single level
#define MAX_LEVELS				1

#define MAX_FLAGS 				2		// Keep it low if you don't need flags

#ifndef MULTI_LEVEL

	// If you aim for a single level ROM, fill in those:

	#define SCR_INI					0		// Initial screen
	#define PLAYER_INI_X			2		//
	#define PLAYER_INI_Y			2		// Initial position
	//#define SCR_END				99		// Final screen, undefine if N/A
	#define PLAYER_END_X			99		//
	#define PLAYER_END_Y			99		// Ending position
	#define PLAYER_MAX_OBJECTS		1		// Objects to finish game

#else

	#define SCR_INI 				(l_scr_ini [level])
	#define PLAYER_INI_X			(l_ini_x [level])
	#define PLAYER_INI_Y			(l_ini_y [level])
	//#define SCR_END				(l_scr_end [level])
	#define PLAYER_END_X			(l_end_x [level])
	#define PLAYER_END_Y			(l_end_y [level])
	#define PLAYER_MAX_OBJECTS		(l_player_max_objects [level])

#endif

// Some flexibility
#define HOTSPOT_TYPE_OBJECT		1
#define HOTSPOT_TYPE_KEYS		2
#define HOTSPOT_TYPE_REFILL		3
#define HOTSPOT_TYPE_AMMO		4
#define HOTSPOT_TYPE_TIME		5		// For time refills

#define HOTSPOT_TYPE_RESONATOR 	4		// An example of custom hotspot
#define HOTSPOT_TYPE_RESONATOR_ON 5		// Custom for this game.

#define WIN_LEVEL_CUSTOM				// A level ends when win_level == 1
										// And such a thing has to be setup by YOU

#define KILL_PLAYER_CUSTOM			// Advanced custom handling. Beware!

#define MAP_FORMAT_PACKED				// Each byte in map data = 2 tiles, 16 tiles max.
#define MAP_FORMAT_RLE16				// RLE'd by rlemap2. 16 tiles max.
#define MAP_FORMAT_RLE53				// RLE'd by rle53mapMK1. 32 tiles max.
#define MAP_FORMAT_RLE44				// RLE'd by rle44mapMK1. 16 tiles max.
#define MAP_FORMAT_RLE53_CHRROM		// RLE'd by rle53mapchrrom and stored in CHR-ROM. 32 tiles max.
#define MAP_FORMAT_RLE44_CHRROM		// RLE'd by rle44mapchrrom and stored in CHR-ROM. 16 tiles max.

#define MAP_WITH_DECORATIONS			// Add decorations when use a 16 tiles mode.


#define MAP_RENDERER_COMPLEX			// Comment for the simple, old renderer

// ============================================================================
// II. Engine type
// ============================================================================

// Player is 8x16 for collision with BG but can be made taller by this amount. Negative values=shorter
#define PLAYER_COLLISION_VSTRETCH_BG	-4

#define PLAYER_COLLISION_VSTRETCH_FG 	-4


#define ENEMS_COLLISION_VSTRETCH_FG		0

// General directives:

#define PLAYER_PUSH_BOXES 				// If defined, tile beh 11 is pushable
#define FIRE_TO_PUSH

#define ENABLE_PUSHED_SCRIPT
#define PUSHED_TILE_FLAG				1
#define PUSHED_TO_X_FLAG				2
#define PUSHED_TO_Y_FLAG				3

// General stuff

#define DEACTIVATE_KEYS						// If defined, keys are not present.
#define DEACTIVATE_OBJECTS				// If defined, objects are not present.
#define PLAYER_BOUNCES				16
#define DOUBLE_BOUNCE
#define DIE_AND_RESPAWN						// If defined, dying = respawn on latest safe.
#define DIE_AND_REENTER				//     ... also, reenter screen on death
#define DIE_AND_REINIT				//     ... or start the level over!

// Breakable, beh & 16
#define BREAKABLE_LIFE					2	// Amount of hits to break wall
#define BREAKABLE_ANIM						// Show explosion when breaking
#define BREAKABLE_MAX					4	// Max # of concurrent explosions
#define BREAKABLE_MAX_FRAMES			8	// Frames to show explosion
#define BREAKABLE_ERASE					0	// Tile to erase broken tiles
#define BREAKABLE_BREAKING				8	// Tile to display while breaking

// Propellers

//#define ENABLE_PROPELLERS
#define PROPELLERS_MAX 					4
#define PROPELLERS_BASE_PATTERN			64	// First of 4 patterns to draw/animate propellers
#define PROPELLERS_MAX_LENGTH			6	// In tiles; undef for infinite
#define PROPELLER_TILE					14	// Tile # in map to detect a propeller
#define PROPELLERS_ON_BY_DEFAULT			// Define so propellers are on by default

// Killing tiles shine, beware!

//#define ENABLE_SHINES
#define SHINES_MAX 						8
#define SHINES_BASE_PATTERN				10	// In the sprite bank, two patterns needed
#define SHINES_PALETTE					3
#define SHINING_TILE					23	// Tile # in map to add shines

// Resonators. Not sure if you need this, but.

#define RESONATOR_BASE_PATTERN			0
#define RESONATOR_PALETTE				3
#define RESONATOR_COUNTER_OFFS_X		4
#define RESONATOR_COUNTER_OFFS_Y		7

// Springs

#define SPRING_TILE 					10
#define SPRING_SPIKE_TILE 				11
#define SIMPLE_WARPERS_BASE_SPRID		(32+((frame_counter>>2)&3))

#define NO_METASPRITE					ssit_06
#define NO_OFFS_X						0
#define NO_OFFS_Y						-24

// Use animation.

#define USE_ANIM_MAX_FRAMES				13
#define USE_ANIM_INTERACT_ON			7
#define USE_ANIM_FRAMES_PER_STEP		4

// Text boxes. If you need them, you call them

#define TEXT_BOX_FRAME_TILE_OFFSET		0x20	// We need tiles in the active metatileset to draw the frame

// Timer. 

//#define ENABLE_TIMER
#define TIMER_INITIAL					99
#define TIMER_START_ON
#define TIMER_REFILL					30			1	// Useful with scripting. raises flag when time zero
#define TIMER_SOUND						10  // play a sound if timer < value


// Enemy types and definitions
#define ENEMS_LIFE_GAUGE				1	// Amount of shots/punches/kicks needed to kill enemies.

#define ENEMS_TOUCHED_FRAMES			8	// # frames to stay frozen after hit
/
#define ENEMS_EXPLODING_CELL 			32
#define ENEMS_EXPLODING_CELLS_HIDES			// Define and the baddie will be substituted by the explosion

#define ENEMS_OCCLUDING_CELL			33	// If you use pezons or saws you need a flame for occlusion

#define FANTY_BASE_SPRID				32
#define FANTY_WITH_FACING
#define FANTY_COLLIDES

#define FANTY_A 						4
#define FANTY_MAXV 						48

#define FANTY_DISTANCE					80
#define FANTY_V_RETREAT					16

#define FANTY_BASE_TIMER				120	// Timed fanties use this

// Pursuers (for top-down)

#define DEATH_COUNT_EXPRESSION			50+(rand8()&63)
#define TYPE_7_FIXED_SPRITE 			4	// If defined, type 7 enemies are always #

#define SAW_BASE_SPRID					48
#define SAW_V_DISPL						4
#define SAW_EMERGING_STEPS				10

#define PEZONS_BASE_SPRID				40
#define PEZON_WAIT						50
#define PEZON_THRUST					384
#define PEZON_VY_FALLING_MAX			256
#define PEZON_G							16

// Compiled enemies

//#define ENABLE_COMPILED_ENEMS
#define COMPILED_ENEMS_SHOOT
#define COMPILED_ENEMS_BASE_SPRID		4

#define HS_OBJ_EMPTY					4
#define HS_OBJ_MIN						5
#define HS_OBJ_MAX						6
#define HS_USE_OFFS						2

#define HS_TYPE_A 						// If defined, render receptors = HS_OBJ_EMPTY, object used = object
										// Otherwise, use its own graphics.
#define HS_FIX_ON_USE					// If defined, object N used at N+H_USE_OFFS becomes N+H_USE_OFFS*2


// Scripting

#define ACTIVATE_SCRIPTING			// Activates msc scripting and flag related stuff.
#define CLEAR_FLAGS					// Zero all flags when entering a level

#define ENABLE_EXTERN_CODE			// Enables custom code to be run from the script using EXTERN n
#define ENABLE_FIRE_ZONE				// Allows to define a zone which auto-triggers "FIRE"

// Interactives
#define ENABLE_INTERACTIVES				// Graphical flag representations!
#define INTERACTIVES_MAX 				4	// As per screen
#define FLAG_INVENTORY					0
#define INTERACTIVES_ONLY_SPRITES			// If you are using other means for items and just need sprites

// ============================================================================
// IV. Player movement configuration
// ============================================================================

#define PLAYER_VY_FALLING_MAX	256		// Max. velocity when falling
#define PLAYER_VY_FALLING_MIN	64		// Use for animating if you need
#define PLAYER_VY_SINKING		2
#define PLAYER_G				16		// Gravity

#define PLAYER_VY_JUMP_INITIAL	64
#define PLAYER_VY_JUMP_INITIAL_TRAMPOLINE 128
#define PLAYER_VY_JUMP_MAX		192		// Max. velocity when jumping
#define PLAYER_AY_JUMP 			12		// Jumpin acceleration 

#define PLAYER_AY_JETPAC		32		// Jetpac increment
#define PLAYER_VY_JETPAC_MAX	256		// Max jetpac vertical speed

#define PLAYER_AY_SWIM			8		// Swimming acceleration.
#define PLAYER_VY_SWIM_MAX		64		// Swimming max. speed

#define PLAYER_VY_LADDERS		96

#define PLAYER_AY_FLOAT			16	
#define PLAYER_VY_FLOAT_MAX		256

#define PLAYER_AY_UNTHRUST 		8 		// Used in the Autojump engine.

// IV.1.b MK2 style jump (overrides PLAYER_?Y_JUMP_* defined before!)
// (Used if PLAYER_JUMP_TYPE_MK2 is defined)
#define PLAYER_G_MK2_JUMPING		4
#define PLAYER_VY_MK2_JUMP_INITIAL	208
#define PLAYER_VY_MK2_JUMP_RELEASE	96
#define PLAYER_VY_MK2_JUMP_A_STEPS 	16
#define PLAYER_VY_MK2_JUMP_INITIAL_TRAMPOLINE 296
#define PLAYER_VY_MK2_TRAMPOLINE_A_STEPS 32	// For trampolines

// IV.2. Horizontal (side view) or general (top view) movement.

#define PLAYER_VX_MAX			128		// Max. horizontal speed
#define PLAYER_VX_CONVEYORS 	64
#define PLAYER_AX				16		// Horizontal acceleration
#define PLAYER_AX_ICE			4
#define PLAYER_RX				16		// Horizontal friction
#define PLAYER_RX_ICE			2

#define PLAYER_VX_MIN (PLAYER_AX << 1)

#define PLAYER_V_REBOUND		224

// Animation cells 

#ifdef PLAYER_TOP_DOWN

	// Cell definitions for top-down view

	#define CELL_FACING_RIGHT 	0
	#define CELL_FACING_LEFT 	6
	#define CELL_FACING_UP 		18
	#define CELL_FACING_DOWN 	12

	#define CELL_IDLE			0
	#define CELL_WALK_CYCLE		1
	#define CELL_PUSHING		5

	#define CELL_USE			24

#else

	// Cell definitions for side view

	#define CELL_FACING_RIGHT	0
	#define CELL_FACING_LEFT	8

	#define CELL_IDLE			0
	#define CELL_WALK_INIT		1
	#define CELL_WALK_CYCLE		1
	#define CELL_AIRBORNE		5

	#define CELL_SPIN_CYCLE		5

	#define CELL_ASCENDING		5
	#define CELL_DESCENDING		6

	#define CELL_SWIM_CYCLE		6

	#define CELL_USE			6

	#define CELL_PUNCHING		8
	#define CELL_KICKING		9

	#define CELL_HIT 			9

	#define CELL_CLIMB_CYCLE	20
	#define CELL_CLIMB_HALF		29
#endif

// Music

#define MUSIC_INGAME 			0

// SFX

#define SFX_START				0
#define SFX_TILE				1
#define SFX_OBJECT				2
#define SFX_USE					3
#define SFX_TRAMPOLINE			3
#define SFX_PHIT				4
#define SFX_DUMMY1				5
#define SFX_ENHIT 				6
#define SFX_DUMMY2				7
#define SFX_JUMP				8
#define SFX_BULLET				9
#define SFX_COCO				10
#define SFX_SPRING				11
#define SFX_COUNT				12
#define SFX_BREAKH				13
#define SFX_HITTER				14
#define SFX_STEPON				15
#define SFX_FLOAT				16
#define SFX_BREAKB				17
#define SFX_RING 				18
#define SFX_FANFARE				19
