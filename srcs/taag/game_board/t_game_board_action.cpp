#include "taag.h"

void				s_game_board::handle_control_camera(SDL_Event *event)
{
	handle_mouvement(event);
	handle_zoom(event);
}

bool				s_game_board::cast_spell(t_vect mouse)
{
	t_actor *player = turn_order[turn_index % turn_order.size()];
	if (get_cell(mouse) == NULL || get_cell(mouse)->node == NULL ||
		(player->spell[s_spell]->on_target == 0 && get_cell(mouse)->actor == NULL) ||
		(player->spell[s_spell]->on_target == 2 && get_cell(mouse)->actor != NULL) ||
		player->cooldown[s_spell] != 0)
		return (false) ;
	if (get_cell(mouse)->v_dist >= player->spell[s_spell]->range[0] &&
		get_cell(mouse)->v_dist <= player->spell[s_spell]->range[1] &&
		player->stat.pm.value >= player->spell[s_spell]->cost_pm &&
		player->stat.pa.value >= player->spell[s_spell]->cost_pa)
	{
		turn_order[turn_index % turn_order.size()]->apply_effect(1);
		if (player->spell[s_spell]->cost_pa > 0)
			player->change_stat_pa(-(player->spell[s_spell]->cost_pa));
		if (player->spell[s_spell]->cost_pm > 0)
			player->change_stat_pm(-(player->spell[s_spell]->cost_pm));
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
			get_cell(mouse)->animation.push_back(player->spell[s_spell]->target_anim);
		while (i < target_list.size())
		{
			size_t j = 0;
			while (j < player->spell[s_spell]->effect.size())
			{
				if (player->spell[s_spell]->effect[j].effect == move_caster)
					set_coord_target(mouse + target_list[i]);
				if (get_cell(mouse + target_list[i]) && player->spell[s_spell]->effect[j].effect != NULL)
				{
					player->spell[s_spell]->effect[j].effect(player, get_cell(mouse + target_list[i])->actor, player->spell[s_spell]->effect[j].stat);
					if (player->spell[s_spell]->anim_type == 1 || (player->spell[s_spell]->anim_type == 2 && get_cell(mouse + target_list[i])->actor != NULL))
						get_cell(mouse + target_list[i])->animation.push_back(player->spell[s_spell]->target_anim);
				}
				j++;
			}
			i++;
		}
		player->cooldown[s_spell] = player->spell[s_spell]->cooldown;
		s_spell = -1;
		calculated = false;
	}
	return (true);
}

void				s_game_board::move_actor(t_actor *player, t_vect dest, double speed)
{
	if (get_cell(dest.x, dest.y) &&
		((get_cell(dest.x, dest.y)->m_dist != 999 ||
		(get_cell(dest.x, dest.y)->m_dist == 999 && get_cell(dest.x, dest.y)->actor != NULL)) || speed != 1))
	{
		player->destination = pathfinding(player, dest, speed);
		if (player->destination.size() != 0)
		{
			t_vect final_dest = player->destination[player->destination.size() - 1];
			if (speed == 1)
			{
				player->change_stat_pm(-get_cell(final_dest)->m_dist);
				player->apply_effect(2);
			}
			get_cell(final_dest)->actor = player;
			get_cell(player->coord)->actor = NULL;
			reset_board();

		}
	}
}
