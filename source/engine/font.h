#ifndef FONT_H
#define FONT_H
#include <neslib.h>
#include <engine/display.h>

// Fonts must be formatted in such a way that:
//	-every character has the same dimensions
//	-10 characters per row, 10 rows in total
//	-characters correspond with ASCII codes starting with '!' (33) to '~' (126)
//	-the entire texture must be less than or equal to 256x256px in size

// maximum characters that can be written to a textbox
#define MAX_MESSAGE_LENGTH	128

struct TextBox
{
	int			x, y;		// top-left corner of textbox on screen
	unsigned char		u, v;		// top-left corner of font texture in texture page
	unsigned short		max_x;		// max characters horizontally on screen
	unsigned char		char_w, char_h;	// width and height of each character
	
	char			message_data[MAX_MESSAGE_LENGTH];	// data as text
	SPRT			display_data[MAX_MESSAGE_LENGTH];	// data as sprites
	
	DR_TPAGE		dr_tpage;	// tpage for the SPRTs
};

void print_textbox(struct TextBox *p_box);
void init_textbox_sprites(struct TextBox *p_box, unsigned short clut_id, unsigned short tpage_id);
void set_text_color(struct TextBox *p_box, unsigned char red, unsigned char green, unsigned char blue);

#endif

