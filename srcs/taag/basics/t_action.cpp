#include "taag.h"

s_action_stat::s_action_stat()
{
	value[0] = 0;
	value[1] = 0;
	value[2] = 0;
	value[3] = 0;
}

s_action_stat::s_action_stat(int p_value0, int p_value1, int p_value2, int p_value3)
{
	value[0] = p_value0;
	value[1] = p_value1;
	value[2] = p_value2;
	value[3] = p_value3;
}

s_action::s_action()
{
	effect = NULL;
	stat = s_action_stat();
}

	s_action::s_action(event p_effect, int p_value0, int p_value1, int p_value2, int p_value3)
{
	effect = p_effect;
	stat = s_action_stat(p_value0, p_value1, p_value2, p_value3);
}
