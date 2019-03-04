#include "taag.h"

vector<s_shop_item *>		tile_item_vector;
s_shop_item					*tile_item_list[18];

void				check_tile_lock()
{
	vector<string>		list_base = list_files(TILESET_CHARSET_PATH, TILESET_CHARSET_EXT);

	tile_item_vector.clear();

	for (size_t i = 0; i < list_base.size(); i++)
	{
		if (check_file_exist(SHOP_TILE_PATH + list_base[i] + SHOP_EXT) == true)
		{
			s_shop_item tmp = s_shop_item(SHOP_TILE_PATH + list_base[i] + SHOP_EXT);

			tile_item_vector.push_back(new t_shop_item(tmp));
		}
	}
}

void			modify_index_sprite_tab(t_data data)
{
	int *index = (int *)(data.data[0]);
	int delta = (int &)(data.data[1]);

	if (*index + delta >= -12 && *index + delta < ((int)(tile_item_vector.size()) / 6) * 6 + 6)
		*index += delta;
	for (int i = 0; i < 18; i++)
	{
		if (i + (*index) < 0 || i + *index >= (int)(tile_item_vector.size()))
			tile_item_list[i] = NULL;
		else
			tile_item_list[i] = tile_item_vector[i + (*index)];
	}
}

void		buy_tile(t_data data)
{
	s_shop_item **item = (s_shop_item **)(data.data[0]);
	int			*index = (int *)(data.data[1]);

	if (item == NULL || *item == NULL)
		return ;

	if (account->add_gold(-((*item)->price)) == true)
	{
		account->tile_unlock.push_back((*item)->to_add);
		sort(account->tile_unlock.begin(), account->tile_unlock.end());
		check_tile_lock();
		for (int i = 0; i < 18; i++)
		{
			if (i + (*index) < 0 || i + *index >= (int)(tile_item_vector.size()))
				tile_item_list[i] = NULL;
			else
				tile_item_list[i] = tile_item_vector[i + (*index)];
		}
		save_game_engine();
	}
}

void create_tab_sprite(t_data data)
{
	t_gui *gui = (t_gui *)(data.data[0]);
	int		*index = (int *)(data.data[1]);

	*gui = t_gui();
	check_tile_lock();

	for (int i = 0; i < 18; i++)
	{
		if (i + (*index) < 0 || i + *index >= (int)(tile_item_vector.size()))
			tile_item_list[i] = NULL;
		else
			tile_item_list[i] = tile_item_vector[i + (*index)];
	}
	t_vect size = t_vect(3.6, 5.2);
	t_vect icon_size = t_vect(3, 3);
	t_vect price_size = t_vect(1.5, 0.6);
	for (int i = 0; i < 18; i++)
	{
		t_vect coord = t_vect(1.25 + (i % 6) * size.x + (0.2 * (i % 6)), (2.4 + (i / 6) * size.y + (0.4 * (i / 6))));
		gui->add(new t_shop_tile(new t_button(new s_text_button(
				"", DARK_GREY,
				gui->unit * coord,
				gui->unit * size, 4,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
				buy_tile, t_data(2, &(tile_item_list[i]), index)),
				&(tile_item_list[i]), //tile_item_address
				gui->unit * (coord + t_vect((size.x - icon_size.x) / 2.0, 1)),	//icon coord
				gui->unit * icon_size,	//icon size
				gui->unit * (coord + (t_vect((size.x - price_size.x) / 2.0, size.y - price_size.y - 0.2))),	//price coord
				gui->unit * price_size,	//price size
				gui->unit * (coord + t_vect(3.6 / 2.0, 0.6)),
				gui->unit * t_vect(size.x - 0.5, price_size.y)));	//text_coord
	}

	gui->add(new t_button(new t_text_button(
						" up ", BLACK,
						gui->unit * t_vect(26, 3), gui->unit * t_vect(2, 5), 4,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						modify_index_sprite_tab, t_data(2, index, -6)));

	gui->add(new t_button(new t_text_button(
						"down", BLACK,
						gui->unit * t_vect(26, 13), gui->unit * t_vect(2, 5), 4,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						modify_index_sprite_tab, t_data(2, index, 6)));
}
