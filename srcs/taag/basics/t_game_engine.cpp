#include "taag.h"

void				s_game_engine::handle_control_camera(SDL_Event *event)
{
	board.handle_mouvement(event);
	board.handle_zoom(event);
}

void				s_game_engine::cast_spell(t_vect mouse)
{
	t_actor *player = turn_order[turn_index % turn_order.size()];
	if (board.get_cell(mouse) == NULL || board.get_cell(mouse)->node == NULL)
		return ;
	if (board.get_cell(mouse)->v_dist >= player->spell[s_spell]->range[0] &&
		board.get_cell(mouse)->v_dist <= player->spell[s_spell]->range[1] &&
		player->stat.pm.value >= player->spell[s_spell]->cost_pm &&
		player->stat.pa.value >= player->spell[s_spell]->cost_pa)
	{
		size_t i = 0;
		vector<t_vect>	target_list;
		if (player->spell[s_spell]->zone_type == Z_CROSS)
			target_list = calc_cross(player->spell[s_spell]->zone_size);
		if (player->spell[s_spell]->zone_type == Z_CROSS_LINE)
			target_list = calc_cross_line(player->spell[s_spell]->zone_size);
		if (player->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = player->coord - mouse;
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
								((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			target_list = calc_line(player->spell[s_spell]->zone_size, dir);
		}
		if (player->spell[s_spell]->zone_type == Z_SQUARE)
			target_list = calc_square(player->spell[s_spell]->zone_size);
		while (i < target_list.size())
		{
			size_t j = 0;
			while (j < player->spell[s_spell]->effect.size())
			{
				if (board.get_cell(mouse + target_list[i]))
					player->spell[s_spell]->effect[j].effect(player, board.get_cell(mouse + target_list[i])->actor, player->spell[s_spell]->effect[j].stat);
				j++;
			}
			i++;
		}
		vector<t_vect>	text_coord;
		if (player->spell[s_spell]->cost_pa > 0)
		{
			t_vect tmp = t_vect(player->coord.x, player->coord.y + (double)(player->visual_info.size()) / 2.0);
			board.get_cell(player->coord)->visual_info.push_back(create_visual_info("-" + to_string(player->spell[s_spell]->cost_pa) + "pa", BLUE, 10, tmp));
			player->stat.pa.value -= player->spell[s_spell]->cost_pa;
		}
		text_coord.clear();
		if (player->spell[s_spell]->cost_pm > 0)
		{
			t_vect tmp = t_vect(player->coord.x, player->coord.y + (double)(player->visual_info.size()) / 2.0);
			board.get_cell(player->coord)->visual_info.push_back(create_visual_info("-" + to_string(player->spell[s_spell]->cost_pm) + "pm", DARK_GREEN, 10, tmp));
			player->stat.pm.value -= player->spell[s_spell]->cost_pm;
		}
		s_spell = -1;
		calculated = false;
	}

}

void				s_game_engine::move_actor(t_vect dest)
{
	if (turn_order[turn_index % turn_order.size()]->destination.size() == 0 &&
		board.get_cell(dest) && board.get_cell(dest.x, dest.y)->m_dist <= turn_order[turn_index % turn_order.size()]->stat.pm.value)
	{
		t_actor *player = turn_order[turn_index % turn_order.size()];
		player->destination = pathfinding(board.get_mouse_pos());
		player->stat.pm.value -= board.get_cell(dest.x, dest.y)->m_dist;
		player->visual_info.push_back(create_visual_info("-" + to_string(board.get_cell(dest.x, dest.y)->m_dist) + "pm", DARK_GREEN, 10, player->coord));
		board.get_cell(dest)->actor = player;
		board.get_cell(player->coord.x, player->coord.y)->actor = NULL;
		board.reset_board();
	}
}
