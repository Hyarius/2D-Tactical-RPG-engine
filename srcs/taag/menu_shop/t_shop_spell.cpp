#include "taag.h"

s_shop_spell::s_shop_spell()
{
	button = NULL;
	item = NULL;
}
/*
vector<string>	text;
vector<string>	saved_text;
vector<t_image *>
				text_image;
vector<t_vect>	image_coord;
vector<t_vect>	image_size;
*/

s_shop_spell::s_shop_spell(s_spell_card *p_button, struct s_shop_item **p_item, t_vect p_delta, t_vect p_size)
{
	button = p_button;
	if (button != NULL)
		funct = button->button->button->funct_left;
	item = p_item;
	delta = p_delta;
	size = p_size;
	text = "";
}

void			s_shop_spell::draw_self()
{
	if (*item != NULL)
		*(button->spell) = &(spell_map[(*item)->to_add]);
	if (button != NULL)
		button->draw_self();
	if (*item != NULL && button != NULL)
	{
		if ((*item)->owned == true)
		{
			button->button->button->funct_left = NULL;
			button->button->button->funct_right = NULL;
		}
		else
			button->button->button->funct_left = funct;
		if (text != to_string((*item)->price) + " gold")
		{
			text = to_string((*item)->price) + " gold";
			int text_size = calc_text_max_size(text, size - 15);
			SDL_Surface *surface = TTF_RenderText_Blended(get_font(text_size), text.c_str(), get_color(DARK_GREY));
			if (surface == NULL)
				error_exit();
			text_image = new t_image(surface);
			image_size = t_vect(surface->w, surface->h);
			image_coord = button->button->button->coord[0] + delta + size / 2;
			SDL_FreeSurface(surface);
		}
		if ((*item)->owned == true)
			draw_rectangle(button->button->button->coord[0], button->button->button->size[0], t_color(0.3, 0.3, 0.3, 0.6));
		draw_border_rectangle(button->button->button->coord[0] + delta, size, 4, t_color(0.8,0.65,0.0), t_color(189.0/255.0,183.0/255.0,107.0/255.0));
		text_image->draw_centred_self(image_coord, image_size);
	}
}

bool			s_shop_spell::click_left(t_vect mouse)
{
	size_t i = 0;

	if (button != NULL && button->click_left(mouse) == true)
		return (true);
	return (false);
}

bool			s_shop_spell::click_right(t_vect mouse)
{
	size_t i = 0;

	if (button != NULL && button->click_left(mouse) == true)
		return (true);
	return (false);
}

bool			s_shop_spell::key_press(SDL_Event *event)
{
	if (button != NULL && button->key_press(event) == true)
		return (true);
	return (false);
}
