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

bool				s_game_engine::get_close_enemy_percent(t_ai_helper data)
{
	if (turn_order[turn_index % turn_order.size()]->stat.pm.value == 0)
		return (false);
	int old_pm = turn_order[turn_index % turn_order.size()]->stat.pm.value;
	int distance = data.value[1];
	int delta = data.value[2];
	int type = data.value[3];
	calculate_distance();
	vector<t_vect> 	to_calc;
	size_t i = 0, j;
	t_actor *saved = NULL;
	while (i < turn_order.size())
	{
		if (turn_order[i]->team != turn_order[turn_index % turn_order.size()]->team && (saved == NULL || saved->stat.hp.percent() > turn_order[i]->stat.hp.percent()))
			saved = turn_order[i];
		i++;
	}
	if (saved == NULL)
		return (false);
	to_calc.push_back(saved->coord);
	vector<t_vect>	range;
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

	vector<t_vect> path = calc_path(result);
	if (data.value[4] != -1 && (int)(path.size()) >= data.value[4] + 1)
		result = path[data.value[4]];
	if (board.get_cell(turn_order[turn_index % turn_order.size()]->coord)->cursor != t_vect(2, 2) && result != t_vect(-1, -1))
	{
		move_actor(result);
		if (turn_order[turn_index % turn_order.size()]->stat.pm.value == old_pm)
			return (false);
		printf("%s move to the enemy with less HP %\n", turn_order[turn_index % turn_order.size()]->name.c_str());
		return (true);
	}
	else
		return (false);
}

bool				s_game_engine::get_close_enemy_weak(t_ai_helper data)
{
	if (turn_order[turn_index % turn_order.size()]->stat.pm.value == 0)
		return (false);
	int old_pm = turn_order[turn_index % turn_order.size()]->stat.pm.value;
	int distance = data.value[1];
	int delta = data.value[2];
	int type = data.value[3];
	calculate_distance();
	vector<t_vect>	range;
	vector<t_vect> 	to_calc;
	size_t i = 0, j;
	t_actor *saved = NULL;
	while (i < turn_order.size())
	{
		if (turn_order[i]->team != turn_order[turn_index % turn_order.size()]->team && (saved == NULL || saved->stat.hp.value > turn_order[i]->stat.hp.value))
			saved = turn_order[i];
		i++;
	}
	if (saved == NULL)
		return (false);
	to_calc.push_back(saved->coord);
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

	vector<t_vect> path = calc_path(result);
	if (data.value[4] != -1 && (int)(path.size()) >= data.value[4] + 1)
		result = path[data.value[4]];
	if (board.get_cell(turn_order[turn_index % turn_order.size()]->coord)->cursor != t_vect(2, 2) && result != t_vect(-1, -1))
	{
		move_actor(result);
		if (turn_order[turn_index % turn_order.size()]->stat.pm.value == old_pm)
			return (false);
		printf("%s move to the enemy with less HP value\n", turn_order[turn_index % turn_order.size()]->name.c_str());
		return (true);
	}
	else
		return (false);
}

bool				s_game_engine::get_close_enemy(t_ai_helper data)
{
	if (turn_order[turn_index % turn_order.size()]->stat.pm.value == 0)
		return (false);
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

	vector<t_vect> path = calc_path(result);
	if (data.value[4] != -1 && (int)(path.size()) >= data.value[4] + 1)
		result = path[data.value[4]];
	if (board.get_cell(turn_order[turn_index % turn_order.size()]->coord)->cursor != t_vect(2, 2) && result != t_vect(-1, -1))
	{
		move_actor(result);
		if (turn_order[turn_index % turn_order.size()]->stat.pm.value == old_pm)
			return (false);
		printf("%s move to the closest enemy\n", turn_order[turn_index % turn_order.size()]->name.c_str());
		return (true);
	}
	else
		return (false);
}

bool				s_game_engine::get_close_ally(t_ai_helper data)
{
	if (turn_order[turn_index % turn_order.size()]->stat.pm.value == 0)
		return (false);
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

	vector<t_vect> path = calc_path(result);
	if (data.value[4] != -1 && (int)(path.size()) >= data.value[4] + 1)
		result = path[data.value[4]];
	if (board.get_cell(turn_order[turn_index % turn_order.size()]->coord)->cursor != t_vect(2, 2) && result != t_vect(-1, -1))
	{
		move_actor(result);
		if (turn_order[turn_index % turn_order.size()]->stat.pm.value == old_pm)
			return (false);
		printf("%s move to the closest ally\n", turn_order[turn_index % turn_order.size()]->name.c_str());
		return (true);
	}
	else
		return (false);
}

