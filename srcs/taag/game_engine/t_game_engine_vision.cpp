#include "taag.h"

map<t_vect, map<t_vect, vector<t_vect>[4]>>	vision_map;

bool				s_game_engine::v_calc_cell(t_vect source, t_vect target, int prev_dist)
{
	int				scale = 5;

	if (vision_map.find(source) == vision_map.end() || vision_map[source].find(target) == vision_map[source].end())
	{
		vision_map[source][target][0] = calc_line_2d(t_vect(source.x + 0.49, source.y + 0.49) * scale, t_vect(target.x + 0.49, target.y + 0.49) * scale);
		vision_map[source][target][1] = calc_line_2d(t_vect(source.x + 0.51, source.y + 0.49) * scale, t_vect(target.x + 0.51, target.y + 0.49) * scale);
		vision_map[source][target][2] = calc_line_2d(t_vect(source.x + 0.49, source.y + 0.51) * scale, t_vect(target.x + 0.49, target.y + 0.51) * scale);
		vision_map[source][target][3] = calc_line_2d(t_vect(source.x + 0.51, source.y + 0.51) * scale, t_vect(target.x + 0.51, target.y + 0.51) * scale);
	}
	vector<t_vect>	vision_tl = vision_map[source][target][0];
	vector<t_vect>	vision_tr = vision_map[source][target][1];
	vector<t_vect>	vision_dl = vision_map[source][target][2];
	vector<t_vect>	vision_dr = vision_map[source][target][3];

	if (board.get_cell(target) && (board.get_cell(target)->node->v_obs == false || turn_order[turn_index % turn_order.size()]->spell[s_spell]->block == INT_TRUE) && board.get_cell(target)->v_dist >= prev_dist)
	{
		size_t i = 0;
		bool visible = true;
		while (turn_order[turn_index % turn_order.size()]->spell[s_spell]->block == INT_FALSE && i < vision_tl.size() && visible == true && board.get_cell(vision_dr[i] / scale))
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
				return (true);
			}
		}
	}
	return (false);
}

void				s_game_engine::calculate_vision_circle()
{
	board.reset_board();
	t_actor 		*player = turn_order[turn_index % turn_order.size()];
	vector<t_vect>	to_calc;
	size_t i;

	to_calc.push_back(player->coord);
	board.get_cell(to_calc[0])->v_dist = 0;
	if (player->spell[s_spell]->range[0] <= 0)
		board.get_cell(player->coord)->cursor = t_vect(0, 2);
	i = 0;
	while (i < to_calc.size())
	{
		if(v_calc_cell(player->coord, t_vect(to_calc[i].x + 1, to_calc[i].y), board.get_cell(to_calc[i])->v_dist) == true)
			to_calc.push_back(t_vect(to_calc[i].x + 1, to_calc[i].y));
		if(v_calc_cell(player->coord, t_vect(to_calc[i].x - 1, to_calc[i].y), board.get_cell(to_calc[i])->v_dist) == true)
			to_calc.push_back(t_vect(to_calc[i].x - 1, to_calc[i].y));
		if(v_calc_cell(player->coord, t_vect(to_calc[i].x, to_calc[i].y + 1), board.get_cell(to_calc[i])->v_dist) == true)
			to_calc.push_back(t_vect(to_calc[i].x, to_calc[i].y + 1));
		if(v_calc_cell(player->coord, t_vect(to_calc[i].x, to_calc[i].y - 1), board.get_cell(to_calc[i])->v_dist) == true)
			to_calc.push_back(t_vect(to_calc[i].x, to_calc[i].y - 1));
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
	if (player->spell[s_spell]->range[0] <= 0)
		board.get_cell(player->coord)->cursor = t_vect(0, 2);
	i = 0;
	while (i <= player->spell[s_spell]->range[1])
	{
		v_calc_cell(player->coord, t_vect(player->coord.x + i, player->coord.y), i - 1);
		v_calc_cell(player->coord, t_vect(player->coord.x - i, player->coord.y), i - 1);
		v_calc_cell(player->coord, t_vect(player->coord.x, player->coord.y + i), i - 1);
		v_calc_cell(player->coord, t_vect(player->coord.x, player->coord.y - i), i - 1);
		i++;
	}
	calculated = true;
}
