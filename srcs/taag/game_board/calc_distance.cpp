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
