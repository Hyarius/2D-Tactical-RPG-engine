#include "taag.h"

vector<event>		g_effects;

t_game_engine		*game;
t_vect				coord;

void					set_coord_target(t_vect p_coord)
{
	coord = p_coord;
}

void			set_game_engine(t_game_engine *new_game)
{
	game = new_game;
}

void			init_actions()
{
	g_effects.push_back(deal_dmg); 		//0
	g_effects.push_back(heal);			//1
	g_effects.push_back(change_pm);		//2
	g_effects.push_back(change_pa);		//3
	g_effects.push_back(push_actor);	//4
	g_effects.push_back(pull_actor);	//5
	g_effects.push_back(heal_caster);	//6
	g_effects.push_back(dmg_caster);	//7
	g_effects.push_back(move_caster); 	//8
	g_effects.push_back(swap_actor);	//9
	g_effects.push_back(change_caster_pm);	//10
	g_effects.push_back(change_caster_pa);	//11
	g_effects.push_back(push_caster);
	g_effects.push_back(pull_caster);
	g_effects.push_back(apply_poison);
	g_effects.push_back(apply_regeneration);
	g_effects.push_back(apply_pa_change);
	g_effects.push_back(apply_pm_change);
	g_effects.push_back(cure_poison);
	g_effects.push_back(cure_regeneration);
	g_effects.push_back(cure_malus_pa);
	g_effects.push_back(cure_malus_pm);
	g_effects.push_back(cure_bonus_pa);
	g_effects.push_back(cure_bonus_pm);
}

void		deal_dmg(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	vector<t_vect>	text_coord;
	(void)source;
	if (target != NULL)
	{
		int damage = (effect_stat.value[0] < target->stat.hp.value ? effect_stat.value[0] : target->stat.hp.value);
		if (damage != 0)
			target->change_stat_hp(-damage);
	}
}

void		heal(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	vector<t_vect>	text_coord;
	(void)source;
	if (target != NULL)
	{
		int damage = (effect_stat.value[0] + target->stat.hp.value < target->stat.hp.max ? effect_stat.value[0] : target->stat.hp.max - target->stat.hp.value);
		if (damage != 0)
			target->change_stat_hp(damage);
	}
}

void		change_pm(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	vector<t_vect>	text_coord;
	(void)source;
	if (target != NULL)
	{
		int damage;
		if (target->stat.pm.value + effect_stat.value[0] < 0)
			damage = target->stat.pm.value;
		else
			damage = effect_stat.value[0];
		if (damage != 0)
			target->change_stat_pm(damage);
	}
}

void		change_pa(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	vector<t_vect>	text_coord;
	(void)source;
	if (target != NULL)
	{
		int damage;
		if (target->stat.pa.value + effect_stat.value[0] < 0)
			damage = target->stat.pa.value;
		else
			damage = effect_stat.value[0];
		if (damage != 0)
			target->change_stat_pa(damage);
	}
}

void		push_actor(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	if (target != NULL)
	{
		t_vect delta = t_vect(source->coord.x > target->coord.x ? -1 : source->coord.x == target->coord.x ? 0 : 1,
			source->coord.y > target->coord.y ? -1 : source->coord.y == target->coord.y ? 0 : 1);
		int i = 0;
		while (i < effect_stat.value[0])
		{
			if (game->board.get_cell(target->coord + delta) && game->board.get_cell(target->coord + delta)->node->m_obs == false && game->board.get_cell(target->coord + delta)->actor == NULL)
			{
				game->board.get_cell(target->coord)->actor = NULL;
				target->coord = target->coord + delta;
				game->board.get_cell(target->coord)->actor = target;
			}
			i++;
		}
	}
}

void		pull_actor(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	if (target != NULL)
	{
		t_vect delta = t_vect(source->coord.x > target->coord.x ? 1 : source->coord.x == target->coord.x ? 0 : -1,
			source->coord.y > target->coord.y ? 1 : source->coord.y == target->coord.y ? 0 : -1);
		int i = 0;
		while (i < effect_stat.value[0])
		{
			if (game->board.get_cell(target->coord + delta)->node->m_obs == false && game->board.get_cell(target->coord + delta)->actor == NULL)
			{
				game->board.get_cell(target->coord)->actor = NULL;
				target->coord = target->coord + delta;
				game->board.get_cell(target->coord)->actor = target;
			}
			i++;
		}
	}
}

