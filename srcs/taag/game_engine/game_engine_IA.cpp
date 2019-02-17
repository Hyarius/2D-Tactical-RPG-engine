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
			if (abs(i + j) >= size_min)
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

t_vect				s_game_engine::get_close_enemy(int distance, int type)
{
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
				board.get_cell(to_calc[i] + range[count])->cursor = t_vect(2, 2);
				vector<t_vect> path = calc_path(to_calc[i] + range[count]);
				if (path.size() < j)
				{
					j = path.size();
					result = to_calc[i] + range[count];
				}
			}
			count++;
		}
		i++;
	}
	return (result);
}

t_vect				s_game_engine::get_close_ally(int distance, int type)
{
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
				board.get_cell(to_calc[i] + range[count])->cursor = t_vect(2, 2);
				vector<t_vect> path = calc_path(to_calc[i] + range[count]);
				if (path.size() < j)
				{
					j = path.size();
					result = to_calc[i] + range[count];
				}
			}
			count++;
		}
		i++;
	}
	return (result);
}

t_vect				s_game_engine::flee_enemy()
{
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
	return (actual);
}

void				s_game_engine::enemy_turn()
{
	board.reset_board();
	t_vect	target = flee_enemy();
	if (board.get_cell(turn_order[turn_index % turn_order.size()]->coord)->cursor != t_vect(2, 2) && target != t_vect(-1, -1))
		move_actor(target);
	SDL_Event	event;

	while (board.check_anim() == false || turn_order[turn_index % turn_order.size()]->destination.size() != 0)
	{
		prepare_screen();

		update_board();
		draw_board();
		draw_gui();

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				exit(0);
			handle_control_camera(&event);
		}
		render_screen(true);
	}
	next_turn();
}

void				s_game_engine::next_turn()
{
	if (turn_order.size())
	{
		turn_order[turn_index % turn_order.size()]->selected = false;
		turn_order[turn_index % turn_order.size()]->reset_value();
	}
	turn_index = (turn_index + 1) % turn_order.size();
	if (turn_order.size())
	{
		turn_order[turn_index % turn_order.size()]->selected = true;
	}
	calculated = false;
	s_spell = -1;
	if (turn_order[turn_index % turn_order.size()]->team != 1)
	{
		enemy_turn();
	}
}
