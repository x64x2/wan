if (no_ct) {
	-- no_ct;
	oam_index = oam_meta_spr (
		prx + NO_OFFS_X, pry + NO_OFFS_Y + SPRITE_ADJUST,
		oam_index,
		NO_METASPRITE
	);
}
