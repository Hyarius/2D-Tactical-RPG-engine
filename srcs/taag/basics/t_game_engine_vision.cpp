#include "taag.h"

map<t_vect, map<t_vect, vector<t_vect>[4]>>	vision_map;

void				s_game_engine::v_calc_cell(vector<t_vect> *to_calc, t_vect target, int prev_dist)
{
	t_vect 			player_coord = turn_order[turn_index % turn_order.size()]->coord;
	int				scale = 10;

	if (vision_map.find(player_coord) == vision_map.end() || vision_map[player_coord].find(target) == vision_map[player_coord].end())
	{
		vision_map[player_coord][target][0] = calc_line_2d(t_vect(player_coord.x + 0.49, player_coord.y + 0.49) * scale, t_vect(target.x + 0.49, target.y + 0.49) * scale);
		vision_map[player_coord][target][1] = calc_line_2d(t_vect(player_coord.x + 0.51, player_coord.y + 0.49) * scale, t_vect(target.x + 0.51, target.y + 0.49) * scale);
		vision_map[player_coord][target][2] = calc_line_2d(t_vect(player_coord.x + 0.49, player_coord.y + 0.51) * scale, t_vect(target.x + 0.49, target.y + 0.51) * scale);
		vision_map[player_coord][target][3] = calc_line_2d(t_vect(player_coord.x + 0.51, player_coord.y + 0.51) * scale, t_vect(target.x + 0.51, target.y + 0.51) * scale);
	}
	vector<t_vect>	vision_tl = vision_map[player_coord][target][0];
	vector<t_vect>	vision_tr = vision_map[player_coord][target][1];
	vector<t_vect>	vision_dl = vision_map[player_coord][target][2];
	vector<t_vect>	vision_dr = vision_map[player_coord][target][3];

	if (board.get_cell(target) && (board.get_cell(target)->node->v_obs == false || turn_order[turn_index % turn_order.size()]->spell[s_spell]->block == false) && board.get_cell(target)->v_dist >= prev_dist)
	{
		size_t i = 0;
		bool visible = true;
		while (turn_order[turn_index % turn_order.size()]->spell[s_spell]->block == true && i < vision_tl.size() && visible == true && board.get_cell(vision_dr[i] / scale))
		{
			if (board.get_cell(vision_tl[i] / scale)->node->v_obs == true ||
				(board.get_cell(vision_tl[i] / scale)->actor != NULL &&
				board.get_cell(vision_tl[i] / scale)->actor != turn_order[turn_index % turn_order.size()] &&
				board.get_cell(vision_tl[i] / scale)->actor != board.get_cell(target)->actor))
				if (board.get_cell(vision_tr[i] / scale)->node->v_obs == true ||
					(board.get_cell(vision_tr[i] / scale)->actor != NULL &&
					board.get_cell(vision_tr[i] / scale)->actor != turn_order[turn_index % turn_order.size()] &&
					board.get_cell(vision_tr[i] / scale)->actor != board.get_cell(target)->actor))
					if (board.get_cell(vision_dl[i] / scale)->node->v_obs == true ||
						(board.get_cell(vision_dl[i] / scale)->actor != NULL &&
						board.get_cell(vision_dl[i] / scale)->actor != turn_order[turn_index % turn_order.size()] &&
						board.get_cell(vision_dl[i] / scale)->actor != board.get_cell(target)->actor))
						if (board.get_cell(vision_dr[i] / scale)->node->v_obs == true ||
							(board.get_cell(vision_dr[i] / scale)->actor != NULL &&
							board.get_cell(vision_dr[i] / scale)->actor != turn_order[turn_index % turn_order.size()] &&
							board.get_cell(vision_dr[i] / scale)->actor != board.get_cell(target)->actor))
							visible = false;
			i++;
		}
		if (visible == true)
		{
			board.get_cell(target)->v_dist = prev_dist + 1;
			if (board.get_cell(target)->v_dist <= turn_order[turn_index % turn_order.size()]->spell[s_spell]->range[1])
			{
				if (board.get_cell(target)->v_dist >= turn_order[turn_index % turn_order.size()]->spell[s_spell]->range[0] &&
					board.get_cell(target)->node->v_obs == false && board.get_cell(target)->node->tile != NULL)
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