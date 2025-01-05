// LUTs
const unsigned char bitmasks [] = {0xfc, 0xf3, 0xcf, 0x3f};

#ifdef ENABLE_MONOCOCOS
	const unsigned char monococo_state_times [] = {
		MONOCOCO_BASE_TIME_HIDDEN, MONOCOCO_BASE_TIME_APPEARING, MONOCOCO_BASE_TIME_ONBOARD, MONOCOCO_BASE_TIME_APPEARING
	};
#endif

#if defined (ENABLE_COCOS) && defined(COCOS_ENABLE_LINEAR)
	// left, up, right, down
	const signed int coco_dx [] = { -COCO_V, 0, COCO_V, 0 };
	const signed int coco_dy [] = { 0, -COCO_V, 0, COCO_V };
#endif

#if defined (ENABLE_COMPILED_ENEMS)
	// Directions are
	const signed char endx [] = {-1, -1, 0, 1, 1, 1, 0, -1};
	const signed char endy [] = {0, 1, 1, 1, 0, -1, -1, -1};
#endif

const unsigned char bits [] = {
	1, 2, 4, 8, 16, 32, 64, 128
};

#ifdef ENEMS_MAY_DIE
	const unsigned char jitter [] = { 0,1,1,0,0,1,0,1,1,0,0,0,1,0,1,1 };
#endif