void		heal_caster(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	(void)target;
	if (source != NULL)
	{
		int damage = (effect_stat.value[0] + source->stat.hp.value < source->stat.hp.max ? effect_stat.value[0] : source->stat.hp.max - source->stat.hp.value);
		if (damage != 0)
			source->change_stat_hp(damage);
	}
}

void		dmg_caster(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	(void)target;
	if (source != NULL)
	{
		int damage = (effect_stat.value[0] < source->stat.hp.value ? effect_stat.value[0] : source->stat.hp.value);
		if (damage != 0)
			source->change_stat_hp(-damage);
	}
}

void		move_caster(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	(void)effect_stat;
	if (target == NULL)
	{
		if (game->board.get_cell(coord)->node->m_obs == false)
		{
			game->board.get_cell(source->coord)->actor = NULL;
			source->coord = coord;
			game->board.get_cell(source->coord)->actor = source;
		}
	}
}

void		swap_actor(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	(void)effect_stat;
	t_vect tmp;
	if (target != NULL)
	{
		tmp = target->coord;
		target->coord = source->coord;
		source->coord = tmp;
		game->board.get_cell(target->coord)->actor = target;
		game->board.get_cell(source->coord)->actor = source;
	}
}

void		change_caster_pm(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	vector<t_vect>	text_coord;
	(void)target;
	if (source != NULL)
	{
		int damage;
		if (source->stat.pm.value + effect_stat.value[0] < 0)
			damage = source->stat.pm.value;
		else
			damage = effect_stat.value[0];
		if (damage != 0)
			source->change_stat_pm(damage);
	}
}

void		change_caster_pa(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	vector<t_vect>	text_coord;
	(void)target;
	if (source != NULL)
	{
		int damage;
		if (source->stat.pa.value + effect_stat.value[0] < 0)
			damage = source->stat.pa.value;
		else
			damage = effect_stat.value[0];
		if (damage != 0)
			source->change_stat_pa(damage);
	}
}

void		push_caster(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	if (target != NULL)
	{
		t_vect delta = t_vect(source->coord.x > target->coord.x ? -1 : source->coord.x == target->coord.x ? 0 : 1,
			source->coord.y > target->coord.y ? -1 : source->coord.y == target->coord.y ? 0 : 1);
		int i = 0;
		while (i < effect_stat.value[0])
		{
			if (game->board.get_cell(source->coord + delta) && game->board.get_cell(source->coord + delta)->node->m_obs == false && game->board.get_cell(source->coord + delta)->actor == NULL)
			{
				game->board.get_cell(source->coord)->actor = NULL;
				source->coord = source->coord + delta;
				game->board.get_cell(source->coord)->actor = target;
			}
			i++;
		}
	}
}

void		pull_caster(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	if (target != NULL)
	{
		t_vect delta = t_vect(source->coord.x > target->coord.x ? 1 : source->coord.x == target->coord.x ? 0 : -1,
			source->coord.y > target->coord.y ? 1 : source->coord.y == target->coord.y ? 0 : -1);
		int i = 0;
		while (i < effect_stat.value[0])
		{
			if (game->board.get_cell(source->coord + delta) && game->board.get_cell(source->coord + delta)->node->m_obs == false && game->board.get_cell(source->coord + delta)->actor == NULL)
			{
				game->board.get_cell(source->coord)->actor = NULL;
				source->coord = source->coord + delta;
				game->board.get_cell(source->coord)->actor = target;
			}
			i++;
		}
	}
}

void		apply_poison(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	(void)source;
	if (target != NULL)
	{
		t_effect new_effect = s_effect(effect_stat.value[0], { t_action(deal_dmg, effect_stat.value[1], 0, 0, effect_stat.value[2]) }, effect_stat.value[2]);
		target->effect_list.poison.push_back(new_effect);
		target->apply_effect("+ Poison");
	}
}

