#include <engine/gui.h>

#define	BOUNDARY_R	255
#define	BOUNDARY_G	255
#define	BOUNDARY_B	255

void init_tabwindow(struct TabWindow *p_tabwindow, struct ShortRECT *p_screen_pos, struct UCharRECT *p_texpage_pos, unsigned char total_tabs, unsigned char total_elements, unsigned short clut, unsigned short tpage, DR_TPAGE *dr_tpage)
{
	short	x, y, w, h;
	int	i, j;

	/// first initialize the window //
	x = p_tabwindow->x = p_screen_pos->x;
	y = p_tabwindow->y = p_screen_pos->y;
	w = p_tabwindow->w = p_screen_pos->w;
	h = p_tabwindow->h = p_screen_pos->h;

	p_tabwindow->total_tabs = total_tabs;
	p_tabwindow->total_locked_tabs = 0;
	p_tabwindow->tab_selected = 0;

	SetPolyF4(&p_tabwindow->window_background);
	setRGB0(&p_tabwindow->window_background, 64, 64, 64);

	p_tabwindow->window_background.x0 = p_tabwindow->window_background.x2 = x; // left side
	p_tabwindow->window_background.x1 = p_tabwindow->window_background.x3 = x+w; // right side
	p_tabwindow->window_background.y0 = p_tabwindow->window_background.y1 = y; // top side
	p_tabwindow->window_background.y2 = p_tabwindow->window_background.y3 = y+h; // bottom side

	p_tabwindow->dr_tpage = *dr_tpage;

	//////// now initialize the tabs ////////
	p_tabwindow->p_tab = (struct TabHeader *)allocmem(total_tabs, sizeof(struct TabHeader));

	if(p_tabwindow->p_tab == NULL)
	{
		printf("FAILED TO ALLOCATE p_tabwindow->p_tab!\n");
		while(1);
	}
	else
	{
		printf("SUCCESS ON ALLOCATION OF p_tabwindow->p_tab!\n");
	}


	for(i = 0; i < total_tabs; i++)
	{
		//// setup initial values ////

		// every tab is unlocked upon initialization
		p_tabwindow->p_tab[i].is_locked = 0;
		p_tabwindow->p_tab[i].total_elements = total_elements;
		p_tabwindow->p_tab[i].total_locked_elements = 0;
		p_tabwindow->p_tab[i].element_selected = 0;

		//// setup textbox ////
		p_tabwindow->p_tab[i].textbox.x = x + i*TABHEADER_WIDTH;
		p_tabwindow->p_tab[i].textbox.y = y+2;

		p_tabwindow->p_tab[i].textbox.char_w = 6;
		p_tabwindow->p_tab[i].textbox.char_h = 8;

		p_tabwindow->p_tab[i].textbox.u = 0;
		p_tabwindow->p_tab[i].textbox.v = 0;

		p_tabwindow->p_tab[i].textbox.max_x = 20;

		init_textbox_sprites(&p_tabwindow->p_tab[i].textbox, clut, tpage);

		strcpy(p_tabwindow->p_tab[i].textbox.message_data, "EMPTY!");

		//// setup icon ////
		SetSprt8(&p_tabwindow->p_tab[i].icon);
		setRGB0(&p_tabwindow->p_tab[i].icon, 128, 128, 128);
		setXY0(&p_tabwindow->p_tab[i].icon, x+24 + i*TABHEADER_WIDTH, y-12);
		setUV0(&p_tabwindow->p_tab[i].icon, p_texpage_pos->x, p_texpage_pos->y);
		p_tabwindow->p_tab[i].icon.clut = clut;

		//// setup selected_element_outline ////
		SetTile(&p_tabwindow->p_tab[i].selected_element_outline);
		setRGB0(&p_tabwindow->p_tab[i].selected_element_outline, 0, 0, 0);
		setXY0(&p_tabwindow->p_tab[i].selected_element_outline, x+8+3, y+20+3);
		setWH(&p_tabwindow->p_tab[i].selected_element_outline, 10, 10);

	}
}

