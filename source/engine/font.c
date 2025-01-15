#include <engine/font.h>
#include <textbox.h>

void print_textbox(struct TextBox *p_box)
{
	int row, col;
	int horizontal_offset = 0, vertical_offset = 0; // offset in number of characters
	int index;
	int length;
	
	length = strlen(p_box->message_data);
	
	// set the tpage for all the SPRTs in the for loop
	DrawPrim(&p_box->dr_tpage);
	
	for(index = 0; index < length; index++)
	{
		if(p_box->message_data[index] == '\n')
		{
			horizontal_offset = 0;
			vertical_offset += 2;
			continue;
		}
		
		if(p_box->message_data[index] == ' ')
		{
			horizontal_offset += 1;
			continue;
		}
	}
}

void init_textbox_sprites(struct TextBox *p_box, unsigned short clut_id, unsigned short tpage_id)
{
	int i;
	
	// initialize textbox's tpage
	SetDrawTPage(&p_box->dr_tpage, 0, 0, tpage_id);
	
	// u0, v0, x0, and y0 are set in print_textbox(), not here
	for(i = 0; i < 128; i++)
	{
		SetSprt(&p_box->display_data[i]);
		
		p_box->display_data[i].w = p_box->char_w; // should be an even number
		p_box->display_data[i].h = p_box->char_h;
		
		p_box->display_data[i].clut = clut_id;
		
		p_box->display_data[i].r0 = 128;
		p_box->display_data[i].g0 = 128;
		p_box->display_data[i].b0 = 128;
	}
}

void set_text_color(struct TextBox *p_box, unsigned char red, unsigned char green, unsigned char blue)
{
	int i;
	
	for(i = 0; i < MAX_MESSAGE_LENGTH; i++)
	{
		p_box->display_data[i].r0 = red;
		p_box->display_data[i].g0 = green;
		p_box->display_data[i].b0 = blue;
	}
}

