#include "taag.h"

void menu_add_gambit(t_data data)
{
	t_actor		*actor = (t_actor *)(data.data[0]);
	size_t		value = (size_t &)(data.data[1]);
	if (value > actor->gambit.size())
		value = actor->gambit.size();
	actor->gambit.insert(actor->gambit.begin() + value, s_ai_helper({-1}));
}
