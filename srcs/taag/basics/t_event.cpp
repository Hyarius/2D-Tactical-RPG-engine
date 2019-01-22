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

void			init_effects()
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
}

void	swap_actor(t_actor *source, t_actor *target, t_effect_stat effect_stat)
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

void	move_caster(t_actor *source, t_actor *target, t_effect_stat effect_stat)
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

void	heal_caster(t_actor *source, t_actor *target, t_effect_stat effect_stat)
{
	(void)target;
	if (source != NULL)
	{
		int damage = (effect_stat.value[0] + source->stat.hp.value < source->stat.hp.max ? effect_stat.value[0] : source->stat.hp.max - source->stat.hp.value);
		source->stat.hp.value += damage;
		source->visual_info.push_back(create_visual_info("+" + to_string((int)(effect_stat.value[0])) + "hp", RED, 10, source->coord - t_vect(0, 0.5 * source->visual_info.size())));
	}
}

void	dmg_caster(t_actor *source, t_actor *target, t_effect_stat effect_stat)
{
	int damage = (effect_stat.value[0] < source->stat.hp.value ? effect_stat.value[0] : source->stat.hp.value);
	source->stat.hp.value -= damage;
	if (source->stat.hp.value != 0)
		source->visual_info.push_back(create_visual_info("-" + to_string((int)(effect_stat.value[0])) + "hp", RED, 10, source->coord - t_vect(0, 0.5 * source->visual_info.size())));
	else if (damage)
		game->board.get_cell(source->coord)->visual_info.push_back(create_visual_info("-" + to_string((int)(effect_stat.value[0])) + "hp", RED, 10, source->coord - t_vect(0, 0.5 * target->visual_info.size())));
}

void	push_actor(t_actor *source, t_actor *target, t_effect_stat effect_stat)
{
	if (target != NULL)
	{
		t_vect delta = t_vect(	source->coord.x > target->coord.x ? -1 : source->coord.x == target->coord.x ? 0 : 1,
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

void	pull_actor(t_actor *source, t_actor *target, t_effect_stat effect_stat)
{
	if (target != NULL)
	{
		t_vect delta = t_vect(	source->coord.x > target->coord.x ? 1 : source->coord.x == target->coord.x ? 0 : -1,
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

void deal_dmg(t_actor *source, t_actor *target, t_effect_stat effect_stat)
{
	vector<t_vect>	text_coord;
	(void)source;
	if (target != NULL)
	{
		int damage = (effect_stat.value[0] < target->stat.hp.value ? effect_stat.value[0] : target->stat.hp.value);
		target->stat.hp.value -= damage;
		if (target->stat.hp.value != 0)
			target->visual_info.push_back(create_visual_info("-" + to_string((int)(effect_stat.value[0])) + "hp", RED, 10, target->coord - t_vect(0, 0.5 * target->visual_info.size())));
		else if (damage)
			game->board.get_cell(target->coord)->visual_info.push_back(create_visual_info("-" + to_string((int)(effect_stat.value[0])) + "hp", RED, 10, target->coord - t_vect(0, 0.5 * target->visual_info.size())));
	}
}

void heal(t_actor *source, t_actor *target, t_effect_stat effect_stat)
{
	vector<t_vect>	text_coord;
	(void)source;
	if (target != NULL)
	{
		int damage = (effect_stat.value[0] + target->stat.hp.value < target->stat.hp.max ? effect_stat.value[0] : target->stat.hp.max - target->stat.hp.value);
		target->stat.hp.value += damage;
		target->visual_info.push_back(create_visual_info("+" + to_string((int)(effect_stat.value[0])) + "hp", RED, 10, target->coord - t_vect(0, 0.5 * target->visual_info.size())));
	}
}

void change_pm(t_actor *source, t_actor *target, t_effect_stat effect_stat)
{
	vector<t_vect>	text_coord;
	(void)source;
	if (target != NULL)
	{
		int damage = (effect_stat.value[0] < target->stat.pm.value ? effect_stat.value[0] : target->stat.pm.value);
		target->stat.pm.value += damage;
		if (target->stat.hp.value != 0)
			target->visual_info.push_back(create_visual_info(to_string((int)(effect_stat.value[0])) + "pm", RED, 10, target->coord - t_vect(0, 0.5 * target->visual_info.size())));
		else if (damage)
			game->board.get_cell(target->coord)->visual_info.push_back(create_visual_info(to_string((int)(effect_stat.value[0])) + "pm", RED, 10, target->coord - t_vect(0, 0.5 * target->visual_info.size())));
	}
}

void change_pa(t_actor *source, t_actor *target, t_effect_stat effect_stat)
{
	vector<t_vect>	text_coord;
	(void)source;
	if (target != NULL)
	{
		int damage = (effect_stat.value[0] < target->stat.pa.value ? effect_stat.value[0] : target->stat.pa.value);
		target->stat.pa.value += damage;
		if (target->stat.hp.value != 0)
			target->visual_info.push_back(create_visual_info(to_string((int)(effect_stat.value[0])) + "pa", RED, 10, target->coord - t_vect(0, 0.5 * target->visual_info.size())));
		else if (damage)
			game->board.get_cell(target->coord)->visual_info.push_back(create_visual_info(to_string((int)(effect_stat.value[0])) + "pa", RED, 10, target->coord - t_vect(0, 0.5 * target->visual_info.size())));
	}
}
