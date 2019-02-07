#include "taag.h"

				s_iterator::s_iterator()
{}

static void		increment_iterator(t_data data) //0 - &value / 1 - &pool / 2 - increment / 3 - cost / 4 - min / 5 - max
{
	int *value = (int *)(data.data[0]);
	int *pool = (int *)(data.data[1]);
	int	delta = (int &)(data.data[2]);
	int	cost = (int &)(data.data[3]);
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_LCTRL])
	{
		delta = delta * 10;
		cost = cost * 10;
	}
	int	min = (int &)(data.data[4]);
	int	max = (int &)(data.data[5]);

	if (value != NULL)
	{
		if (*value + delta <= max && ((pool != NULL && *pool - cost >= 0) || pool == NULL))
		{
			*value += delta;
			if (pool)
				*pool -= cost;
		}
	}
}

static void		decrement_iterator(t_data data) //0 - &value / 1 - &pool / 2 - increment / 3 - cost / 4 - min / 5 - max
{
	int *value = (int *)(data.data[0]);
	int *pool = (int *)(data.data[1]);
	int	delta = (int &)(data.data[2]);
	int	cost = (int &)(data.data[3]);
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_LCTRL])
	{
		delta = delta * 10;
		cost = cost * 10;
	}
	int	min = (int &)(data.data[4]);
	int	max = (int &)(data.data[5]);

	if (value != NULL)
	{
		if (*value - delta >= min)
		{
			*value -= delta;
			if (pool)
				*pool += cost;
		}
	}
}

				s_iterator::s_iterator(int base_value, int *pool, int delta, int pool_cost, int min, int max,
					t_button *p_desc, t_button *p_minus, t_button *p_contain, t_button *p_plus)
{
	value = base_value;
	linked_value = NULL;
	desc = p_desc;
	minus = p_minus;
	if (minus != NULL)
	{
		minus->button->funct = decrement_iterator;
		minus->button->data = t_data(6, &value, pool, delta, pool_cost, min, max); //0 - &value / 1 - &pool / 2 - increment / 3 - cost / 4 - min / 5 - max
	}
	container = p_contain;
	plus = p_plus;
	if (plus != NULL)
	{
		plus->button->funct = increment_iterator;
		plus->button->data = t_data(6, &value, pool, delta, pool_cost, min, max); //0 - &value / 1 - &pool / 2 - increment / 3 - cost
	}
}

				s_iterator::s_iterator(int *p_linked_value, int *pool, int delta, int pool_cost, int min, int max,
					t_button *p_desc, t_button *p_minus, t_button *p_contain, t_button *p_plus)
{
	linked_value = p_linked_value;
	value = *linked_value;
	desc = p_desc;
	minus = p_minus;
	if (minus != NULL)
	{
		minus->button->funct = decrement_iterator;
		minus->button->data = t_data(6, linked_value, pool, delta, pool_cost, min, max); //0 - linked_value / 1 - &pool / 2 - increment / 3 - cost / 4 - min / 5 - max
	}
	container = p_contain;
	plus = p_plus;
	if (plus != NULL)
	{
		plus->button->funct = increment_iterator;
		plus->button->data = t_data(6, linked_value, pool, delta, pool_cost, min, max); //0 - linked_value / 1 - &pool / 2 - increment / 3 - cost / 4 - min / 5 - max
	}
}

void			s_iterator::draw_self()
{
	if (desc != NULL)
		desc->draw_self();
	if (minus != NULL)
		minus->draw_self();
	if (container != NULL)
	{
		if (linked_value != NULL)
			value = *linked_value;
		container->button->text = to_string(value);
		container->draw_self();
	}
	if (plus != NULL)
		plus->draw_self();
}

bool			s_iterator::click(t_vect mouse)
{
	if (minus != NULL && minus->click(mouse) == true)
		return (true);
	if (plus != NULL && plus->click(mouse) == true)
		return (true);
	return (false);
}

bool			s_iterator::key_press(SDL_Event *event)
{
	(void)event;
	return (false);
}
