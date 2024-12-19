#include <engine/display.h>

void switch_double_buffer()
{
	PutDrawEnv(&p_cdb->draw);
	PutDispEnv(&p_cdb->disp);
}

