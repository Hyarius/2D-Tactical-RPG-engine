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

s_shop_map::s_shop_map(t_button *p_button, struct s_shop_item **p_item, t_vect p_icons_coord, t_vect p_icons_size, t_vect p_text_coord)
{
	button = p_button;
	item = p_item;
	icon_coord = p_icons_coord;
	icon_size = p_icons_size;
	text_coord = p_text_coord;
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
		if (text[0] != (*item)->to_add)
		{
			t_game_board tmp = t_game_board(MAP_PATH + (*item)->to_add + MAP_EXT);
			text[0] = (*item)->to_add;
			text[1] = "Map difficulty : " + to_string(tmp.map_difficulty);
			text[2] = "Exp reward : " + to_string(tmp.exp_reward);
			text[3] = "Gold reward : " + to_string(tmp.gold_reward);
			text[4] = "Price : " + to_string((*item)->price) + " gold";
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
		for (size_t i = 0; i < 5; i++)
		{
			text_image[i]->draw_self(tmp2, image_size[i]);
			tmp2.y += image_size[i].y * 1.1;
		}
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