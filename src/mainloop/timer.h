if (timer_on && timer) {
	if (timer_frames) -- timer_frames; else {
		timer_frames = 50;
		-- timer; 

		#ifdef TIMER_SOUND
			if (timer < TIMER_SOUND) sfx_play (SFX_COUNT, 2);
		#endif

		if (timer == 0) {
			#ifdef TIMER_ZERO_FLAG
				flags [TIMER_ZERO_FLAG] = 
			#endif
			timer_zero = 1;
			#ifdef ACTIVATE_SCRIPTING
				hud_update (); 
				run_script (2 * MAP_SIZE + 3);
			#endif
		}
	}
}
#ifdef TIMER_TIME_FLAG
	flags [TIMER_TIME_FLAG] = timer;
#endif
