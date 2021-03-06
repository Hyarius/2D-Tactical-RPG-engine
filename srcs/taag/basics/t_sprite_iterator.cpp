#include "taag.h"

static void		increment_sprite_iterator(t_data data)//0 - &tileset / 1 - &selected / 2 - &delta / 3 - vector_name
{
	size_t *index = (size_t *)(data.data[0]);
	t_vect *selected = (t_vect *)(data.data[1]);
	t_vect delta = *((t_vect *)(data.data[2]));
	vector<string>	*vector_name = (vector<string> *)(data.data[3]);

	selected->x += delta.x;
	if (get_sprite_tile((*vector_name)[*index])->nb_sprite.x <= selected->x)
	{
		selected->x = 0;
		selected->y += delta.y;
	}
	if (get_sprite_tile((*vector_name)[*index])->nb_sprite.y <= selected->y)
	{
		selected->x = 0;
		selected->y = 0;
		(*index)++;
	}
	if (*index >= (*vector_name).size())
		*index = 0;
}

static void		decrement_sprite_iterator(t_data data)//0 - &tileset / 1 - &selected / 2 - &delta / 3 - vector_name
{
	size_t *index = (size_t *)(data.data[0]);
	t_vect *selected = (t_vect *)(data.data[1]);
	t_vect delta = *(t_vect *)(data.data[2]);
	vector<string>	*vector_name = (vector<string> *)(data.data[3]);

	selected->x -= delta.x;
	if (selected->x < 0)
	{
		selected->x = get_sprite_tile((*vector_name)[*index])->nb_sprite.x - delta.x;
		selected->y -= delta.y;
	}
	if (selected->y < 0)
	{
		selected->x = get_sprite_tile((*vector_name)[*index])->nb_sprite.x - delta.x;
		selected->y = get_sprite_tile((*vector_name)[*index])->nb_sprite.y - delta.y;
		if (*index == 0)
			*index = (*vector_name).size() - 1;
		else
			(*index)--;
	}
}

s_sprite_iterator::s_sprite_iterator()
{
	i = 0;
}

s_sprite_iterator::s_sprite_iterator(t_vect p_delta, vector<string> *p_vector_name, t_button *p_desc, t_button *p_minus, t_button *p_contain, t_button *p_plus)
{
	i = 0;
	delta = p_delta;
	vector_name = p_vector_name;
	desc = p_desc;
	minus = p_minus;
	container = p_contain;
	if (container != NULL)
	{
		for (i = 0; i != vector_name->size(); ++i)
		{
			if (get_sprite_tile((*vector_name)[i]) == ((t_tileset_button *)(container->button))->tile)
				break;
		}
		container->button->funct_left = menu_choose_sprite;
		container->button->data_left = t_data(2, &i, &((t_tileset_button *)(p_contain->button))->selected); //0 - &index / 1 - &selected / 2 - &delta
	}
	if (minus != NULL && container != NULL)
	{
		minus->button->funct_left = decrement_sprite_iterator;
		minus->button->data_left = t_data(4, &i, &((t_tileset_button *)(p_contain->button))->selected, &delta, vector_name); //0 - &index / 1 - &selected / 2 - &delta
	}
	plus = p_plus;
	if (plus != NULL && container != NULL)
	{
		plus->button->funct_left = increment_sprite_iterator;
		plus->button->data_left = t_data(4, &i, &((t_tileset_button *)(p_contain->button))->selected, &delta, vector_name); //0 - &index / 1 - &selected / 2 - &delta
	}
}

void			s_sprite_iterator::draw_self()
{
	if (desc != NULL)
		desc->draw_self();
	if (minus != NULL)
		minus->draw_self();
	if (container != NULL)
	{
		if (get_sprite_tile((*vector_name)[i]) != ((t_tileset_button *)(container->button))->tile)
			((t_tileset_button *)(container->button))->tile = get_sprite_tile((*vector_name)[i]);
		container->draw_self();
	}
	if (plus != NULL)
		plus->draw_self();
}

bool			s_sprite_iterator::click_left(t_vect mouse)
{
	if (minus != NULL && minus->click_left(mouse) == true)
		return (true);
	if (container != NULL && container->click_left(mouse) == true)
		return (true);
	if (plus != NULL && plus->click_left(mouse) == true)
		return (true);
	return (false);
}

bool			s_sprite_iterator::click_right(t_vect mouse)
{
	if (minus != NULL && minus->click_right(mouse) == true)
		return (true);
	if (container != NULL && container->click_right(mouse) == true)
		return (true);
	if (plus != NULL && plus->click_right(mouse) == true)
		return (true);
	return (false);
}

bool			s_sprite_iterator::key_press(SDL_Event *event)
{
	(void)event;
	return (false);
}
