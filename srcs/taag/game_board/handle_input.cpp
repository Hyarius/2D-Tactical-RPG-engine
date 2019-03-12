#include "taag.h"

extern bool	bool_checker[];
extern bool draw_self_bool;

void				s_game_board::handle_mouvement(SDL_Event *event)
{
	if (event->type == SDL_MOUSEMOTION && event->button.button == SDL_BUTTON_MIDDLE)
	{
		target.x += (double)(event->motion.xrel) / (sprite_unit.x * zoom);
		target.y += (double)(event->motion.yrel) / (sprite_unit.y * zoom);
		SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
	}
	else
		SDL_EventState(SDL_MOUSEBUTTONUP, SDL_ENABLE);
}

void				s_game_board::handle_zoom(SDL_Event *event)
{
	if (event->type == SDL_MOUSEWHEEL)
	{
		if (event->wheel.y > 0 && zoom < 10)
			zoom *= 1.1;
		else if (event->wheel.y < 0 && zoom >= 1)
			zoom *= 0.9;
	}
}

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
		player->apply_effect(1);
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
		int		find[6] = {0, 0, 0, 0, 0, 0};
		while (i < target_list.size())
		{
			size_t j = 0;
			while (j < player->spell[s_spell]->effect.size())
			{
				if (get_cell(mouse + target_list[i]))
				{
					t_actor *target = get_cell(mouse + target_list[i])->actor;
					if (target == NULL && (player->spell[s_spell]->effect[j].effect == pull_caster || player->spell[s_spell]->effect[j].effect == push_caster))
					{
						target = new t_actor();
						target->coord = mouse + target_list[i];
					}
					if (player->spell[s_spell]->effect[j].effect == move_caster)
						set_coord_target(mouse + target_list[i]);
					if (player->spell[s_spell]->effect[j].effect != NULL && (player->spell[s_spell]->effect[j].stat.value[3] == 0 || player->spell[s_spell]->effect[j].stat.value[3] > find[j]))
					{
						find[j]++;
						player->spell[s_spell]->effect[j].effect(player, target, player->spell[s_spell]->effect[j].stat);
						if (player->spell[s_spell]->anim_type == 1 || (player->spell[s_spell]->anim_type == 2 && target != NULL))
							get_cell(mouse + target_list[i])->animation.push_back(player->spell[s_spell]->target_anim);
					}
				}
				j++;
			}
			i++;
		}
		player->cooldown[s_spell] = player->spell[s_spell]->cooldown;
		player->spell_used[s_spell]++;
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

void				s_game_board::handle_actor_placement(SDL_Event *event, int *index, vector<t_actor *> *game_actor_list)
{
	t_vect 			mouse = get_mouse_pos();

	if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)
	{
		if (actor_pool.size() && get_cell(mouse) && get_cell(mouse)->actor == NULL)
		{
			size_t count = 0;
			while (count < placement_list.size() && mouse != placement_list[count])
				count++;
			if (count < placement_list.size())
			{
				invoke_actor((*game_actor_list)[(*index % game_actor_list->size())], get_mouse_pos());
				game_actor_list->erase(game_actor_list->begin() + *index % actor_pool.size());
			}
		}
	}
	else if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_RIGHT)
	{

		if (get_cell(mouse) && get_cell(mouse) && get_cell(mouse)->actor != NULL)
		{
			size_t count = 0;
			while (count < placement_list.size() && mouse != placement_list[count])
				count++;
			if (count < placement_list.size())
			{
				if (game_actor_list->size() == 0)
					game_actor_list->insert(game_actor_list->begin(), get_cell(mouse)->actor);
				else
					game_actor_list->insert(game_actor_list->end(), get_cell(mouse)->actor);
				outvoke_actor(get_cell(mouse)->actor);
			}
		}
	}
	else if (event->type == SDL_KEYUP)
	{
		if (event->key.keysym.sym == SDLK_UP)
			(*index)--;
		else if (event->key.keysym.sym == SDLK_DOWN)
			(*index)++;
		if (event->key.keysym.sym == SDLK_F1)
		{
			bool_checker[0] = (bool_checker[0] == true ? false : true);
			draw_self_bool = true;
		}
		else if (event->key.keysym.sym == SDLK_F2)
		{
			bool_checker[1] = (bool_checker[1] == true ? false : true);
			draw_self_bool = true;
		}
		else if (event->key.keysym.sym == SDLK_F3)
		{
			bool_checker[2] = (bool_checker[2] == true ? false : true);
			draw_self_bool = true;
		}
		else if (event->key.keysym.sym == SDLK_F4)
		{
			bool_checker[3] = (bool_checker[3] == true ? false : true);
			draw_self_bool = true;
		}
		else if (event->key.keysym.sym == SDLK_F5)
		{
			bool_checker[4] = (bool_checker[4] == true ? false : true);
			draw_self_bool = true;
		}
		else if (event->key.keysym.sym == SDLK_F6)
		{
			bool_checker[5] = (bool_checker[5] == true ? false : true);
			draw_self_bool = true;
		}
		else if (event->key.keysym.sym == SDLK_F7)
		{
			bool_checker[6] = (bool_checker[6] == true ? false : true);
			draw_self_bool = true;
		}
		else if (event->key.keysym.sym == SDLK_F8)
		{
			bool_checker[7] = (bool_checker[7] == true ? false : true);
			draw_self_bool = true;
		}
		else if (event->key.keysym.sym == SDLK_F9)
		{
			bool_checker[8] = (bool_checker[8] == true ? false : true);
			draw_self_bool = true;
		}
	}
}

