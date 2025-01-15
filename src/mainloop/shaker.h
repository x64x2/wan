if (shaker_ct) {
	-- shaker_ct;
	scroll (254 + (rand8 () & 3), SCROLL_Y - 2 + (rand8 () & 3));
	if (shaker_ct == 0) scroll (0, SCROLL_Y);
}
