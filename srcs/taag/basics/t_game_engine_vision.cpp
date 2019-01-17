#include "taag.h"

void				s_game_engine::v_calc_cell(vector<t_vect> *to_calc, t_vect target, int prev_dist)
{
	t_vect 			player_coord = turn_order[turn_index % turn_order.size()]->coord;
	vector<t_vect>	vision_tl = calc_line_2d(t_vect(player_coord.x + 0.49, player_coord.y + 0.49) * 10, t_vect(target.x + 0.49, target.y + 0.49) * 10);
	vector<t_vect>	vision_tr = calc_line_2d(t_vect(player_coord.x + 0.51, player_coord.y + 0.49) * 10, t_vect(target.x + 0.51, target.y + 0.49) * 10);
	vector<t_vect>	vision_dl = calc_line_2d(t_vect(player_coord.x + 0.49, player_coord.y + 0.51) * 10, t_vect(target.x + 0.49, target.y + 0.51) * 10);
	vector<t_vect>	vision_dr = calc_line_2d(t_vect(player_coord.x + 0.51, player_coord.y + 0.51) * 10, t_vect(target.x + 0.51, target.y + 0.51) * 10);

	if (board.get_cell(target) && (board.get_cell(target)->node->v_obs == false || turn_order[turn_index % turn_order.size()]->spell[s_spell]->block == false) && board.get_cell(target)->v_dist >= prev_dist)
	{
		size_t i = 0;
		bool visible = true;
		while (turn_order[turn_index % turn_order.size()]->spell[s_spell]->block == true && i < vision_tl.size() && visible == true)
		{
			if (board.get_cell(vision_tl[i] / 10)->node->v_obs == true ||
				(board.get_cell(vision_tl[i] / 10)->actor != NULL &&
				board.get_cell(vision_tl[i] / 10)->actor != turn_order[turn_index % turn_order.size()] &&
				board.get_cell(vision_tl[i] / 10)->actor != board.get_cell(target)->actor))
				if (board.get_cell(vision_tr[i] / 10)->node->v_obs == true ||
					(board.get_cell(vision_tr[i] / 10)->actor != NULL &&
					board.get_cell(vision_tr[i] / 10)->actor != turn_order[turn_index % turn_order.size()] &&
					board.get_cell(vision_tr[i] / 10)->actor != board.get_cell(target)->actor))
					if (board.get_cell(vision_dl[i] / 10)->node->v_obs == true ||
						(board.get_cell(vision_dl[i] / 10)->actor != NULL &&
						board.get_cell(vision_dl[i] / 10)->actor != turn_order[turn_index % turn_order.size()] &&
						board.get_cell(vision_dl[i] / 10)->actor != board.get_cell(target)->actor))
						if (board.get_cell(vision_dr[i] / 10)->node->v_obs == true ||
							(board.get_cell(vision_dr[i] / 10)->actor != NULL &&
							board.get_cell(vision_dr[i] / 10)->actor != turn_order[turn_index % turn_order.size()] &&
							board.get_cell(vision_dr[i] / 10)->actor != board.get_cell(target)->actor))
							visible = false;
			i++;
		}
		if (visible == true)
		{
			board.get_cell(target)->v_dist = prev_dist + 1;
			if (board.get_cell(target)->v_dist <= turn_order[turn_index % turn_order.size()]->spell[s_spell]->range[1])
			{
				if (board.get_cell(target)->v_dist >= turn_order[turn_index % turn_order.size()]->spell[s_spell]->range[0] &&
					board.get_cell(target)->node->v_obs == false)
					board.get_cell(target)->cursor = t_vect(0, 2);
				if (to_calc != NULL)
					to_calc->push_back(target);
			}
		}
	}
}

void				s_game_engine::calculate_vision_circle()
{
	board.reset_board();
	t_actor 		*player = turn_order[turn_index % turn_order.size()];
	vector<t_vect>	to_calc;
	size_t i;

	to_calc.push_back(player->coord);
	board.get_cell(to_calc[0])->v_dist = 0;
	i = 0;
	while (i < to_calc.size())
	{
		v_calc_cell(&to_calc, t_vect(to_calc[i].x + 1, to_calc[i].y), board.get_cell(to_calc[i])->v_dist);
		v_calc_cell(&to_calc, t_vect(to_calc[i].x - 1, to_calc[i].y), board.get_cell(to_calc[i])->v_dist);
		v_calc_cell(&to_calc, t_vect(to_calc[i].x, to_calc[i].y + 1), board.get_cell(to_calc[i])->v_dist);
		v_calc_cell(&to_calc, t_vect(to_calc[i].x, to_calc[i].y - 1), board.get_cell(to_calc[i])->v_dist);
		i++;
	}
	calculated = true;
}

void				s_game_engine::calculate_vision_line()
{
	board.reset_board();
	t_actor 		*player = turn_order[turn_index % turn_order.size()];
	int i;

	board.get_cell(player->coord)->v_dist = 0;
	i = 0;
	while (i <= player->spell[s_spell]->range[1])
	{
		v_calc_cell(NULL, t_vect(player->coord.x + i, player->coord.y), i - 1);
		v_calc_cell(NULL, t_vect(player->coord.x - i, player->coord.y), i - 1);
		v_calc_cell(NULL, t_vect(player->coord.x, player->coord.y + i), i - 1);
		v_calc_cell(NULL, t_vect(player->coord.x, player->coord.y - i), i - 1);
		i++;
	}
	calculated = true;
}
\
