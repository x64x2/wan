void propellers_add (void) {
	// Store YX for animation
	prp_yx [prp_idx ++] = rdm;
}

void propellers_do (void) {
	rdb = half_life << 1;
	gpit = prp_idx; while (gpit --) {
		rda = prp_yx [gpit];
		gp_addr = 0x2000 + (
			((rda & 0xf) << 1) | 
			((((rda >> 3) & 0xfe) + TOP_ADJUST) << 5)
		);
		_n = rdb + PROPELLERS_BASE_PATTERN; ul_putc ();
		++ _n; ul_putc ();
	}
}

