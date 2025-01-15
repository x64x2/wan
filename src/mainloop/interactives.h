if (interactive_index) {
	if (
		b_button
		#ifdef ENABLE_USE_ANIM
			&& use_ct == 0
		#endif
	) {
		rdx = prx + 4; rdy = pry + 8;

		gpit = interactive_index; while (gpit --) {
			rda = interactive_yx [gpit]; rdb = rda << 4; rdc = rda & 0xf0;
			if (
				(rdx >= rdb - 4 && rdx <= rdb + 19 && rdy >= rdc && rdy <= rdc + 15)
				#ifndef PLAYER_TOP_DOWN
					/*&& ppossee*/
				#endif
			) {
				#if defined (ENABLE_USE_ANIM) && !defined (INTERACTIVES_ONLY_SPRITES)
					// Only if it is a genuine interactive				
					if (interactive_f [gpit] & 0x80) {
						use_ct = 1; use_sub_ct = USE_ANIM_FRAMES_PER_STEP;
						use_type = USE_TYPE_INTERACTIVE;
					}
				#endif
				interactive_interact_with = gpit;
				b_button = 0;
				break;
			}
		}
	}

	if (interactive_interact_with != 0xff) {
		rdc = interactives_f [interactive_interact_with];
		#ifdef ACTIVATE_SCRIPTING
			script_arg = rdc;
		#endif

		#if defined (ENABLE_USE_ANIM) && !defined (INTERACTIVE_ONLY_SPRITES)
			if (
				(use_ct == USE_ANIM_INTERACT_ON && use_sub_ct == USE_ANIM_FRAMES_PER_STEP)
				|| (rdc & 0x80) == 0
			)
		#endif
		{
			#if !defined (INTERACTIVE_ONLY_SPRITES)
				if (rdc & 0x80) {	
					rda = rdc & 0x7f;
					rdb = flags [rda];
					flags [rda] = flags [FLAG_INVENTORY];
					flags [FLAG_INVENTORY] = rdb;
					sfx_play (SFX_OBJECT, 1);
				} 
			#endif

			#ifdef ACTIVATE_SCRIPTING
				// Run script; script_arg is set, always success
				just_interacted = 1;
				run_fire_script ();
				fire_script_success = 1;
			#endif	

			interactive_interact_with = 0xff;
		}
	}
}
