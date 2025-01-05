const unsigned char l_scr_ini [] = 				{ 0, 0 };
const unsigned char l_ini_x [] = 				{ 3, 5 };
const unsigned char l_ini_y [] = 				{ 3, 5 };
const unsigned char l_map_w [] = 				{ 5, 5 };
const unsigned char l_map_h [] =				{ 5, 5 };

const unsigned char l_scr_end [] = 				{ 0, 0 };
const unsigned char l_end_x [] = 				{ 3, 3 };
const unsigned char l_end_y [] = 				{ 3, 3 };
*/

// # of objects. 

const unsigned char l_player_max_objects [] =	{ MAX_HOTSPOTS_TYPE_1_0, 1 /*MAX_HOTSPOTS_TYPE_1_1*/ };

// # of killable enemies 

const unsigned char l_player_max_enems [] = 	{ KILLABLE_ENEMS_0, KILLABLE_ENEMS_1 };

// Palettes

const unsigned char * const l_pal_bg [] = 		{ palts0, palts1 };
const unsigned char * const l_pal_fg [] = 		{ palss0, palss0 };

// Metatiles

const unsigned char * const l_ts_tmaps [] = 	{ ts0_tmaps, ts1_tmaps };
const unsigned char * const l_ts_pals [] = 		{ ts0_pals, ts1_pals };
const unsigned char * const l_behs [] = 		{ behs0, behs1 };

// Enemy spriteset

const unsigned char * const * const l_spr_enems [] =
												{ spr_enems0, spr_enems1 };


#ifndef DEACTIVATE_KEYS
	const unsigned char * const l_locks [] = 	{ map_0_locks, map_1_locks };
	const unsigned char l_n_bolts [] = 			{ N_BOLTS_0, N_BOLTS_1 };
#endif

const unsigned char * const l_enems [] = 		{ enems_0, enems_1 };
const unsigned char * const l_hotspots [] = 	{ hotspots_0, hotspots_1 };
#endif

// Music track

const unsigned char l_music [] = 				{ 0, 1 };
