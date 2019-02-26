#include "taag.h"

vector<string>			check_spell_lock()
{
	vector<string>		list_base = list_files(SPELL_PATH, SPELL_EXT);

	return (list_base);
}

void create_tab_spell(t_data data)
{
	t_gui *gui = (t_gui *)(data.data[0]);

	*gui = t_gui();
	vector<string>		spell_name_list = check_spell_lock();

	for (size_t i = 0; i < spell_name_list.size(); i++)
		printf("spell[%zu] = %s\n", i, spell_name_list[i].c_str());
}
