#include "taag.h"


void				s_game_engine::m_calc_cell(vector<t_vect> *to_calc, int i, int x, int j, int y)
{
	if (board.get_cell(i + x, j + y) != NULL && board.get_cell(i + x, j + y)->node->m_obs == false &&
		board.get_cell(i + x, j + y)->m_dist > board.get_cell(i, j)->m_dist + (board.get_cell(i + x, j + y)->actor == NULL ? board.get_cell(i + x, j + y)->node->cost : 999))
	{
		board.get_cell(i + x, j + y)->m_dist = board.get_cell(i, j)->m_dist + (board.get_cell(i + x, j + y)->actor == NULL ? board.get_cell(i + x, j + y)->node->cost : 999);
		to_calc->push_back(t_vect(i + x, j + y));
	}
}

void				s_game_engine::calculate_distance()
{
	board.reset_board();
	t_actor 		*player = turn_order[turn_index % turn_order.size()];
	vector<t_vect>	to_calc;
	size_t i, j;

	to_calc.push_back(player->coord);

	i = 0;
	board.get_cell(to_calc[i].x, to_calc[i].y)->m_dist = 0;
	while (i < to_calc.size())
	{
		m_calc_cell(&to_calc, to_calc[i].x, 1, to_calc[i].y, 0);
		m_calc_cell(&to_calc, to_calc[i].x, -1, to_calc[i].y, 0);
		m_calc_cell(&to_calc, to_calc[i].x, 0, to_calc[i].y, 1);
		m_calc_cell(&to_calc, to_calc[i].x, 0, to_calc[i].y, -1);
		i++;
	}
	i = 1;
	while (i < to_calc.size() && player->team == 1)
	{
		if (board.get_cell(to_calc[i].x, to_calc[i].y)->m_dist <= turn_order[turn_index % turn_order.size()]->stat.pm.value)
			board.get_cell(to_calc[i].x, to_calc[i].y)->cursor = t_vect(1, 2);
		i++;
	}
	calculated = true;
}

vector<t_vect>		s_game_engine::pathfinding(t_vect dest)
{
	vector<t_vect>	path;
	t_vect			actual = dest;
	t_vect			to_look = actual;
	t_vect			source = turn_order[turn_index % turn_order.size()]->coord;

	if (board.get_cell(dest) && board.get_cell(dest)->actor != NULL &&
		((abs(source.x - dest.x) == 1 && source.y - dest.y == 0) ||
		(abs(source.y - dest.y) == 1 && source.x - dest.x == 0)))
		actual = source;
	while (actual != source)
	{
		actual = to_look;
		if (board.get_cell((int)(actual.x) + 1, (int)(actual.y)) && board.get_cell((int)(actual.x) + 1, (int)(actual.y))->m_dist < board.get_cell(to_look.x, to_look.y)->m_dist)
			to_look = t_vect((int)(actual.x) + 1, (int)(actual.y));
		if (board.get_cell((int)(actual.x) - 1, (int)(actual.y)) && board.get_cell((int)(actual.x) - 1, (int)(actual.y))->m_dist < board.get_cell(to_look.x, to_look.y)->m_dist)
			to_look = t_vect((int)(actual.x) - 1, (int)(actual.y));
		if (board.get_cell((int)(actual.x), (int)(actual.y) + 1) && board.get_cell((int)(actual.x), (int)(actual.y) + 1)->m_dist < board.get_cell(to_look.x, to_look.y)->m_dist)
			to_look = t_vect((int)(actual.x), (int)(actual.y) + 1);
		if (board.get_cell((int)(actual.x), (int)(actual.y) - 1) && board.get_cell((int)(actual.x), (int)(actual.y) - 1)->m_dist < board.get_cell(to_look.x, to_look.y)->m_dist)
			to_look = t_vect((int)(actual.x), (int)(actual.y) - 1);
		if (actual == to_look)
			break;
		if (board.get_cell((int)(actual.x), (int)(actual.y))->m_dist <= turn_order[turn_index % turn_order.size()]->stat.pm.value && actual != source)
		{
			int i = 0;
			t_vect delta = ((to_look - actual) / (15 * board.get_cell((int)(actual.x), (int)(actual.y))->node->cost));
			while (i < 15 * board.get_cell((int)(actual.x), (int)(actual.y))->node->cost)
			{
				path.insert(path.begin(), actual + delta * i);
				i++;
			}
		}
	}
	calculated = false;
	return (path);
}

vector<t_vect>		s_game_engine::calc_path(t_vect dest)
{
	vector<t_vect>	path;
	t_vect			actual = dest;
	t_vect			to_look = actual;
	t_vect			source = turn_order[turn_index % turn_order.size()]->coord;

	if (board.get_cell(dest.x, dest.y) == NULL || (board.get_cell(dest.x, dest.y)->m_dist == 999 && board.get_cell(dest.x, dest.y)->actor == NULL) )
		return (path);
	while (actual != source)
	{
		actual = to_look;
		if (board.get_cell((int)(actual.x) + 1, (int)(actual.y)) && board.get_cell((int)(actual.x) + 1, (int)(actual.y))->m_dist < board.get_cell(to_look)->m_dist)
			to_look = t_vect((int)(actual.x) + 1, (int)(actual.y));
		if (board.get_cell((int)(actual.x) - 1, (int)(actual.y)) && board.get_cell((int)(actual.x) - 1, (int)(actual.y))->m_dist < board.get_cell(to_look)->m_dist)
			to_look = t_vect((int)(actual.x) - 1, (int)(actual.y));
		if (board.get_cell((int)(actual.x), (int)(actual.y) + 1) && board.get_cell((int)(actual.x), (int)(actual.y) + 1)->m_dist < board.get_cell(to_look)->m_dist)
			to_look = t_vect((int)(actual.x), (int)(actual.y) + 1);
		if (board.get_cell((int)(actual.x), (int)(actual.y) - 1) && board.get_cell((int)(actual.x), (int)(actual.y) - 1)->m_dist < board.get_cell(to_look)->m_dist)
			to_look = t_vect((int)(actual.x), (int)(actual.y) - 1);
		if (to_look == actual)
			break;
		if (to_look != source)
			path.insert(path.begin(), to_look);
	}
	if (path.size() != 0 || (path.size() == 0 && board.get_cell(dest)->actor != NULL))
		path.insert(path.begin(), dest);
	if (path.size() == 2 && path[0] == path[1])
		path.clear();
	return (path);
}
