#include "gui.h"

s_text_button::s_text_button(	string p_text, int p_text_color, //text info
								t_vect p_coord, t_vect p_size, int border, //object info
								t_color p_color1, t_color p_color2) // color info
{
	text = p_text;
	text_color = p_text_color;
	coord[0] = p_coord;
	size[0] = p_size;
	coord[1] = p_coord + border;
	size[1] = p_size - (border * 2);
	coord[2] = coord[1] + size[1] / 2;
	if (text != "")
		text_size = calc_text_size(text, size[1]);
	else
		text_size = -1;
	color[0] = p_color1;
	color[1] = p_color2;
	funct = NULL;
	data = NULL;
	draw_funct = draw_centred_text;
}

void		s_text_button::draw_self()
{
	for (int i = 0 ; i < 2 ; i++)
		draw_rectangle(coord[i], size[i], color[i]);
	if (text != "")
	{
		if (text_size == -1 || size[1].x > calc_text_len(text, text_size) || size[1].y > get_char(text_size, BLACK, 'M')->surface->h)
			text_size = calc_text_size(text, size[1]);
		this->draw_funct(text, text_size, coord[2], text_color);
	}
}
