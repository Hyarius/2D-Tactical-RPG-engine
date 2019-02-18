#include "taag.h"

static vector<t_vect>	AI_calc_diam(int size_min, int size_max)
{
	vector<t_vect>	result;

	int i = -size_max;
	while (i <= size_max)
	{
		int j = -(size_max - abs(i));
		while (abs(i) + j <= size_max)
		{
			if (abs(i) + abs(j) >= size_min)
				result.push_back(t_vect(i, j));
			j++;
		}
		i++;
	}

	return (result);
}

static vector<t_vect>	AI_calc_cross(int size_min, int size_max)
{
	vector<t_vect>	result;

	int i = -size_max;
	while (i <= size_max)
	{
		if (abs(i) >= size_min)
		{
			result.push_back(t_vect(0, i));
			if (i != 0)
				result.push_back(t_vect(i, 0));
		}
		i++;
	}

	return (result);
}

bool				s_game_engine::get_close_enemy(t_ai_helper data)
{
	if (turn_order[turn_index % turn_order.size()]->stat.pm.value == 0)
		return (false);
	if (data.value.size() < 4)
		error_exit("not enought arg into get_close_enemy for enemy " + turn_order[turn_index % turn_order.size()]->name, 2567);
	if (data.value.size() > 4)
		error_exit("too much arg into get_close_enemy for enemy " + turn_order[turn_index % turn_order.size()]->name, 2567);
	int old_pm = turn_order[turn_index % turn_order.size()]->stat.pm.value;
	int distance = data.value[1];
	int delta = data.value[2];
	int type = data.value[3];
	calculate_distance();
	vector<t_vect>	range;
	vector<t_vect> 	to_calc;
	size_t i = 0, j;
	while (i < turn_order.size())
	{
		if (turn_order[i]->team != turn_order[turn_index % turn_order.size()]->team)
			to_calc.push_back(turn_order[i]->coord);
		i++;
	}
	if (type == R_CIRCLE)
		range = AI_calc_diam(distance - delta, distance);
	if (type == R_LINE)
		range = AI_calc_cross(distance - delta, distance);
	t_vect result = t_vect(-1, -1);
	j = 999;
	i = 0;
	while (i < to_calc.size())
	{
		size_t count = 0;
		while (count < range.size())
		{
			if (board.get_cell(to_calc[i] + range[count]) && board.get_cell(to_calc[i] + range[count])->node->m_obs == false && board.get_cell(to_calc[i] + range[count])->m_dist != 999)
			{
				if (board.get_cell(to_calc[i] + range[count])->actor == NULL || board.get_cell(to_calc[i] + range[count])->actor == turn_order[turn_index % turn_order.size()] || board.get_cell(to_calc[i] + range[count])->actor->team != turn_order[turn_index % turn_order.size()]->team)
				{
					board.get_cell(to_calc[i] + range[count])->cursor = t_vect(2, 2);
					vector<t_vect> path = calc_path(to_calc[i] + range[count]);
					if (path.size() < j)
					{
						j = path.size();
						result = to_calc[i] + range[count];
					}
				}
			}
			count++;
		}
		i++;
	}
	if (board.get_cell(turn_order[turn_index % turn_order.size()]->coord)->cursor != t_vect(2, 2) && result != t_vect(-1, -1))
	{
		move_actor(result);
		if (turn_order[turn_index % turn_order.size()]->stat.pm.value == old_pm)
			return (false);
		return (true);
	}
	else
		return (false);
}

bool				s_game_engine::get_close_ally(t_ai_helper data)
{
	if (turn_order[turn_index % turn_order.size()]->stat.pm.value == 0)
		return (false);
	if (data.value.size() < 4)
		error_exit("not enought arg into get_close_ally for enemy " + turn_order[turn_index % turn_order.size()]->name, 2567);
	if (data.value.size() > 4)
		error_exit("too much arg into get_close_ally for enemy " + turn_order[turn_index % turn_order.size()]->name, 2567);
	int old_pm = turn_order[turn_index % turn_order.size()]->stat.pm.value;
	int distance = data.value[1];
	int delta = data.value[2];
	int type = data.value[3];
	calculate_distance();
	vector<t_vect>	range;
	vector<t_vect> 	to_calc;
	size_t i = 0, j;
	while (i < turn_order.size())
	{
		if (turn_order[i] != turn_order[turn_index % turn_order.size()] &&
			turn_order[i]->team == turn_order[turn_index % turn_order.size()]->team)
			to_calc.push_back(turn_order[i]->coord);
		i++;
	}
	if (type == R_CIRCLE)
		range = AI_calc_diam(distance - 1, distance);
	if (type == R_LINE)
		range = AI_calc_cross(distance - 1, distance);
	t_vect result = t_vect(-1, -1);
	j = 999;
	i = 0;
	while (i < to_calc.size())
	{
		size_t count = 0;
		while (count < range.size())
		{
			if (board.get_cell(to_calc[i] + range[count]) && board.get_cell(to_calc[i] + range[count])->node->m_obs == false)
			{
				if (board.get_cell(to_calc[i] + range[count])->actor == NULL || board.get_cell(to_calc[i] + range[count])->actor == turn_order[turn_index % turn_order.size()] || board.get_cell(to_calc[i] + range[count])->actor->team == turn_order[turn_index % turn_order.size()]->team)
				{
					board.get_cell(to_calc[i] + range[count])->cursor = t_vect(2, 2);
					vector<t_vect> path = calc_path(to_calc[i] + range[count]);
					if (path.size() < j)
					{
						j = path.size();
						result = to_calc[i] + range[count];
					}
				}
			}
			count++;
		}
		i++;
	}
	if (board.get_cell(turn_order[turn_index % turn_order.size()]->coord)->cursor != t_vect(2, 2) && result != t_vect(-1, -1))
	{
		move_actor(result);
		if (turn_order[turn_index % turn_order.size()]->stat.pm.value == old_pm)
			return (false);
		return (true);
	}
	else
		return (false);
}

