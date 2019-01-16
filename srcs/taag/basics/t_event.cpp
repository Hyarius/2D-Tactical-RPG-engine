#include "taag.h"

vector<event>		g_effects;

void			init_effects()
{
	g_effects.push_back(deal_dmg);
	g_effects.push_back(heal);
	g_effects.push_back(change_pm);
	g_effects.push_back(change_pa);
}

void deal_dmg(t_actor *source, t_actor *target, t_effect_stat effect_stat)
{
	vector<t_vect>	text_coord;
	(void)source;
	if (target != NULL)
	{
		int damage = (effect_stat.value[0] < target->stat.hp.value ? effect_stat.value[0] : target->stat.hp.value);
		target->stat.hp.value -= damage;
		target->visual_info.push_back(create_visual_info("-" + to_string(damage) + "hp", RED, 10, target->coord));
	}
}

void heal(t_actor *source, t_actor *target, t_effect_stat effect_stat)
{
	vector<t_vect>	text_coord;
	(void)source;
	if (target != NULL)
	{
		int damage = (effect_stat.value[0] < target->stat.hp.value ? effect_stat.value[0] : target->stat.hp.value);
		target->stat.hp.value += damage;
		target->visual_info.push_back(create_visual_info("+" + to_string(damage) + "hp", RED, 10, target->coord));
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
		target->visual_info.push_back(create_visual_info(to_string(damage) + "pm", DARK_GREEN, 10, target->coord));
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
		target->visual_info.push_back(create_visual_info(to_string(damage) + "pa", BLUE, 10, target->coord));
	}
}
