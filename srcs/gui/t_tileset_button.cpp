#include "gui.h"

s_tileset_button::s_tileset_button(string p_text, int p_text_color,
							t_tileset *p_tile, t_vect p_selected,
							t_vect p_coord, t_vect p_size, int border)
{
	text = p_text;
	text_color = p_text_color;
	coord[0] = p_coord;
	size[0] = p_size;
	coord[1] = p_coord + border;
	size[1] = p_size - (border * 2);
	coord[2] = coord[1] + size[1] / 2;
	if (text != "")
		text_size = calc_text_size(text, size[1] - border * 2);
	else
		text_size = -1;
	tile = p_tile;
	selected = p_selected;
	anim = t_vect(1, 0);
	funct_left = NULL;
	data_left = NULL;
	funct_right = NULL;
	data_right = NULL;
	draw_funct = draw_centred_text;
}

s_tileset_button::s_tileset_button(t_tileset *p_tile, t_vect p_selected,
	t_vect p_coord, t_vect p_size, int border)
{
	text = "";
	text_size = -1;
	coord[0] = p_coord;
	size[0] = p_size;
	coord[1] = p_coord + border;
	size[1] = p_size - (border * 2);
	coord[2] = coord[1] + size[1] / 2;
	tile = p_tile;
	selected = p_selected;
	anim = t_vect(1, 0);
	funct_left = NULL;
	data_left = NULL;
	funct_right = NULL;
	data_right = NULL;
	draw_funct = draw_centred_text;
}

s_tileset_button::s_tileset_button(string p_text, int p_text_color,
	t_tileset *p_tile, t_vect p_selected, t_vect p_anim,
	t_vect p_coord, t_vect p_size, int border)
{
	text = p_text;
	text_color = p_text_color;
	coord[0] = p_coord;
	size[0] = p_size;
	coord[1] = p_coord + border;
	size[1] = p_size - (border * 2);
	coord[2] = coord[1] + size[1] / 2;
	if (text != "")
		text_size = calc_text_size(text, size[1] - border * 2);
	else
		text_size = -1;
	tile = p_tile;
	selected = p_selected;
	anim = p_anim;
	funct_left = NULL;
	data_left = NULL;
	funct_right = NULL;
	data_right = NULL;
	draw_funct = draw_centred_text;
}

s_tileset_button::s_tileset_button(t_tileset *p_tile, t_vect p_selected, t_vect p_anim,
	t_vect p_coord, t_vect p_size, int border)
{
	text = "";
	text_size = -1;
	coord[0] = p_coord;
	size[0] = p_size;
	coord[1] = p_coord + border;
	size[1] = p_size - (border * 2);
	coord[2] = coord[1] + size[1] / 2;
	tile = p_tile;
	selected = p_selected;
	anim = p_anim;
	funct_left = NULL;
	data_left = NULL;
	funct_right = NULL;
	data_right = NULL;
	draw_funct = draw_centred_text;
}

void			s_tileset_button::draw_self()
{
	draw_rectangle(coord[0], size[0], t_color(0.4, 0.4, 0.4));
	draw_rectangle(coord[1], size[1], t_color(0.6, 0.6, 0.6));
	if (tile != NULL && selected != t_vect(-1, -1))
	{
		int type = get_frame_state(anim.x);
		if (type == 3)
			type = 1;
		tile->draw_self(coord[1], size[1], selected + t_vect(type, 0));
	}
	if (text != "")
	{
		if (text_size == -1 || size[1].x > calc_text_len(text, text_size) || size[1].y > get_char(text_size, BLACK, 'M')->surface->h)
			text_size = calc_text_size(text, size[1] - (size[0] - size[1]));
		this->draw_funct(text, text_size, coord[2], text_color);
	}
}