bool				s_game_engine::get_close_ally_percent(t_ai_helper data)
{
	if (turn_order[turn_index % turn_order.size()]->stat.pm.value == 0)
		return (false);
	int old_pm = turn_order[turn_index % turn_order.size()]->stat.pm.value;
	int distance = data.value[1];
	int delta = data.value[2];
	int type = data.value[3];
	calculate_distance();
	vector<t_vect>	range;
	vector<t_vect> 	to_calc;
	size_t i = 0, j;
	t_actor *saved = NULL;
	while (i < turn_order.size())
	{
		if (turn_order[i] != turn_order[turn_index % turn_order.size()] &&
			turn_order[i]->team == turn_order[turn_index % turn_order.size()]->team &&
			(saved == NULL || saved->stat.hp.percent() > turn_order[i]->stat.hp.percent()))
			saved = turn_order[i];
		i++;
	}
	if (saved == NULL)
		return (false);
	to_calc.push_back(saved->coord);
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

	vector<t_vect> path = calc_path(result);
	if (data.value[4] != -1 && (int)(path.size()) >= data.value[4] + 1)
		result = path[data.value[4]];
	if (board.get_cell(turn_order[turn_index % turn_order.size()]->coord)->cursor != t_vect(2, 2) && result != t_vect(-1, -1))
	{
		move_actor(result);
		if (turn_order[turn_index % turn_order.size()]->stat.pm.value == old_pm)
			return (false);
		printf("%s move to the ally with less HP %\n", turn_order[turn_index % turn_order.size()]->name.c_str());
		return (true);
	}
	else
		return (false);
}

bool				s_game_engine::get_close_ally_damaged(t_ai_helper data)
{
	if (turn_order[turn_index % turn_order.size()]->stat.pm.value == 0)
		return (false);
	int old_pm = turn_order[turn_index % turn_order.size()]->stat.pm.value;
	int distance = data.value[2];
	int delta = data.value[3];
	int type = data.value[4];
	calculate_distance();
	vector<t_vect>	range;
	vector<t_vect> 	to_calc;
	size_t i = 0, j;
	t_actor *saved = NULL;
	while (i < turn_order.size())
	{
		if (turn_order[i] != turn_order[turn_index % turn_order.size()] &&
			turn_order[i]->team == turn_order[turn_index % turn_order.size()]->team &&
			(saved == NULL || (saved->stat.hp.percent() > turn_order[i]->stat.hp.percent() && turn_order[i]->stat.hp.percent() < data.value[1])))
			saved = turn_order[i];
		i++;
	}
	if (saved == NULL)
		return (false);
	to_calc.push_back(saved->coord);
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

	vector<t_vect> path = calc_path(result);
	if (data.value[5] != -1 && (int)(path.size()) >= data.value[5] + 1)
		result = path[data.value[5]];
	if (board.get_cell(turn_order[turn_index % turn_order.size()]->coord)->cursor != t_vect(2, 2) && result != t_vect(-1, -1))
	{
		move_actor(result);
		if (turn_order[turn_index % turn_order.size()]->stat.pm.value == old_pm)
			return (false);
		printf("%s move to the ally with HP % < %d\n", turn_order[turn_index % turn_order.size()]->name.c_str(), data.value[1]);
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
		printf("%s flee\n", turn_order[turn_index % turn_order.size()]->name.c_str());
		return (true);
	}
	else
		return (false);
}

