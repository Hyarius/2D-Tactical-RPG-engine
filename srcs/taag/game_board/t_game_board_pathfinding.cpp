#include "taag.h"


void				s_game_board::m_calc_cell(vector<t_vect> *to_calc, int i, int x, int j, int y)
{
	if (get_cell(i + x, j + y) != NULL && get_cell(i + x, j + y)->node->m_obs == false &&
		get_cell(i + x, j + y)->m_dist > get_cell(i, j)->m_dist + (get_cell(i + x, j + y)->actor == NULL ? get_cell(i + x, j + y)->node->cost : 999))
	{
		get_cell(i + x, j + y)->m_dist = get_cell(i, j)->m_dist + (get_cell(i + x, j + y)->actor == NULL ? get_cell(i + x, j + y)->node->cost : 999);
		to_calc->push_back(t_vect(i + x, j + y));
	}
}

void				s_game_board::calculate_distance(t_vect start)
{
	reset_board();
	vector<t_vect>	to_calc;
	size_t i;

	to_calc.push_back(start);

	i = 0;
	get_cell(to_calc[i].x, to_calc[i].y)->m_dist = 0;
	while (i < to_calc.size())
	{
		m_calc_cell(&to_calc, to_calc[i].x, 1, to_calc[i].y, 0);
		m_calc_cell(&to_calc, to_calc[i].x, -1, to_calc[i].y, 0);
		m_calc_cell(&to_calc, to_calc[i].x, 0, to_calc[i].y, 1);
		m_calc_cell(&to_calc, to_calc[i].x, 0, to_calc[i].y, -1);
		i++;
	}
	i = 1;
	t_actor *player = turn_order[turn_index % turn_order.size()];
	while (i < to_calc.size() && player->team == 1)
	{
		if (get_cell(to_calc[i].x, to_calc[i].y)->m_dist <= turn_order[turn_index % turn_order.size()]->stat.pm.value)
			get_cell(to_calc[i].x, to_calc[i].y)->cursor = t_vect(1, 2);
		i++;
	}
	calculated = true;
}

vector<t_vect>		s_game_board::pathfinding(t_actor *player, t_vect dest, double speed)
{
	vector<t_vect>	path;
	t_vect			actual = dest;
	t_vect			to_look = actual;
	t_vect			source = player->coord;

	if (get_cell(dest) && get_cell(dest)->actor != NULL &&
		((abs(source.x - dest.x) == 1 && source.y - dest.y == 0) ||
		(abs(source.y - dest.y) == 1 && source.x - dest.x == 0)))
		actual = source;
	while (actual != source)
	{
		actual = to_look;
		if (get_cell((int)(actual.x) + 1, (int)(actual.y)) && get_cell((int)(actual.x) + 1, (int)(actual.y))->m_dist < get_cell(to_look.x, to_look.y)->m_dist)
			to_look = t_vect((int)(actual.x) + 1, (int)(actual.y));
		if (get_cell((int)(actual.x) - 1, (int)(actual.y)) && get_cell((int)(actual.x) - 1, (int)(actual.y))->m_dist < get_cell(to_look.x, to_look.y)->m_dist)
			to_look = t_vect((int)(actual.x) - 1, (int)(actual.y));
		if (get_cell((int)(actual.x), (int)(actual.y) + 1) && get_cell((int)(actual.x), (int)(actual.y) + 1)->m_dist < get_cell(to_look.x, to_look.y)->m_dist)
			to_look = t_vect((int)(actual.x), (int)(actual.y) + 1);
		if (get_cell((int)(actual.x), (int)(actual.y) - 1) && get_cell((int)(actual.x), (int)(actual.y) - 1)->m_dist < get_cell(to_look.x, to_look.y)->m_dist)
			to_look = t_vect((int)(actual.x), (int)(actual.y) - 1);
		if (actual == to_look)
			break;
		if ((get_cell(actual)->m_dist <= player->stat.pm.value && actual != source) || speed != 1)
		{
			int i = 0;
			t_vect delta = ((to_look - actual) / ((15.0 * speed) * (speed != 1 ? get_cell(actual)->node->cost : 1)));
			while (i < (15.0 * speed) * (speed != 1 ? get_cell(actual)->node->cost : 1))
			{
				path.insert(path.begin(), actual + delta * i);
				i++;
			}
		}
	}
	calculated = false;
	return (path);
}

vector<t_vect>		s_game_board::calc_path(t_vect dest)
{
	vector<t_vect>	path;
	t_vect			actual = dest;
	t_vect			to_look = actual;
	t_vect			source = turn_order[turn_index % turn_order.size()]->coord;

	if (get_cell(dest.x, dest.y) == NULL || (get_cell(dest.x, dest.y)->m_dist == 999 && get_cell(dest.x, dest.y)->actor == NULL) )
		return (path);
	while (actual != source)
	{
		actual = to_look;
		if (get_cell((int)(actual.x) + 1, (int)(actual.y)) && get_cell((int)(actual.x) + 1, (int)(actual.y))->m_dist < get_cell(to_look)->m_dist)
			to_look = t_vect((int)(actual.x) + 1, (int)(actual.y));
		if (get_cell((int)(actual.x) - 1, (int)(actual.y)) && get_cell((int)(actual.x) - 1, (int)(actual.y))->m_dist < get_cell(to_look)->m_dist)
			to_look = t_vect((int)(actual.x) - 1, (int)(actual.y));
		if (get_cell((int)(actual.x), (int)(actual.y) + 1) && get_cell((int)(actual.x), (int)(actual.y) + 1)->m_dist < get_cell(to_look)->m_dist)
			to_look = t_vect((int)(actual.x), (int)(actual.y) + 1);
		if (get_cell((int)(actual.x), (int)(actual.y) - 1) && get_cell((int)(actual.x), (int)(actual.y) - 1)->m_dist < get_cell(to_look)->m_dist)
			to_look = t_vect((int)(actual.x), (int)(actual.y) - 1);
		if (to_look == actual)
			break;
		if (to_look != source)
			path.insert(path.begin(), to_look);
	}
	if (path.size() != 0 || (path.size() == 0 && get_cell(dest)->actor != NULL))
		path.insert(path.begin(), dest);
	if (path.size() == 2 && path[0] == path[1])
		path.clear();
	return (path);
}
