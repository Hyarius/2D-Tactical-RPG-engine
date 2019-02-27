#include "taag.h"
vector<s_shop_item *>		map_item_vector;

void			modify_index_map_tab(t_data data)
{
	int *index = (int *)(data.data[0]);
	int delta = (int &)(data.data[1]);
	printf("here index map tab\n");
}

void				check_map_lock()
{
	vector<string>		list_base = list_files(MAP_PATH, MAP_EXT);

	map_item_vector.clear();

	for (size_t i = 0; i < list_base.size(); i++)
	{
		if (check_file_exist(SHOP_MAP_PATH + list_base[i] + SHOP_EXT) == true)
		{
			s_shop_item tmp = s_shop_item(SHOP_MAP_PATH + list_base[i] + SHOP_EXT);

			int find = 0;
			for (size_t j = 0; j < account->map_unlock.size(); j++)
			{
				if (account->map_unlock[j] == tmp.to_add)
					find = 1;
			}
			if (find == 0)
				map_item_vector.push_back(new t_shop_item(tmp));
		}
	}
}

void create_tab_map(t_data data)
{
	t_gui *gui = (t_gui *)(data.data[0]);
	int		*index = (int *)(data.data[1]);

	*gui = t_gui();
	check_map_lock();

	for (size_t i = 0; i < map_item_vector.size(); i++)
		printf("map[%zu] = %s\n", i, map_item_vector[i]->to_add.c_str());
}
