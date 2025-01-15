#include <neslib.h>
#ifndef TIMER_H
#define TIMER_H



// records absolute time via VSync(-1)
struct Timer
{
	int	vblnk_start;
	int	length;
	char	is_stopped;
};

void set_timer(struct Timer *, int vblnk_start, int length, int is_stopped);
char timer_has_completed(struct Timer *);

#endif

