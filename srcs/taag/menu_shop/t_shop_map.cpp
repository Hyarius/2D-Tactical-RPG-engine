#include "taag.h"

s_shop_map::s_shop_map()
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

s_shop_map::s_shop_map(t_button *p_button, struct s_shop_item **p_item, t_vect p_icon_coord, t_vect p_icon_size, t_vect p_text_coord, t_vect p_price_coord, t_vect p_price_size)
{
	button = p_button;
	if (button != NULL)
		funct = button->button->funct_left;
	item = p_item;
	icon_coord = p_icon_coord;
	icon_size = p_icon_size;
	text_coord = p_text_coord;
	price_coord = p_price_coord;
	price_size = p_price_size;
	text.resize(5);
	text_image.resize(5);
	image_size.resize(5);
	text_size = calc_text_max_size("mmmmmmmmmmmmmm", button->button->size[1]);
}

void			s_shop_map::draw_self()
{
	if (button != NULL)
		button->draw_self();
	if (*item != NULL && button != NULL)
	{
		if ((*item)->owned == true)
		{
			button->button->funct_left = NULL;
			button->button->funct_right = NULL;
		}
		else
			button->button->funct_left = funct;
		if (text[0] != (*item)->to_add)
		{
			t_game_board tmp = t_game_board(MAP_PATH + (*item)->to_add + MAP_EXT);
			text[0] = (*item)->to_add;
			text[1] = "Map difficulty : " + to_string(tmp.map_difficulty);
			text[2] = "Exp reward : " + to_string(tmp.exp_reward);
			text[3] = "Gold reward : " + to_string(tmp.gold_reward);
			text[4] = to_string((*item)->price) + " gold";
			for (size_t i = 0; i < 5; i++)
			{
				SDL_Surface *surface = TTF_RenderText_Blended(get_font(text_size), text[i].c_str(), get_color(DARK_GREY));
				if (surface == NULL)
					error_exit();
				text_image[i] = new t_image(surface);
				image_size[i] = t_vect(surface->w, surface->h);
				SDL_FreeSurface(surface);
			}
		}
		(*item)->tile->draw_self(icon_coord, icon_size, (*item)->sprite);
		t_vect tmp2 = t_vect(text_coord);
		for (size_t i = 0; i < 4; i++)
		{
			text_image[i]->draw_self(tmp2, image_size[i]);
			tmp2.y += image_size[i].y * 1.1;
		}
		if ((*item)->owned == true)
			draw_rectangle(button->button->coord[0], button->button->size[0], t_color(0.3, 0.3, 0.3, 0.6));
		draw_border_rectangle(price_coord, price_size, 4, t_color(0.8,0.65,0.0), t_color(189.0/255.0,183.0/255.0,107.0/255.0));
		text_image[4]->draw_centred_self(price_coord + price_size / 2, image_size[4]);
	}
}

bool			s_shop_map::click_left(t_vect mouse)
{
	size_t i = 0;

	if (button != NULL && button->click_left(mouse) == true)
		return (true);
	return (false);
}

bool			s_shop_map::click_right(t_vect mouse)
{
	size_t i = 0;

	if (button != NULL && button->click_left(mouse) == true)
		return (true);
	return (false);
}

bool			s_shop_map::key_press(SDL_Event *event)
{
	if (button != NULL && button->key_press(event) == true)
		return (true);
	return (false);
}
