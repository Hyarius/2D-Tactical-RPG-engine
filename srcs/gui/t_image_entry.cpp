#include "gui.h"

s_image_entry::s_image_entry(	string p_desc, string p_text, int p_text_color,
								t_image p_image, t_image p_selec_image,
								t_vect p_coord, t_vect p_size, int border)
{
	desc = p_desc;
	text = p_text;
	back = "";
	front = "";
	text_color = p_text_color;
	max_len = 32;
	coord[0] = p_coord;
	size[0] = p_size;
	coord[1] = p_coord + border;
	size[1] = p_size - (border * 2);
	coord[2] = coord[1] + t_vect(0, size[1].y / 2);
	if (desc != "")
		desc_size = calc_text_max_size("M", size[1] - border * 2);
	text_size = calc_text_max_size("M", size[1] - border * 2);
	selected = false;
	image = p_image;
	selec_image = p_selec_image;
	draw_funct = draw_lined_text;
}

void		s_image_entry::draw_self()
{
	if (selected == true)
		selec_image.draw_self(coord[0], size[0]);
	else
		image.draw_self(coord[0], size[0]);
	if (text == "" && desc != "")
		this->draw_funct(desc, desc_size, coord[2], text_color);
	if (text != "")
		this->draw_funct(front + text + back, text_size, coord[2], text_color);
}
