#include "taag.h"

s_stat::s_stat()
{

}

s_stat::s_stat(t_value p_hp, t_value p_pa, t_value p_pm, int p_init)
{
	hp = p_hp;
	pa = p_pa;
	pm = p_pm;
	init = p_init;
}

s_value::s_value()
{
	value = -1;
	max = -1;
}

s_value::s_value(int p_max)
{
	value = p_max;
	max = p_max;
}

s_value::s_value(int p_value, int p_max)
{
	value = p_value;
	max = p_max;
}