void		apply_regeneration(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	(void)source;
	if (target != NULL)
	{
		t_effect new_effect = s_effect(effect_stat.value[0], { t_action(heal, effect_stat.value[1], 0, 0, effect_stat.value[2]) }, effect_stat.value[2]);
		target->effect_list.regeneration.push_back(new_effect);
		target->apply_effect("+ Regeneration");
	}
}

void		apply_pa_change(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	(void)source;
	if (target != NULL)
	{
		t_effect new_effect = s_effect(effect_stat.value[0], { t_action(change_pa, effect_stat.value[1], 0, 0, effect_stat.value[2]) }, effect_stat.value[2]);
		target->effect_list.change_pa.push_back(new_effect);
		if (effect_stat.value[1] < 0)
			target->apply_effect("+ Malus PA");
		else
			target->apply_effect("+ Bonus PA");
	}
}

void		apply_pm_change(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	(void)source;
	if (target != NULL)
	{
		t_effect new_effect = s_effect(effect_stat.value[0], { t_action(change_pm, effect_stat.value[1], 0, 0, effect_stat.value[2]) }, effect_stat.value[2]);
		target->effect_list.change_pm.push_back(new_effect);
		if (effect_stat.value[1] < 0)
			target->apply_effect("+ Malus PM");
		else
			target->apply_effect("+ Bonus PM");

	}
}

void		cure_poison(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	(void)source;
	(void)effect_stat;
	if (target != NULL)
	{
		if (target->effect_list.poison.size() != 0)
			target->apply_effect("- Poison");
		target->effect_list.poison.clear();
	}
}

void		cure_regeneration(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	(void)source;
	(void)effect_stat;
	if (target != NULL)
	{
		if (target->effect_list.regeneration.size() != 0)
			target->apply_effect("- Regeneration");
		target->effect_list.regeneration.clear();
	}
}

void		cure_malus_pa(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	(void)source;
	(void)effect_stat;
	if (target != NULL)
	{
		if (target->effect_list.change_pa.size() != 0)
			target->apply_effect("- Malus PA");
		for (size_t i = 0; i < target->effect_list.change_pa.size(); i++)
		{
			if (target->effect_list.change_pa[i].action[0].stat.value[0] < 0)
			{
				target->effect_list.change_pa.erase(target->effect_list.change_pa.begin() + i);
				i--;
			}
		}
	}
}

void		cure_malus_pm(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	(void)source;
	(void)effect_stat;
	if (target != NULL)
	{
		if (target->effect_list.change_pm.size() != 0)
			target->apply_effect("- Malus PM");
		for (size_t i = 0; i < target->effect_list.change_pm.size(); i++)
		{
			if (target->effect_list.change_pm[i].action[0].stat.value[0] < 0)
			{
				target->effect_list.change_pm.erase(target->effect_list.change_pm.begin() + i);
				i--;
			}
		}
	}
}

void		cure_bonus_pa(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	(void)source;
	(void)effect_stat;

	if (target != NULL)
	{
		if (target->effect_list.change_pa.size() != 0)
			target->apply_effect("- Bonus PA");
		for (size_t i = 0; i < target->effect_list.change_pa.size(); i++)
		{
			if (target->effect_list.change_pa[i].action[0].stat.value[0] >= 0)
			{
				target->effect_list.change_pa.erase(target->effect_list.change_pa.begin() + i);
				i--;
			}
		}
	}
}

void		cure_bonus_pm(t_actor *source, t_actor *target, t_action_stat effect_stat)
{
	(void)source;
	(void)effect_stat;
	if (target != NULL)
	{
		if (target->effect_list.change_pm.size() != 0)
			target->apply_effect("- Bonus PM");
		for (size_t i = 0; i < target->effect_list.change_pm.size(); i++)
		{
			if (target->effect_list.change_pm[i].action[0].stat.value[0] >= 0)
			{
				target->effect_list.change_pm.erase(target->effect_list.change_pm.begin() + i);
				i--;
			}
		}
	}
}
