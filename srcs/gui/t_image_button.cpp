#include "gui.h"

s_image_button::s_image_button(string p_text, int p_text_color,
					t_image p_image, t_vect p_coord, t_vect p_size, int border)
{
	text_image = NULL;
	text = p_text;
	text_color = p_text_color;
	coord[0] = p_coord;
	size[0] = p_size;
	coord[1] = p_coord + border;
	size[1] = p_size - (border * 2);
	coord[2] = coord[1] + size[1] / 2;
	if (text != "")
	{
		text_size = calc_text_size(text, size[1]);
		surface = TTF_RenderText_Blended(get_font(text_size), text.c_str(), get_color(text_color));
		if (surface == NULL)
			error_exit("Can't create the text for a t_image button with the string " + text + " at size " + to_string(text_size));
		saved_text = text;
		text_image = new t_image(surface);
		image_coord = t_vect(coord[2].x - text_image->surface->w / 2, coord[2].y - text_image->surface->h / 2);
		image_size = t_vect(text_image->surface->w, text_image->surface->h);
		SDL_FreeSurface(surface);
	}
	else
	{
		text_size = -1;
		text_image = NULL;
	}
	image = p_image;
	funct_left = NULL;
	data_left = NULL;
	funct_right = NULL;
	data_right = NULL;
	draw_funct = draw_centred_text;
}

s_image_button::s_image_button(t_image p_image, t_vect p_coord, t_vect p_size)
{
	text_image = NULL;
	text = "";
	text_size = -1;
	coord[0] = p_coord;
	size[0] = p_size;
	image = p_image;
	funct_left = NULL;
	data_left = NULL;
	funct_right = NULL;
	data_right = NULL;
}

void		s_image_button::draw_self()
{
	image.draw_self(coord[0], size[0]);
	if (saved_text != text)
	{
		if (text != "")
		{
				text_size = calc_text_size(text, size[1]);
				surface = TTF_RenderText_Blended(get_font(text_size), text.c_str(), get_color(text_color));
				if (surface == NULL)
					error_exit("Can't create the text for a t_image button with the string " + text + " at size " + to_string(text_size));
				saved_text = text;
				text_image = new t_image(surface);
				image_coord = t_vect(coord[2].x - text_image->surface->w / 2, coord[2].y - text_image->surface->h / 2);
				image_size = t_vect(text_image->surface->w, text_image->surface->h);
				SDL_FreeSurface(surface);
		}
		else
			text_image = NULL;
	}
	if (text_image != NULL)
		text_image->draw_self(image_coord, image_size);
}
