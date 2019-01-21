#include "taag.h"

/*
Z_CROSS = 0,
Z_CROSS_LINE = 1,
Z_LINE = 2,
Z_SQUARE = 3,
*/

vector<t_vect>	s_game_engine::calc_cross(int size)
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

vector<t_vect>	s_game_engine::calc_cross_line(int size)
{
	vector<t_vect>	result;

	int i = -size;
	while (i <= size)
	{
		result.push_back(t_vect(0, i));
		result.push_back(t_vect(i, 0));
		i++;
	}
	
	return (result);
}

vector<t_vect>	s_game_engine::calc_line(int size, t_vect dir)
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

vector<t_vect>	s_game_engine::calc_square(int size)
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

void				s_game_engine::calculate_zone()
{
	t_vect mouse = board.get_mouse_pos();
	t_actor *player = turn_order[turn_index % turn_order.size()];
	if (board.get_cell(mouse) && board.get_cell(mouse)->cursor == t_vect(0, 2))
	{
		vector<t_vect>	result;
		if (player->spell[s_spell]->zone_type == Z_CROSS)
			result = calc_cross(player->spell[s_spell]->zone_size);
		if (player->spell[s_spell]->zone_type == Z_CROSS_LINE)
			result = calc_cross_line(player->spell[s_spell]->zone_size);
		if (player->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = player->coord - mouse;
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
								((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			result = calc_line(player->spell[s_spell]->zone_size, dir);
		}
		if (player->spell[s_spell]->zone_type == Z_SQUARE)
			result = calc_square(player->spell[s_spell]->zone_size);
		size_t i = 0;
		while (i < result.size())
		{
			if (board.get_cell(mouse + result[i]))
				board.get_cell(mouse + result[i])->cursor = t_vect(2, 2);
			i++;
		}
		calculated = false;
	}
}