bool				s_game_engine::flee_enemy(t_ai_helper data)
{
	if (turn_order[turn_index % turn_order.size()]->stat.pm.value == 0)
		return (false);
	if (turn_order[turn_index % turn_order.size()]->stat.hp.percent() > data.value[1])
		return (false);
	int old_pm = turn_order[turn_index % turn_order.size()]->stat.pm.value;
	vector<vector<int>> tab;
	vector<t_vect>	to_calc;
	tab.resize(board.board_size.x);
	int i = 0, j;
	while (i < board.board_size.x)
	{
		tab[i].resize(board.board_size.y);
		j = 0;
		while (j < board.board_size.y)
		{
			if (board.get_cell(i, j) && (board.get_cell(i, j)->actor == NULL || board.get_cell(i, j)->actor->team == turn_order[turn_index % turn_order.size()]->team))
				tab[i][j] = 0;
			else
				tab[i][j] = board.board_size.x + board.board_size.y + 1;
			j++;
		}
		i++;
	}
	int min = 999;
	i = 0;
	int find;
	while (i < board.board_size.x)
	{
		j = 0;
		while (j < board.board_size.y)
		{
			find = 0;
			if (tab[i][j] != 0)
			{
				if (board.get_cell(i + 1, j) && (tab[i + 1][j] == 0 || tab[i + 1][j] < tab[i][j] - 1) && board.get_cell(i + 1, j)->node->m_obs == false && (find = 1))
					tab[i + 1][j] = tab[i][j] - 1;
				if (board.get_cell(i - 1, j) && (tab[i - 1][j] == 0 || tab[i - 1][j] < tab[i][j] - 1) && board.get_cell(i - 1, j)->node->m_obs == false && (find = 1))
					tab[i - 1][j] = tab[i][j] - 1;
				if (board.get_cell(i, j + 1) && (tab[i][j + 1] == 0 || tab[i][j + 1] < tab[i][j] - 1) && board.get_cell(i, j + 1)->node->m_obs == false && (find = 1))
					tab[i][j + 1] = tab[i][j] - 1;
				if (board.get_cell(i, j - 1) && (tab[i][j - 1] == 0 || tab[i][j - 1] < tab[i][j] - 1) && board.get_cell(i, j - 1)->node->m_obs == false && (find = 1))
					tab[i][j - 1] = tab[i][j] - 1;
			}
			if (min > tab[i][j])
				min = tab[i][j];
			if (find == 1)
			{
				i = 0;
				j = 0;
			}
			else
				j++;
		}
		i++;
	}
	t_vect			actual = turn_order[turn_index % turn_order.size()]->coord;
	t_vect			to_look;
	find = 1;
	while (tab[actual.x][actual.y] != min && find == 1)
	{
		find = 0;
		to_look = actual;
		if (board.get_cell(actual + t_vect(1, 0)) && tab[to_look.x][to_look.y] > tab[actual.x + 1][actual.y] && tab[actual.x + 1][actual.y] != 0)
		{
			to_look = t_vect(actual.x + 1, actual.y);
			find = 1;
		}
		if (board.get_cell(actual + t_vect(-1, 0)) && tab[to_look.x][to_look.y] > tab[actual.x - 1][actual.y] && tab[actual.x - 1][actual.y] != 0)
		{
			to_look = t_vect(actual.x - 1, actual.y);
			find = 1;
		}
		if (board.get_cell(actual + t_vect(0, 1)) && tab[to_look.x][to_look.y] > tab[actual.x][actual.y + 1] && tab[actual.x][actual.y + 1] != 0)
		{
			to_look = t_vect(actual.x, actual.y + 1);
			find = 1;
		}
		if (board.get_cell(actual + t_vect(0, -1)) && tab[to_look.x][to_look.y] > tab[actual.x][actual.y - 1] && tab[actual.x][actual.y - 1] != 0)
		{
			to_look = t_vect(actual.x, actual.y - 1);
			find = 1;
		}
		actual = to_look;
	}
	calculate_distance();
	if (board.get_cell(turn_order[turn_index % turn_order.size()]->coord)->cursor != t_vect(2, 2) && actual != t_vect(-1, -1))
	{
		move_actor(actual);
		if (turn_order[turn_index % turn_order.size()]->stat.pm.value == old_pm)
			return (false);
		return (true);
	}
	else
		return (false);
}