void control_tabwindow(struct TabWindow *p_tabwindow)
{
	unsigned char tab_selected = p_tabwindow->tab_selected;
	unsigned char total_elements = p_tabwindow->p_tab[tab_selected].total_elements;

	if(is_button_just_pressed(0, PADRIGHT))
	{
		// do not overrun
		if(p_tabwindow->p_tab[tab_selected].element_selected < total_elements-1)
		{
			p_tabwindow->p_tab[tab_selected].element_selected++;
		}
		else if (is_button_just_pressed(0, PADDOWN))
	}
}

void draw_tabwindow(struct TabWindow *p_tabwindow)
{
	int		i;
	short		x, y, w, h;
	unsigned char	tab_selected = p_tabwindow->tab_selected;
	unsigned char	element_selected = p_tabwindow->p_tab[tab_selected].element_selected;

	x = p_tabwindow->x;
	y = p_tabwindow->y;
	w = p_tabwindow->w;
	h = p_tabwindow->h;

	// ensure that the background and textboxes are in the correct positions.
	p_tabwindow->window_background.x0 = p_tabwindow->window_background.x2 = x;
	p_tabwindow->window_background.x1 = p_tabwindow->window_background.x3 = x+w;

	DrawPrim(&p_tabwindow->window_background);

	// determine the correct position of selected_element_outline
	p_tabwindow->p_tab[tab_selected].selected_element_outline.x0 = x+8+3+( (element_selected%15)*16);
	p_tabwindow->p_tab[tab_selected].selected_element_outline.y0 = y+20+3+( ( element_selected/15)*16);

	// draw the outline before drawing the elements (which are TILEs)
	DrawPrim(&p_tabwindow->p_tab[tab_selected].selected_element_outline);

	// draw all of the selected tab's elements
	for(i = 0; i < p_tabwindow->p_tab[tab_selected].total_elements; i++)
	{
		// if the element is locked, make it red
		if(p_tabwindow->p_tab[tab_selected].p_element[i].is_locked)
		{
			setRGB0(&p_tabwindow->p_tab[tab_selected].p_element[i].tile, 128, 0, 0);
		}

		// ensure that each element is placed correctly if tab window has moved
		p_tabwindow->p_tab[tab_selected].p_element[i].tile.x0 = x+8+4+((i%15)*16);
		DrawPrim(&p_tabwindow->p_tab[tab_selected].p_element[i].tile);
	}

	// now draw the TabWindow's header
	for(i = 0; i < p_tabwindow->total_tabs; i++)
	{
		DrawPrim(&p_tabwindow->dr_tpage);

		// ensure that each icon is placed correctly if tab window has moved
		p_tabwindow->p_tab[i].icon.x0 = x+24 + i*TABHEADER_WIDTH;
		DrawPrim(&p_tabwindow->p_tab[i].icon);

		// ensure that each textbox is placed correctly if tab window has moved
		p_tabwindow->p_tab[i].textbox.x = x + i*64;
		print_textbox(&p_tabwindow->p_tab[i].textbox);
	}
}

void free_tabwindow(struct TabWindow *p_tabwindow)
{
	int i, j;

	// free every element array from the each tab first
	for(i = 0; i < p_tabwindow->total_tabs; i++)
	{
		freemem((char *)p_tabwindow->p_tab[i].p_element);
	}

	// now free the tab array
	freemem((char *)p_tabwindow->p_tab);
}

