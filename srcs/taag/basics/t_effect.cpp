#include "taag.h"

s_effect_stat::s_effect_stat()
{
	value[0] = -1;
	value[1] = -1;
	value[2] = -1;
	value[3] = -1;
}

s_effect_stat::s_effect_stat(double p_value0, double p_value1, double p_value2, double p_value3)
{
	value[0] = p_value0;
	value[1] = p_value1;
	value[2] = p_value2;
	value[3] = p_value3;
}

s_effect::s_effect()
{
	effect = NULL;
}

	s_effect::s_effect(event p_effect, double p_value0, double p_value1, double p_value2, double p_value3)
{
	effect = p_effect;
	stat = s_effect_stat(p_value0, p_value1, p_value2, p_value3);
}
