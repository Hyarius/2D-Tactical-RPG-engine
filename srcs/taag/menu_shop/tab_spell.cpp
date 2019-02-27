#include "taag.h"

t_spell						*spell_list_shop[18];
s_shop_item					*spell_item_list[18];
vector<s_shop_item *>		item_list;

vector<s_shop_item *>			check_spell_lock()
{
	vector<string>		list_base = list_files(SPELL_PATH, SPELL_EXT);

	vector<s_shop_item *>	item_list;

	for (size_t i = 0; i < list_base.size(); i++)
	{
		if (check_file_exist(SHOP_SPELL_PATH + list_base[i] + SHOP_EXT) == true)
		{
			s_shop_item tmp = s_shop_item(SHOP_SPELL_PATH + list_base[i] + SHOP_EXT);

			int find = 0;
			for (size_t j = 0; j < account->spell_unlock.size(); j++)
			{
				if (account->spell_unlock[j] == tmp.to_add)
					find = 1;
			}
			if (find == 0)
				item_list.push_back(new t_shop_item(tmp));
		}
	}

	return (item_list);
}

void			modify_index_spell_tab(t_data data)
{
	int *index = (int *)(data.data[0]);
	int delta = (int &)(data.data[1]);

	if (*index + delta >= -12 && *index + delta < ((int)(item_list.size()) / 6) * 6 + 6)
		*index += delta;
	for (int i = 0; i < 18; i++)
	{
		if (i + (*index) < 0 || i + *index >= (int)(item_list.size()))
		{
			spell_list_shop[i] = &(spell_map["NULL"]);
			spell_item_list[i] = NULL;
		}
		else
		{
			spell_list_shop[i] = &(spell_map[item_list[i + *index]->to_add]);
			spell_item_list[i] = item_list[i + (*index)];
		}
	}
}


void create_tab_spell(t_data data)
{
	t_gui *gui = (t_gui *)(data.data[0]);
	int		*index = (int *)(data.data[1]);

	*gui = t_gui();
	item_list = check_spell_lock();

	for (int i = 0; i < 18; i++)
	{
		if (i + (*index) < 0 || i + *index >= (int)(item_list.size()))
		{
			spell_list_shop[i] = &(spell_map["NULL"]);
			spell_item_list[i] = NULL;
		}
		else
		{
			spell_list_shop[i] = &(spell_map[item_list[i + *index]->to_add]);
			spell_item_list[i] = item_list[i + (*index)];
		}
	}

	int i = 0;

	t_vect size = t_vect(3.6, 5.2);
	for (int i = 0; i < 18; i++)
	{
		gui->add(new s_shop_spell(new t_spell_card(&(spell_list_shop[i]),
							gui->unit * t_vect(1.25 + (i % 6) * size.x + (0.2 * (i % 6)), (2.4 + (i / 6) * size.y + (0.4 * (i / 6)))),
							gui->unit * size,
							NULL, NULL),
							&(spell_item_list[i]), gui->unit * t_vect(1.3, -0.2), gui->unit * t_vect(size.x - 1.5, 0.6)));
	}

	gui->add(new t_button(new t_text_button(
						" up ", BLACK,
						gui->unit * t_vect(26, 3), gui->unit * t_vect(2, 5), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						modify_index_spell_tab, t_data(2, index, -6)));

	gui->add(new t_button(new t_text_button(
						"down", BLACK,
						gui->unit * t_vect(26, 13), gui->unit * t_vect(2, 5), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						modify_index_spell_tab, t_data(3, index, 6, &gui)));
}
