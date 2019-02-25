#include "taag.h"

/*
Z_CROSS = 0,
Z_CROSS_LINE = 1,
Z_LINE = 2,
Z_SQUARE = 3,
*/

vector<t_vect>	s_game_board::calc_diam(int size)
{
	vector<t_vect>	result;

	int i = -size;
	while (i <= size)
	{
		int j = -(size - abs(i));
		while (abs(i) + j <= size)
		{
			result.push_back(t_vect(i, j));
			j++;
		}
		i++;
	}

	return (result);
}

vector<t_vect>	s_game_board::calc_cross(int size)
{
	vector<t_vect>	result;

	int i = -size;
	while (i <= size)
	{
		result.push_back(t_vect(0, i));
		if (i != 0)
			result.push_back(t_vect(i, 0));
		i++;
	}

	return (result);
}

vector<t_vect>	s_game_board::calc_line(int size, t_vect dir)
{
	vector<t_vect>	result;

	int i = 0;
	while (i < size)
	{
		result.push_back(t_vect(i * dir.x, i * dir.y));
		i++;
	}

	return (result);
}

vector<t_vect>	s_game_board::calc_square(int size)
{
	vector<t_vect>	result;

	int i = -size;
	while (i <= size)
	{
		int j = -size;
		while (j <= size)
		{
			result.push_back(t_vect(i, j));
			j++;
		}
		i++;
	}

	return (result);
}

void				s_game_board::reset_vision(int min, int max)
{
	int x = 0;
	while (x < board_size.x)
	{
		int y = 0;
		while (y < board_size.y)
		{
			get_cell(x, y)->cursor = t_vect(0, 0);
			if (get_cell(x, y)->v_dist >= min && get_cell(x, y)->v_dist != 999 &&
				get_cell(x, y)->v_dist <= max)
				get_cell(x, y)->cursor = t_vect(0, 2);
			y++;
		}
		x++;
	}
}

void				s_game_board::calculate_zone()
{
	t_vect mouse = get_mouse_pos();
	t_actor *player = turn_order[turn_index % turn_order.size()];
	if (s_spell != -1)
		reset_vision(player->spell[s_spell]->range[0], player->spell[s_spell]->range[1]);
	if (get_cell(mouse) && get_cell(mouse)->cursor == t_vect(0, 2))
	{
		vector<t_vect>	result;
		if (player->spell[s_spell]->zone_type == Z_DIAM)
			result = calc_diam(player->spell[s_spell]->zone_size);
		else if (player->spell[s_spell]->zone_type == Z_CROSS)
			result = calc_cross(player->spell[s_spell]->zone_size);
		else if (player->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = player->coord - mouse;
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
								((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			result = calc_line(player->spell[s_spell]->zone_size, dir);
		}
		else if (player->spell[s_spell]->zone_type == Z_SQUARE)
			result = calc_square(player->spell[s_spell]->zone_size);
		size_t i = 0;
		while (i < result.size())
		{
			if (get_cell(mouse + result[i]))
				get_cell(mouse + result[i])->cursor = t_vect(2, 2);
			i++;
		}
	}
}

/*
Z_CROSS = 0,
Z_CROSS_LINE = 1,
Z_LINE = 2,
Z_SQUARE = 3,
*/

vector<t_vect>	s_game_board::calc_diam(int size)
{
	vector<t_vect>	result;

	int i = -size;
	while (i <= size)
	{
		int j = -(size - abs(i));
		while (abs(i) + j <= size)
		{
			result.push_back(t_vect(i, j));
			j++;
		}
		i++;
	}

	return (result);
}

vector<t_vect>	s_game_board::calc_cross(int size)
{
	vector<t_vect>	result;

	int i = -size;
	while (i <= size)
	{
		result.push_back(t_vect(0, i));
		if (i != 0)
			result.push_back(t_vect(i, 0));
		i++;
	}

	return (result);
}

vector<t_vect>	s_game_board::calc_line(int size, t_vect dir)
{
	vector<t_vect>	result;

	int i = 0;
	while (i < size)
	{
		result.push_back(t_vect(i * dir.x, i * dir.y));
		i++;
	}

	return (result);
}

vector<t_vect>	s_game_board::calc_square(int size)
{
	vector<t_vect>	result;

	int i = -size;
	while (i <= size)
	{
		int j = -size;
		while (j <= size)
		{
			result.push_back(t_vect(i, j));
			j++;
		}
		i++;
	}

	return (result);
}

void				s_game_board::reset_vision(int min, int max)
{
	int x = 0;
	while (x < board_size.x)
	{
		int y = 0;
		while (y < board_size.y)
		{
			get_cell(x, y)->cursor = t_vect(0, 0);
			if (get_cell(x, y)->v_dist >= min && get_cell(x, y)->v_dist != 999 &&
				get_cell(x, y)->v_dist <= max)
				get_cell(x, y)->cursor = t_vect(0, 2);
			y++;
		}
		x++;
	}
}

void				s_game_board::calculate_zone()
{
	t_vect mouse = get_mouse_pos();
	t_actor *player = turn_order[turn_index % turn_order.size()];
	if (s_spell != -1)
		reset_vision(player->spell[s_spell]->range[0], player->spell[s_spell]->range[1]);
	if (get_cell(mouse) && get_cell(mouse)->cursor == t_vect(0, 2))
	{
		vector<t_vect>	result;
		if (player->spell[s_spell]->zone_type == Z_DIAM)
			result = calc_diam(player->spell[s_spell]->zone_size);
		else if (player->spell[s_spell]->zone_type == Z_CROSS)
			result = calc_cross(player->spell[s_spell]->zone_size);
		else if (player->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = player->coord - mouse;
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
								((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			result = calc_line(player->spell[s_spell]->zone_size, dir);
		}
		else if (player->spell[s_spell]->zone_type == Z_SQUARE)
			result = calc_square(player->spell[s_spell]->zone_size);
		size_t i = 0;
		while (i < result.size())
		{
			if (get_cell(mouse + result[i]))
				get_cell(mouse + result[i])->cursor = t_vect(2, 2);
			i++;
		}
	}
}
