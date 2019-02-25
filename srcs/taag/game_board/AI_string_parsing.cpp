#include "taag.h"

string 				s_game_board::parse_effect_poison_desc(s_effect *effect)
{
	if (effect->effect_type == 0)
		return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " damage(s) when turn begin (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
	else if (effect->effect_type == 1)
		return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " damage(s) when attack (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
	else if (effect->effect_type == 2)
		return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " damage(s) when move (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
	return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " damage(s) never (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
}

string 				s_game_board::parse_effect_regeneration_desc(s_effect *effect)
{
	if (effect->effect_type == 0)
		return ("Effect : +" + to_string(effect->action[0].stat.value[0]) + " hp(s) when turn begin (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
	else if (effect->effect_type == 1)
		return ("Effect : +" + to_string(effect->action[0].stat.value[0]) + " hp(s) when attack (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
	else if (effect->effect_type == 2)
		return ("Effect : +" + to_string(effect->action[0].stat.value[0]) + " hp(s) when move (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
	return ("Effect : +" + to_string(effect->action[0].stat.value[0]) + " hp(s) never (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
}

string 				s_game_board::parse_effect_change_pm_desc(s_effect *effect)
{
	if (effect->action[0].stat.value[0] < 0)
	{
		if (effect->effect_type == 0)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PM(s) when turn begin (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		else if (effect->effect_type == 1)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PM(s) when attack (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		else if (effect->effect_type == 2)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PM(s) when move (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PM(s) never (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
	}
	else
	{
		if (effect->effect_type == 0)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PM(s) when turn begin (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		else if (effect->effect_type == 1)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PM(s) when attack (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		else if (effect->effect_type == 2)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PM(s) when move (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PM(s) never (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
	}
}

string 				s_game_board::parse_effect_change_pa_desc(s_effect *effect)
{
	if (effect->action[0].stat.value[0] < 0)
	{
		if (effect->effect_type == 0)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PA(s) when turn begin (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		else if (effect->effect_type == 1)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PA(s) when attack (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		else if (effect->effect_type == 2)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PA(s) when move (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PA(s) never (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
	}
	else
	{
		if (effect->effect_type == 0)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PA(s) when turn begin (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		else if (effect->effect_type == 1)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PA(s) when attack");
		else if (effect->effect_type == 2)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PA(s) when move");
		return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PA(s) never");
	}
}
