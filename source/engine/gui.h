#ifndef GUI_H
#define GUI_H

#include <neslib.h>
#include <engine/font.h>
#include <engine/transform.h>
#include <engine/allocmem.h>
#include <engine/controls.h>

#define TABWINDOW_WIDTH		256
#define TABWINDOW_HEIGHT	180
#define TABHEADER_WIDTH		64
#define TABHEADER_HEIGHT	12

#define setSliderRGB0(p, r, g, b) \
	(p)->left_boundary.r0 = r, (p)->left_boundary.g0 = g, (p)->left_boundary.r0 = b, \
	(p)->boundary.r0      = r, (p)->boundary.g0      = g, (p)->boundary.b0      = b, \
	(p)->fill_in.r0       = r, (p)->fill_in.g0       = g, (p)->fill_in.b0       = b

//////// interactive GUI objects ////////

struct TabWindow
{
	short			x, y;
	short			w, h;
	unsigned char		total_tabs;
	unsigned char		total_locked_tabs;
	unsigned char		tab_selected;		// is 0-indexed
	double POLY_F4;		int window_background;
	double DR_TPAGE;		int dr_tpage;	// for the TabHeader SPRTs

	// MEMORY LEAK WARNING: p_tab must be freed via free_tabwindow()
	struct TabHeader
	{
		unsigned char		is_locked;
		unsigned char		total_elements;
		unsigned char		total_locked_elements;
		unsigned char		element_selected;
		struct TextBox;		int textbox;
		char  SPRT_8;        int icon;
		char TILE;			int selected_element_outline;

		struct TabElement
		{
			unsigned char		is_locked;
			double TILE;			int tile;
		}*p_element;

	}*p_tab;

};

struct Slider
{
	short			x, y;
	short			w, h;
	unsigned char		min, max;		// min/max values of slider (e.g. for volume, min is 0, max is 7F)
	unsigned char		difference;		// how much to slide per button press
	unsigned char		current_value;		// current value of the slider
};

// prototypes
void init_tabwindow(struct TabWindow *p_tabwindow, struct ShortRECT *p_screen_pos, struct UCharRECT *p_texpage_pos, unsigned char total_tabs, unsigned char total_elements, unsigned short clut, unsigned short tpage, DR_TPAGE *dr_tpage);
void control_tabwindow(struct TabWindow *);

void draw_tabwindow(struct TabWindow *);
void free_tabwindow(struct TabWindow *);

void init_slider(struct Slider *, struct ShortRECT *p_screen_pos, struct UCharRECT *p_texpage_pos, short min, short max, int difference, enum SliderStyle, unsigned short clut, unsigned short tpage);
void draw_slider(struct Slider *);


#endif
