#include "taag.h"

s_effect::s_effect()
{
	effect_type = -1;
	action.resize(0);
	duration = 0;
}

s_effect::s_effect(int p_effect_type, vector<t_action> p_action, int p_duration)
{
	effect_type = p_effect_type;
	action = p_action;
	duration = p_duration;
}

s_effect_list::s_effect_list()
{
	poison.resize(0);
	regeneration.resize(0);
};