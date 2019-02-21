#include "taag.h"

s_ai_helper::s_ai_helper()
{
	value.resize(10);
	for (size_t i = 0; i < 10; i++)
		value[i] = 0;
}

s_ai_helper::s_ai_helper(vector<int> p_value)
{
	value.resize(10);
	size_t i = 0;
	while (i < 10)
	{
		if (i < p_value.size())
			value[i] = p_value[i];
		else
			value[i] = 0;
		i++;
	}
}

void			s_ai_helper::operator = (vector<int> data)
{
	size_t i = 1;
	while (i < data.size())
	{
		this->value[i] = data[i];
		i++;
	}
}