bool				s_game_engine::attack(t_ai_helper data)
{
	s_spell = data.value[1];
	t_actor *actor = turn_order[turn_index % turn_order.size()];
	if (actor->stat.pa.value < actor->spell[s_spell]->cost_pa ||
		actor->stat.pm.value < actor->spell[s_spell]->cost_pm ||
		(data.value[2] != -1 && actor->stat.pa.value != data.value[2]) ||
		(data.value[3] != -1 && actor->stat.pm.value != data.value[3]))
		return (false);
	board.reset_board();
	calculated = false;
	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		calculate_vision_circle();
	else
		calculate_vision_line();
	size_t i = 0;

	vector<t_vect>	target_list; //every tile hited by the spell
	if (actor->spell[s_spell]->zone_type == Z_DIAM)
		target_list = calc_diam(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_CROSS)
		target_list = calc_cross(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_SQUARE)
		target_list = calc_square(actor->spell[s_spell]->zone_size);


	vector<t_vect> to_look; //every tile to look

	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		to_look = AI_calc_diam(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);
	if (actor->spell[s_spell]->range_type == R_LINE)
		to_look = AI_calc_cross(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);

	t_vect target = t_vect(-1, -1); //the saved coord of the spell
	int		utils_value = 999;

	for (size_t i = 0; i < to_look.size(); i++)
	{
		int nb_target_hit = 0;
		int percent_target_hit = 100;
		if (actor->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = actor->coord - to_look[i];
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
				((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			target_list = calc_line(actor->spell[s_spell]->zone_size, dir);
		}
		for (size_t j = 0; j < target_list.size(); j++)
		{
			t_vect tmp = actor->coord + to_look[i] + target_list[j];
			if (board.get_cell(tmp) && board.get_cell(actor->coord + to_look[i]) && board.get_cell(actor->coord + to_look[i])->cursor == t_vect(0, 2) && board.get_cell(tmp)->actor != NULL)
			{
				if (board.get_cell(tmp)->actor->team != actor->team)
				{
					if (percent_target_hit >= board.get_cell(tmp)->actor->stat.hp.percent())
						percent_target_hit = board.get_cell(tmp)->actor->stat.hp.percent();
					nb_target_hit++;
				}
			}
		}
		if (nb_target_hit != 0 && percent_target_hit / nb_target_hit < utils_value)
		{
			utils_value = percent_target_hit / nb_target_hit;
			target = actor->coord + to_look[i];
		}
	}
	if (target == t_vect(-1, -1))
		return (false);
	if (cast_spell(target) == false)
		return (false);
	board.reset_board();
	return (true);
}

bool				s_game_engine::attack_weak(t_ai_helper data)
{
	s_spell = data.value[1];
	t_actor *actor = turn_order[turn_index % turn_order.size()];
	if (actor->stat.pa.value < actor->spell[s_spell]->cost_pa ||
		actor->stat.pm.value < actor->spell[s_spell]->cost_pm ||
		(data.value[2] != -1 && actor->stat.pa.value != data.value[2]) ||
		(data.value[3] != -1 && actor->stat.pm.value != data.value[3]))
		return (false);
	board.reset_board();
	calculated = false;
	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		calculate_vision_circle();
	else
		calculate_vision_line();
	size_t i = 0;
	t_actor *target_actor = NULL;
	while (i < turn_order.size())
	{
		if ((target_actor == NULL || (board.get_cell(turn_order[i]->coord)->v_dist < board.get_cell(target_actor->coord)->v_dist && turn_order[i]->stat.hp.value < target_actor->stat.hp.value)) &&
			turn_order[i]->team != turn_order[turn_index % turn_order.size()]->team && board.get_cell(turn_order[i]->coord)->cursor == t_vect(0, 2))
			target_actor = turn_order[i];
		i++;
	}
	if (target_actor == NULL)
		return (false);
	i = 0;

	vector<t_vect>	target_list; //every tile hited by the spell
	if (actor->spell[s_spell]->zone_type == Z_DIAM)
		target_list = calc_diam(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_CROSS)
		target_list = calc_cross(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_SQUARE)
		target_list = calc_square(actor->spell[s_spell]->zone_size);


	vector<t_vect> to_look; //every tile to look

	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		to_look = AI_calc_diam(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);
	if (actor->spell[s_spell]->range_type == R_LINE)
		to_look = AI_calc_cross(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);

	t_vect target = t_vect(-1, -1); //the saved coord of the spell
	int		utils_value = 999;

	for (size_t i = 0; i < to_look.size(); i++)
	{
		int find = 0;
		int nb_target_hit = 0;
		int percent_target_hit = 100;
		if (actor->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = actor->coord - to_look[i];
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
				((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			target_list = calc_line(actor->spell[s_spell]->zone_size, dir);
		}
		for (size_t j = 0; j < target_list.size(); j++)
		{
			t_vect tmp = actor->coord + to_look[i] + target_list[j];
			if (board.get_cell(tmp) && board.get_cell(actor->coord + to_look[i]) && board.get_cell(actor->coord + to_look[i])->cursor == t_vect(0, 2) && board.get_cell(tmp)->actor != NULL)
			{
				if (board.get_cell(tmp)->actor == target_actor)
					find = 1;
				if (board.get_cell(tmp)->actor->team != actor->team)
				{
					if (percent_target_hit >= board.get_cell(tmp)->actor->stat.hp.value)
						percent_target_hit = board.get_cell(tmp)->actor->stat.hp.value;
					nb_target_hit++;
				}
			}
		}
		if (nb_target_hit != 0 && percent_target_hit / nb_target_hit < utils_value && find == 1)
		{
			utils_value = percent_target_hit / nb_target_hit;
			target = actor->coord + to_look[i];
		}
	}
	if (target == t_vect(-1, -1))
		return (false);
	if (cast_spell(target) == false)
		return (false);
	board.reset_board();
	printf("%s atttack the enemy with the lower HP value\n", turn_order[turn_index % turn_order.size()]->name.c_str());
	return (true);
}

bool				s_game_engine::attack_percent(t_ai_helper data)
{
	s_spell = data.value[1];
	t_actor *actor = turn_order[turn_index % turn_order.size()];

	if (actor->stat.pa.value < actor->spell[s_spell]->cost_pa ||
		actor->stat.pm.value < actor->spell[s_spell]->cost_pm ||
		(data.value[2] != -1 && actor->stat.pa.value != data.value[2]) ||
		(data.value[3] != -1 && actor->stat.pm.value != data.value[3]))
		return (false);
	board.reset_board();
	calculated = false;
	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		calculate_vision_circle();
	else
		calculate_vision_line();
	size_t i = 0;
	t_actor *target_actor = NULL;
	while (i < turn_order.size())
	{
		if ((target_actor == NULL || (board.get_cell(turn_order[i]->coord)->v_dist < board.get_cell(target_actor->coord)->v_dist && turn_order[i]->stat.hp.percent() < target_actor->stat.hp.percent())) &&
			turn_order[i]->team != turn_order[turn_index % turn_order.size()]->team && board.get_cell(turn_order[i]->coord)->cursor == t_vect(0, 2))
			target_actor = turn_order[i];
		i++;
	}
	if (target_actor == NULL)
		return (false);
	i = 0;

	vector<t_vect>	target_list; //every tile hited by the spell
	if (actor->spell[s_spell]->zone_type == Z_DIAM)
		target_list = calc_diam(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_CROSS)
		target_list = calc_cross(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_SQUARE)
		target_list = calc_square(actor->spell[s_spell]->zone_size);


	vector<t_vect> to_look; //every tile to look

	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		to_look = AI_calc_diam(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);
	if (actor->spell[s_spell]->range_type == R_LINE)
		to_look = AI_calc_cross(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);

	t_vect target = t_vect(-1, -1); //the saved coord of the spell
	int		utils_value = 999;

	for (size_t i = 0; i < to_look.size(); i++)
	{
		int find = 0;
		int nb_target_hit = 0;
		int percent_target_hit = 100;
		if (actor->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = actor->coord - to_look[i];
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
				((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			target_list = calc_line(actor->spell[s_spell]->zone_size, dir);
		}
		for (size_t j = 0; j < target_list.size(); j++)
		{
			t_vect tmp = actor->coord + to_look[i] + target_list[j];
			if (board.get_cell(tmp) && board.get_cell(actor->coord + to_look[i]) && board.get_cell(actor->coord + to_look[i])->cursor == t_vect(0, 2) && board.get_cell(tmp)->actor != NULL)
			{
				if (board.get_cell(tmp)->actor == target_actor)
					find = 1;
				if (board.get_cell(tmp)->actor->team != actor->team)
				{
					if (percent_target_hit >= board.get_cell(tmp)->actor->stat.hp.percent())
						percent_target_hit = board.get_cell(tmp)->actor->stat.hp.percent();
					nb_target_hit++;
				}
			}
		}
		if (nb_target_hit != 0 && percent_target_hit / nb_target_hit < utils_value && find == 1)
		{
			utils_value = percent_target_hit / nb_target_hit;
			target = actor->coord + to_look[i];
		}
	}
	if (target == t_vect(-1, -1))
		return (false);
	if (cast_spell(target) == false)
		return (false);
	board.reset_board();
	printf("%s atttack the enemy with lower HP %\n", turn_order[turn_index % turn_order.size()]->name.c_str());
	return (true);
}

bool				s_game_engine::help(t_ai_helper data)
{
	s_spell = data.value[1];
	t_actor *actor = turn_order[turn_index % turn_order.size()];
	if (actor->stat.pa.value < actor->spell[s_spell]->cost_pa ||
		actor->stat.pm.value < actor->spell[s_spell]->cost_pm ||
		(data.value[2] != -1 && actor->stat.pa.value != data.value[2]) ||
		(data.value[3] != -1 && actor->stat.pm.value != data.value[3]))
		return (false);
	board.reset_board();
	calculated = false;
	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		calculate_vision_circle();
	else
		calculate_vision_line();
	size_t i = 0;

	vector<t_vect>	target_list; //every tile hited by the spell
	if (actor->spell[s_spell]->zone_type == Z_DIAM)
		target_list = calc_diam(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_CROSS)
		target_list = calc_cross(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_SQUARE)
		target_list = calc_square(actor->spell[s_spell]->zone_size);


	vector<t_vect> to_look; //every tile to look

	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		to_look = AI_calc_diam(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);
	if (actor->spell[s_spell]->range_type == R_LINE)
		to_look = AI_calc_cross(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);

	t_vect target = t_vect(-1, -1); //the saved coord of the spell
	int		utils_value = 999;

	for (size_t i = 0; i < to_look.size(); i++)
	{
		int nb_target_hit = 0;
		int percent_target_hit = 100;
		if (actor->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = actor->coord - to_look[i];
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
				((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			target_list = calc_line(actor->spell[s_spell]->zone_size, dir);
		}
		for (size_t j = 0; j < target_list.size(); j++)
		{
			t_vect tmp = actor->coord + to_look[i] + target_list[j];
			if (board.get_cell(tmp) && board.get_cell(actor->coord + to_look[i]) && board.get_cell(actor->coord + to_look[i])->cursor == t_vect(0, 2) && board.get_cell(tmp)->actor != NULL)
			{
				if (board.get_cell(tmp)->actor->team == actor->team)
				{
					if (percent_target_hit > board.get_cell(tmp)->actor->stat.hp.percent())
						percent_target_hit = board.get_cell(tmp)->actor->stat.hp.percent();
					nb_target_hit++;
				}
			}
		}
		if (nb_target_hit != 0 && percent_target_hit / nb_target_hit < utils_value)
		{
			utils_value = percent_target_hit / nb_target_hit;
			target = actor->coord + to_look[i];
		}
	}
	if (target == t_vect(-1, -1))
		return (false);
	if (cast_spell(target) == false)
		return (false);
	board.reset_board();
	printf("%s help the closest ally\n", turn_order[turn_index % turn_order.size()]->name.c_str());
	return (true);
}

bool				s_game_engine::help_weak(t_ai_helper data)
{
	s_spell = data.value[1];
	t_actor *actor = turn_order[turn_index % turn_order.size()];
	if (actor->stat.pa.value < actor->spell[s_spell]->cost_pa ||
		actor->stat.pm.value < actor->spell[s_spell]->cost_pm ||
		(data.value[2] != -1 && actor->stat.pa.value != data.value[2]) ||
		(data.value[3] != -1 && actor->stat.pm.value != data.value[3]))
		return (false);
	board.reset_board();
	calculated = false;
	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		calculate_vision_circle();
	else
		calculate_vision_line();
	size_t i = 0;
	t_actor *target_actor = NULL;
	while (i < turn_order.size())
	{
		if ((target_actor == NULL || (board.get_cell(turn_order[i]->coord)->v_dist < board.get_cell(target_actor->coord)->v_dist && turn_order[i]->stat.hp.value < target_actor->stat.hp.value)) &&
			turn_order[i]->team != turn_order[turn_index % turn_order.size()]->team && board.get_cell(turn_order[i]->coord)->cursor == t_vect(0, 2))
			target_actor = turn_order[i];
		i++;
	}
	if (target_actor == NULL)
		return (false);
	i = 0;

	vector<t_vect>	target_list; //every tile hited by the spell
	if (actor->spell[s_spell]->zone_type == Z_DIAM)
		target_list = calc_diam(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_CROSS)
		target_list = calc_cross(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_SQUARE)
		target_list = calc_square(actor->spell[s_spell]->zone_size);


	vector<t_vect> to_look; //every tile to look

	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		to_look = AI_calc_diam(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);
	if (actor->spell[s_spell]->range_type == R_LINE)
		to_look = AI_calc_cross(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);

	t_vect target = t_vect(-1, -1); //the saved coord of the spell
	int		utils_value = 999;

	for (size_t i = 0; i < to_look.size(); i++)
	{
		int find = 0;
		int nb_target_hit = 0;
		int percent_target_hit = 100;
		if (actor->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = actor->coord - to_look[i];
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
				((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			target_list = calc_line(actor->spell[s_spell]->zone_size, dir);
		}
		for (size_t j = 0; j < target_list.size(); j++)
		{
			t_vect tmp = actor->coord + to_look[i] + target_list[j];
			if (board.get_cell(tmp) && board.get_cell(actor->coord + to_look[i]) && board.get_cell(actor->coord + to_look[i])->cursor == t_vect(0, 2) && board.get_cell(tmp)->actor != NULL)
			{
				if (board.get_cell(tmp)->actor == target_actor)
					find = 1;
				if (board.get_cell(tmp)->actor->team == actor->team)
				{
					if (percent_target_hit >= board.get_cell(tmp)->actor->stat.hp.percent())
						percent_target_hit = board.get_cell(tmp)->actor->stat.hp.percent();
					nb_target_hit++;
				}
			}
		}
		if (nb_target_hit != 0 && percent_target_hit / nb_target_hit < utils_value && find == 1)
		{
			utils_value = percent_target_hit / nb_target_hit;
			target = actor->coord + to_look[i];
		}
	}
	if (target == t_vect(-1, -1))
		return (false);
	if (cast_spell(target) == false)
		return (false);
	board.reset_board();
	printf("%s help the closest ally with lower HP value\n", turn_order[turn_index % turn_order.size()]->name.c_str());
	return (true);
}

bool				s_game_engine::help_percent(t_ai_helper data)
{
	s_spell = data.value[1];
	t_actor *actor = turn_order[turn_index % turn_order.size()];

	if (actor->stat.pa.value < actor->spell[s_spell]->cost_pa ||
		actor->stat.pm.value < actor->spell[s_spell]->cost_pm ||
		(data.value[3] != -1 && actor->stat.pa.value != data.value[3]) ||
		(data.value[4] != -1 && actor->stat.pm.value != data.value[4]))
		return (false);
	board.reset_board();
	calculated = false;
	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		calculate_vision_circle();
	else
		calculate_vision_line();
	size_t i = 0;
	t_actor *target_actor = NULL;
	while (i < turn_order.size())
	{
		t_actor *tmp = NULL;
		if (target_actor == NULL && board.get_cell(turn_order[i]->coord)->cursor == t_vect(0, 2))
			tmp = turn_order[i];
		else if (target_actor != NULL && board.get_cell(turn_order[i]->coord)->v_dist < board.get_cell(target_actor->coord)->v_dist && board.get_cell(turn_order[i]->coord)->cursor == t_vect(0, 2))
			tmp = turn_order[i];
		if (tmp != NULL)
		{
			if (tmp->stat.hp.percent() > data.value[2])
				tmp = NULL;
			if(tmp == NULL)
				break ;
			if (tmp->team != turn_order[turn_index % turn_order.size()]->team)
				tmp = NULL;
			if(tmp == NULL)
				break ;
			if (tmp != NULL)
				target_actor = tmp;
		}
		i++;
	}
	if (target_actor == NULL)
		return (false);
	i = 0;

	vector<t_vect>	target_list; //every tile hited by the spell
	if (actor->spell[s_spell]->zone_type == Z_DIAM)
		target_list = calc_diam(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_CROSS)
		target_list = calc_cross(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_SQUARE)
		target_list = calc_square(actor->spell[s_spell]->zone_size);


	vector<t_vect> to_look; //every tile to look

	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		to_look = AI_calc_diam(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);
	if (actor->spell[s_spell]->range_type == R_LINE)
		to_look = AI_calc_cross(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);

	t_vect target = t_vect(-1, -1); //the saved coord of the spell
	int		utils_value = 999;

	for (size_t i = 0; i < to_look.size(); i++)
	{
		int find = 0;
		int nb_target_hit = 0;
		int percent_target_hit = 100;
		if (actor->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = actor->coord - to_look[i];
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
				((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			target_list = calc_line(actor->spell[s_spell]->zone_size, dir);
		}
		for (size_t j = 0; j < target_list.size(); j++)
		{
			t_vect tmp = actor->coord + to_look[i] + target_list[j];
			if (board.get_cell(tmp) && board.get_cell(actor->coord + to_look[i]) && board.get_cell(actor->coord + to_look[i])->cursor == t_vect(0, 2) && board.get_cell(tmp)->actor != NULL)
			{
				if (board.get_cell(tmp)->actor == target_actor)
					find = 1;
				if (board.get_cell(tmp)->actor->team == actor->team)
				{
					if (percent_target_hit >= board.get_cell(tmp)->actor->stat.hp.percent())
						percent_target_hit = board.get_cell(tmp)->actor->stat.hp.percent();
					nb_target_hit++;
				}
			}
		}
		if (nb_target_hit != 0 && percent_target_hit / nb_target_hit < utils_value && find == 1)
		{
			utils_value = percent_target_hit / nb_target_hit;
			target = actor->coord + to_look[i];
		}
	}
	if (target == t_vect(-1, -1))
		return (false);
	if (cast_spell(target) == false)
		return (false);
	board.reset_board();
	printf("%s help the closest ally with HP % < %d\n", turn_order[turn_index % turn_order.size()]->name.c_str(), data.value[0]);
	return (true);
}

bool				s_game_engine::help_caster_hp(t_ai_helper data)
{
	s_spell = data.value[1];
	t_actor *actor = turn_order[turn_index % turn_order.size()];

	if (actor->stat.pa.value < actor->spell[s_spell]->cost_pa ||
		actor->stat.pm.value < actor->spell[s_spell]->cost_pm ||
		(data.value[3] != -1 && actor->stat.pa.value != data.value[3]) ||
		(data.value[4] != -1 && actor->stat.pm.value != data.value[4]) ||
		actor->stat.hp.percent() > data.value[2])
		return (false);
	board.reset_board();
	calculated = false;
	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		calculate_vision_circle();
	else
		calculate_vision_line();
	size_t i = 0;
	t_actor *target_actor = NULL;
	while (i < turn_order.size())
	{
		t_actor *tmp = NULL;
		if (target_actor == NULL && board.get_cell(turn_order[i]->coord)->cursor == t_vect(0, 2))
			tmp = turn_order[i];
		else if (target_actor != NULL && board.get_cell(turn_order[i]->coord)->v_dist < board.get_cell(target_actor->coord)->v_dist && board.get_cell(turn_order[i]->coord)->cursor == t_vect(0, 2))
			tmp = turn_order[i];
		if (tmp != NULL)
		{
			if (target_actor != NULL && tmp->stat.hp.percent() > target_actor->stat.hp.percent())
				tmp = NULL;
			if (tmp == NULL)
				break;
			if (tmp->team != turn_order[turn_index % turn_order.size()]->team)
				tmp = NULL;
			if (tmp == NULL)
				break;
			if (tmp != NULL)
				target_actor = tmp;
		}
		i++;
	}
	if (target_actor == NULL)
		return (false);
	i = 0;

	vector<t_vect>	target_list; //every tile hited by the spell
	if (actor->spell[s_spell]->zone_type == Z_DIAM)
		target_list = calc_diam(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_CROSS)
		target_list = calc_cross(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_SQUARE)
		target_list = calc_square(actor->spell[s_spell]->zone_size);


	vector<t_vect> to_look; //every tile to look

	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		to_look = AI_calc_diam(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);
	if (actor->spell[s_spell]->range_type == R_LINE)
		to_look = AI_calc_cross(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);

	t_vect target = t_vect(-1, -1); //the saved coord of the spell
	int		utils_value = 999;

	for (size_t i = 0; i < to_look.size(); i++)
	{
		int find = 0;
		int nb_target_hit = 0;
		int percent_target_hit = 100;
		if (actor->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = actor->coord - to_look[i];
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
				((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			target_list = calc_line(actor->spell[s_spell]->zone_size, dir);
		}
		for (size_t j = 0; j < target_list.size(); j++)
		{
			t_vect tmp = actor->coord + to_look[i] + target_list[j];
			if (board.get_cell(tmp) && board.get_cell(actor->coord + to_look[i]) && board.get_cell(actor->coord + to_look[i])->cursor == t_vect(0, 2) && board.get_cell(tmp)->actor != NULL)
			{
				if (board.get_cell(tmp)->actor == target_actor)
					find = 1;
				if (board.get_cell(tmp)->actor->team == actor->team)
				{
					if (percent_target_hit >= board.get_cell(tmp)->actor->stat.hp.percent())
						percent_target_hit = board.get_cell(tmp)->actor->stat.hp.percent();
					nb_target_hit++;
				}
			}
		}
		if (nb_target_hit != 0 && percent_target_hit / nb_target_hit < utils_value && find == 1)
		{
			utils_value = percent_target_hit / nb_target_hit;
			target = actor->coord + to_look[i];
		}
	}
	if (target == t_vect(-1, -1))
		return (false);
	if (cast_spell(target) == false)
		return (false);
	board.reset_board();
	printf("%s (with > %d % HP) help the closest ally with lower HP %\n", turn_order[turn_index % turn_order.size()]->name.c_str(), data.value[2]);
	return (true);
}

bool				s_game_engine::attack_caster_hp(t_ai_helper data)
{
	s_spell = data.value[1];
	t_actor *actor = turn_order[turn_index % turn_order.size()];
	if (actor->stat.pa.value < actor->spell[s_spell]->cost_pa ||
		actor->stat.pm.value < actor->spell[s_spell]->cost_pm ||
		(data.value[3] != -1 && actor->stat.pa.value != data.value[3]) ||
		(data.value[4] != -1 && actor->stat.pm.value != data.value[4]) ||
		actor->stat.hp.percent() > data.value[2])
		return (false);
	board.reset_board();
	calculated = false;
	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		calculate_vision_circle();
	else
		calculate_vision_line();
	size_t i = 0;

	vector<t_vect>	target_list; //every tile hited by the spell
	if (actor->spell[s_spell]->zone_type == Z_DIAM)
		target_list = calc_diam(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_CROSS)
		target_list = calc_cross(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_SQUARE)
		target_list = calc_square(actor->spell[s_spell]->zone_size);


	vector<t_vect> to_look; //every tile to look

	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		to_look = AI_calc_diam(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);
	if (actor->spell[s_spell]->range_type == R_LINE)
		to_look = AI_calc_cross(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);

	t_vect target = t_vect(-1, -1); //the saved coord of the spell
	int		utils_value = 999;

	for (size_t i = 0; i < to_look.size(); i++)
	{
		int nb_target_hit = 0;
		int percent_target_hit = 100;
		if (actor->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = actor->coord - to_look[i];
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
				((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			target_list = calc_line(actor->spell[s_spell]->zone_size, dir);
		}
		for (size_t j = 0; j < target_list.size(); j++)
		{
			t_vect tmp = actor->coord + to_look[i] + target_list[j];
			if (board.get_cell(tmp) && board.get_cell(actor->coord + to_look[i]) && board.get_cell(actor->coord + to_look[i])->cursor == t_vect(0, 2) && board.get_cell(tmp)->actor != NULL)
			{
				if (board.get_cell(tmp)->actor->team != actor->team)
				{
					if (percent_target_hit >= board.get_cell(tmp)->actor->stat.hp.percent())
						percent_target_hit = board.get_cell(tmp)->actor->stat.hp.percent();
					nb_target_hit++;
				}
			}
		}
		if (nb_target_hit != 0 && percent_target_hit / nb_target_hit < utils_value)
		{
			utils_value = percent_target_hit / nb_target_hit;
			target = actor->coord + to_look[i];
		}
	}
	if (target == t_vect(-1, -1))
		return (false);
	if (cast_spell(target) == false)
		return (false);
	board.reset_board();
	printf("%s (with less than %d % HP) atttack the closest enemy\n", turn_order[turn_index % turn_order.size()]->name.c_str(), data.value[2]);
	return (true);
}

bool				s_game_engine::action_on_turn(t_ai_helper data)
{
	(void)data;
	return (false);

}
