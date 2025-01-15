#include <engine/timer.h>

void set_timer(struct Timer *p_timer, int vblnk_start, int length, int is_stopped)
{
	p_timer->vblnk_start = vblnk_start;
	p_timer->length = length;
	p_timer->is_stopped = is_stopped;
}

char timer_has_completed(struct Timer *p_timer)
{
	// if the current number of VSyncs passed is larger than what you started with +
	if(VSync(-1) > (p_timer->vblnk_start + p_timer->length))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