void				s_game_board::handle_control_game(SDL_Event *event)
{
	t_actor *player = turn_order[turn_index % turn_order.size()];
	if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_SPACE)
		next_turn();
	else if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)
	{
		if (gui.click(event) == false)
		{
			if (s_spell == -1)
			{
				if (check_anim() == false || check_visual() == false)
					return ;
				move_actor(player, get_mouse_pos(), 1.0);
			}
			else
				cast_spell(get_mouse_pos());
			check_alive();
		}
	}
	if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_RIGHT && s_spell != -1)
	{
		s_spell = -1;
		calculated = false;
	}
	if (event->type == SDL_KEYUP)
	{
		if (event->key.keysym.sym == SDLK_F1)
		{
			bool_checker[0] = (bool_checker[0] == true ? false : true);
			draw_self_bool = true;
		}
		else if (event->key.keysym.sym == SDLK_F2)
		{
			bool_checker[1] = (bool_checker[1] == true ? false : true);
			draw_self_bool = true;
		}
		else if (event->key.keysym.sym == SDLK_F3)
		{
			bool_checker[2] = (bool_checker[2] == true ? false : true);
			draw_self_bool = true;
		}
		else if (event->key.keysym.sym == SDLK_F4)
		{
			bool_checker[3] = (bool_checker[3] == true ? false : true);
			draw_self_bool = true;
		}
		else if (event->key.keysym.sym == SDLK_F5)
		{
			bool_checker[4] = (bool_checker[4] == true ? false : true);
			draw_self_bool = true;
		}
		else if (event->key.keysym.sym == SDLK_F6)
		{
			bool_checker[5] = (bool_checker[5] == true ? false : true);
			draw_self_bool = true;
		}
		else if (event->key.keysym.sym == SDLK_F7)
		{
			bool_checker[6] = (bool_checker[6] == true ? false : true);
			draw_self_bool = true;
		}
		else if (event->key.keysym.sym == SDLK_F8)
		{
			bool_checker[7] = (bool_checker[7] == true ? false : true);
			draw_self_bool = true;
		}
		else if (event->key.keysym.sym == SDLK_F9)
		{
			bool_checker[8] = (bool_checker[8] == true ? false : true);
			draw_self_bool = true;
		}
		if (event->key.keysym.sym == account->keyboard[0])
		{
			if (player->stat.pa.value >= player->spell[0]->cost_pa && player->stat.pm.value >= player->spell[0]->cost_pm &&
				player->cooldown[0] == 0)
				s_spell = 0;
			else
				s_spell = -1;
		}
		else if (event->key.keysym.sym == account->keyboard[1])
		{
			if (player->stat.pa.value >= player->spell[1]->cost_pa && player->stat.pm.value >= player->spell[1]->cost_pm &&
				player->cooldown[1] == 0)
				s_spell = 1;
			else
				s_spell = -1;
		}
		else if (event->key.keysym.sym == account->keyboard[2])
		{
			if (player->stat.pa.value >= player->spell[2]->cost_pa && player->stat.pm.value >= player->spell[2]->cost_pm &&
				player->cooldown[2] == 0)
				s_spell = 2;
			else
				s_spell = -1;
		}
		else if (event->key.keysym.sym == account->keyboard[3])
		{
			if (player->stat.pa.value >= player->spell[3]->cost_pa && player->stat.pm.value >= player->spell[3]->cost_pm &&
				player->cooldown[3] == 0)
				s_spell = 3;
			else
				s_spell = -1;
		}
		else if (event->key.keysym.sym == account->keyboard[4])
		{
			if (player->stat.pa.value >= player->spell[4]->cost_pa && player->stat.pm.value >= player->spell[4]->cost_pm &&
				player->cooldown[4] == 0)
				s_spell = 4;
			else
				s_spell = -1;
		}
		else if (event->key.keysym.sym == account->keyboard[5])
		{
			if (player->stat.pa.value >= player->spell[5]->cost_pa && player->stat.pm.value >= player->spell[0]->cost_pm &&
				player->cooldown[5] == 0)
				s_spell = 5;
			else
				s_spell = -1;
		}
		calculated = false;
	}
}
