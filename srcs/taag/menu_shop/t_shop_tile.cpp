#include "taag.h"

s_shop_tile::s_shop_tile()
{
	button = NULL;
	item = NULL;
}
/*
t_button		*button;
struct s_shop_item
				**item;
int				text_size;
t_vect			icon_coord;
t_vect			icon_size;
t_vect			price_coord;
t_vect			price_size;
string			text[2]; //0 - name / 1 - price
t_image			*text_image[2];
t_vect			image_coord[2];
t_vect			image_size[2];
*/

s_shop_tile::s_shop_tile(t_button *p_button, struct s_shop_item **p_item, t_vect p_icon_coord, t_vect p_icon_size, t_vect p_price_coord, t_vect p_price_size, t_vect p_text_coord, t_vect p_text_back)
{
	button = p_button;
	if (button != NULL)
		funct = button->button->funct_left;
	item = p_item;
	text_size = calc_text_max_size("mmmmmmmmmmmmmm", button->button->size[1]);
	icon_coord = p_icon_coord;
	icon_size = p_icon_size;
	price_coord = p_price_coord;
	price_size = p_price_size;
	text_coord = p_text_coord;
	text_back = p_text_back;
	text[0] = "";
	text[1] = "";
}

void			s_shop_tile::draw_self()
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
			text[0] = (*item)->to_add;
			text[1] = to_string((*item)->price) + " gold";
			for (size_t i = 0; i < 2; i++)
			{
				SDL_Surface *surface = TTF_RenderText_Blended(get_font(text_size), text[i].c_str(), get_color(DARK_GREY));
				if (surface == NULL)
					error_exit();
				text_image[i] = new t_image(surface);
				image_size[i] = t_vect(surface->w, surface->h);
				SDL_FreeSurface(surface);
			}
		}
		draw_border_rectangle(icon_coord, icon_size, 4, t_color(0.4,0.4,0.4), t_color(0.6, 0.6, 0.6));
		int type = get_frame_state(4);
		if (type == 3)
			type = 1;
		(*item)->tile->draw_self(icon_coord, icon_size, (*item)->sprite + type);
		draw_border_rectangle(text_coord - text_back / 2, text_back, 4, t_color(0.4,0.4,0.4), t_color(0.6, 0.6, 0.6));
		text_image[0]->draw_centred_self(text_coord, image_size[0]);
		if ((*item)->owned == true)
			draw_rectangle(button->button->coord[0], button->button->size[0], t_color(0.3, 0.3, 0.3, 0.6));
		draw_border_rectangle(price_coord, price_size, 4, t_color(0.8,0.65,0.0), t_color(189.0/255.0,183.0/255.0,107.0/255.0));
		text_image[1]->draw_centred_self(price_coord + price_size / 2, image_size[1]);
	}
}

bool			s_shop_tile::click_left(t_vect mouse)
{
	size_t i = 0;

	if (button != NULL && button->click_left(mouse) == true)
		return (true);
	return (false);
}

bool			s_shop_tile::click_right(t_vect mouse)
{
	size_t i = 0;

	if (button != NULL && button->click_left(mouse) == true)
		return (true);
	return (false);
}

bool			s_shop_tile::key_press(SDL_Event *event)
{
	if (button != NULL && button->key_press(event) == true)
		return (true);
	return (false);
}
