#include "gui.h"

s_image_button::s_image_button(string p_text, int p_text_color,
					t_image p_image, t_vect p_coord, t_vect p_size, int border)
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
	image = p_image;
	funct = NULL;
	data = NULL;
	draw_funct = draw_centred_text;
}

s_image_button::s_image_button(t_image p_image, t_vect p_coord, t_vect p_size)
{
	text = "";
	text_size = -1;
	coord[0] = p_coord;
	size[0] = p_size;
	image = p_image;
	funct = NULL;
	data = NULL;
}

void		s_image_button::draw_self()
{
	image.draw_self(coord[0], size[0]);
	if (text != "")
	{
		if (text_size == -1 || size[1].x > calc_text_len(text, text_size) || size[1].y > get_char(text_size, BLACK, 'M')->surface->h)
			text_size = calc_text_size(text, size[1]);
		this->draw_funct(text, text_size, coord[2], text_color);
	}
}
