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
	item = p_item;
	delta = p_delta;
	size = p_size;
}

void			s_shop_spell::draw_self()
{
	if (*item != NULL)
		*(button->spell) = &(spell_map[(*item)->to_add]);
	button->draw_self();
	if (*item != NULL)
	{
		draw_border_rectangle(button->button->button->coord[0] + delta, size, 4, t_color(0.8,0.65,0.0), t_color(189.0/255.0,183.0/255.0,107.0/255.0));
		int text_size = calc_text_max_size(to_string((*item)->price) + " gold", size - 15);
		draw_centred_text(to_string((*item)->price) + " gold", text_size, NORMAL, 2, button->button->button->coord[0] + delta + size / 2, LIGHT_GREY, BLACK);
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
