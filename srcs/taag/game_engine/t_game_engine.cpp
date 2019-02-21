#include "taag.h"

void				s_game_engine::handle_control_camera(SDL_Event *event)
{
	board.handle_mouvement(event);
	board.handle_zoom(event);
}

bool				s_game_engine::cast_spell(t_vect mouse)
{
	t_actor *player = turn_order[turn_index % turn_order.size()];
	if (board.get_cell(mouse) == NULL || board.get_cell(mouse)->node == NULL ||
		(player->spell[s_spell]->on_target == 0 && board.get_cell(mouse)->actor == NULL) ||
		(player->spell[s_spell]->on_target == 2 && board.get_cell(mouse)->actor != NULL) ||
		player->cooldown[s_spell] != 0)
		return (false) ;
	if (board.get_cell(mouse)->v_dist >= player->spell[s_spell]->range[0] &&
		board.get_cell(mouse)->v_dist <= player->spell[s_spell]->range[1] &&
		player->stat.pm.value >= player->spell[s_spell]->cost_pm &&
		player->stat.pa.value >= player->spell[s_spell]->cost_pa)
	{
		vector<t_vect>	text_coord;
		if (player->spell[s_spell]->cost_pa > 0)
		{
			t_vect tmp = t_vect(player->coord.x, player->coord.y + (double)(player->visual_info->size() + board.get_cell(player->coord)->visual_info.size()) / 2.0);
			board.get_cell(player->coord)->visual_info.push_back(create_visual_info("-" + to_string(player->spell[s_spell]->cost_pa) + "pa", BLUE, 10, tmp));
			player->stat.pa.value -= player->spell[s_spell]->cost_pa;
		}
		text_coord.clear();
		if (player->spell[s_spell]->cost_pm > 0)
		{
			t_vect tmp = t_vect(player->coord.x, player->coord.y + (double)(player->visual_info->size() + board.get_cell(player->coord)->visual_info.size()) / 2.0);
			board.get_cell(player->coord)->visual_info.push_back(create_visual_info("-" + to_string(player->spell[s_spell]->cost_pm) + "pm", DARK_GREEN, 10, tmp));
			player->stat.pm.value -= player->spell[s_spell]->cost_pm;
		}
		size_t i = 0;
		vector<t_vect>	target_list;
		if (player->spell[s_spell]->zone_type == Z_DIAM)
			target_list = calc_diam(player->spell[s_spell]->zone_size);
		else if (player->spell[s_spell]->zone_type == Z_CROSS)
			target_list = calc_cross(player->spell[s_spell]->zone_size);
		else if (player->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = player->coord - mouse;
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
								((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			target_list = calc_line(player->spell[s_spell]->zone_size, dir);
		}
		else if (player->spell[s_spell]->zone_type == Z_SQUARE)
			target_list = calc_square(player->spell[s_spell]->zone_size);
		if (player->spell[s_spell]->anim_type == 0)
			board.get_cell(mouse)->animation.push_back(player->spell[s_spell]->target_anim);
		while (i < target_list.size())
		{
			size_t j = 0;
			while (j < player->spell[s_spell]->effect.size())
			{
				if (player->spell[s_spell]->effect[j].effect == move_caster)
					set_coord_target(mouse + target_list[i]);
				if (board.get_cell(mouse + target_list[i]) && player->spell[s_spell]->effect[j].effect != NULL)
				{
					player->spell[s_spell]->effect[j].effect(player, board.get_cell(mouse + target_list[i])->actor, player->spell[s_spell]->effect[j].stat);
					if (player->spell[s_spell]->anim_type == 1 || (player->spell[s_spell]->anim_type == 2 && board.get_cell(mouse + target_list[i])->actor != NULL))
						board.get_cell(mouse + target_list[i])->animation.push_back(player->spell[s_spell]->target_anim);
				}
				j++;
			}
			i++;
		}
		player->cooldown[s_spell] = player->spell[s_spell]->cooldown;
		check_alive();
		s_spell = -1;
		calculated = false;
	}
	return (true);
}

void				s_game_engine::move_actor(t_vect dest)
{
	if (board.get_cell(dest.x, dest.y) &&
		turn_order[turn_index % turn_order.size()]->destination.size() == 0 &&
		turn_order[turn_index % turn_order.size()]->stat.pm.value != 0 &&
		(board.get_cell(dest.x, dest.y)->m_dist != 999 ||
		(board.get_cell(dest.x, dest.y)->m_dist == 999 && board.get_cell(dest.x, dest.y)->actor != NULL)))
	{
		t_actor *player = turn_order[turn_index % turn_order.size()];
		player->destination = pathfinding(dest);
		if (player->destination.size() != 0)
		{
			t_vect final_dest = player->destination[player->destination.size() - 1];
			player->stat.pm.value -= board.get_cell(final_dest.x, final_dest.y)->m_dist;
	
			board.get_cell(player->coord)->visual_info.push_back(create_visual_info("-" + to_string(board.get_cell(final_dest.x, final_dest.y)->m_dist) + "pm", RED, 10, player->coord - t_vect(0, 0.5 * player->visual_info->size())));
			board.get_cell(final_dest)->actor = player;
			board.get_cell(player->coord.x, player->coord.y)->actor = NULL;
			player->visual_info = &(board.get_cell(final_dest)->visual_info);
			board.reset_board();
		}
	}
}
