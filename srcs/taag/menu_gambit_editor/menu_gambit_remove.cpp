#include "taag.h"

void menu_remove_gambit(t_data data)
{
	t_actor		*actor = (t_actor *)(data.data[0]);
	size_t		value = (size_t &)(data.data[1]);
	t_gui		*gui_right = (t_gui *)(data.data[2]);

	if (actor->gambit.size() == 0 || value > actor->gambit.size())
		return ;
	*gui_right = t_gui();
	actor->gambit.erase(actor->gambit.begin() + value);
}