void init_slider(struct Slider *p_slider, struct ShortRECT *p_screen_pos, struct UCharRECT *p_texpage_pos, short min, short max, int difference, enum SliderStyle style, unsigned short clut, unsigned short tpage)
{
	p_slider->x = p_screen_pos->x;
	p_slider->y = p_screen_pos->y;
	p_slider->w = p_screen_pos->w;
	p_slider->h = p_screen_pos->h;

	p_slider->min = min;
	p_slider->max = max;

	p_slider->difference = difference;

	p_slider->current_value = 0x60;

	//// initialize "simple style" slider ////
	SetLineF2(&p_slider->bar);
	setRGB0(&p_slider->bar, 0, 0, 0);
	setXY2(&p_slider->bar, p_screen_pos->x, p_screen_pos->y, p_screen_pos->x+p_screen_pos->w, p_screen_pos->y);

	SetSprt8(&p_slider->handle);
	setRGB0(&p_slider->handle, 128, 128, 128);
	setXY0(&p_slider->handle, p_screen_pos->x, p_screen_pos->y-4);
	setUV0(&p_slider->handle, p_texpage_pos->x, p_texpage_pos->y);

	p_slider->handle.clut = clut;

	//// FIRST setup the LINE_F2
	SetLineF2(&p_slider->left_boundary);
	setRGB0(&p_slider->left_boundary, BOUNDARY_R, BOUNDARY_G, BOUNDARY_B);

	// bottom left
	p_slider->left_boundary.x0 = p_screen_pos->x;
	p_slider->left_boundary.y0 = p_screen_pos->y + p_screen_pos->h;

	// top left
	p_slider->left_boundary.x1 = p_screen_pos->x;
	p_slider->left_boundary.y1 = p_screen_pos->y;

	//// THEN setup the LINE_F4
	SetLineF4(&p_slider->boundary);
	setRGB0(&p_slider->boundary, BOUNDARY_R, BOUNDARY_G, BOUNDARY_B);

	// top left
	p_slider->boundary.x0 = p_screen_pos->x;
	p_slider->boundary.y0 = p_screen_pos->y;

	// top right
	p_slider->boundary.x1 = p_screen_pos->x + p_screen_pos->w;
	p_slider->boundary.y1 = p_screen_pos->y;

	// bottom right
	p_slider->boundary.x2 = p_screen_pos->x + p_screen_pos->w;
	p_slider->boundary.y2 = p_screen_pos->y + p_screen_pos->h;

	// bottom left
	p_slider->boundary.x3 = p_screen_pos->x;
	p_slider->boundary.y3 = p_screen_pos->y + p_screen_pos->h;

	//// FINALLY setup the POLY_F4
	SetPolyF4(&p_slider->fill_in);
	setRGB0(&p_slider->fill_in, 255, 255, 255);

	// top left
	p_slider->fill_in.x0 = p_slider->boundary.x0+1;
	p_slider->fill_in.y0 = p_slider->boundary.y0+1;

	// top right
	p_slider->fill_in.x1 = p_slider->boundary.x1;
	p_slider->fill_in.y1 = p_slider->boundary.y1+1;

	// bottom left
	p_slider->fill_in.x2 = p_slider->boundary.x3+1;
	p_slider->fill_in.y2 = p_slider->boundary.y3;

	// bottom right
	p_slider->fill_in.x3 = p_slider->boundary.x2;
	p_slider->fill_in.y3 = p_slider->boundary.y2;

	// set selected style
	p_slider->style = style;

	// set DR_TPAGE primitive
	SetDrawTPage(&p_slider->dr_tpage, 0, 0, tpage);
}

void draw_slider(struct Slider *p_slider)
{
	if(p_slider->style)
	{
		DrawPrim(&p_slider->dr_tpage);

		p_slider->handle.x0 = p_slider->x + p_slider->current_value;
		DrawPrim(&p_slider->bar);
		DrawPrim(&p_slider->handle);

		// right side of fill_in
		p_slider->fill_in.x1 = p_slider->x + p_slider->current_value;
		p_slider->fill_in.x3 = p_slider->x + p_slider->current_value;
	
	    DrawPrim(&p_slider->left_boundary);
		DrawPrim(&p_slider->boundary);
	    DrawPrim(&p_slider->fill_in);		
			
	}
}


