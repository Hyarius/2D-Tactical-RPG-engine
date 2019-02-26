#include "taag.h"

s_shop_item::s_shop_item()
{
	to_add = "NULL";
	tile = NULL;
	sprite = t_vect(-1, -1);
	price = 0;
}

s_shop_item::s_shop_item(string path)
{
	ifstream		myfile;
	vector<string>	tab;
	string			tile_name;

	myfile.open(path);
	if (myfile.fail())
		printf("can't open such file : %s\n", path.c_str());
	to_add = get_strsplit(&myfile, ":", 2)[1];
	tile = get_interface_tile(get_strsplit(&myfile, ":", 2)[1]);
	tab = get_strsplit(&myfile, ":", 3);
	sprite = t_vect(atoi(tab[1].c_str()), atoi(tab[2].c_str()));
	price = atoi(get_strsplit(&myfile, ":", 2)[1].c_str());
}

s_shop_item::s_shop_item(string p_to_add, t_tileset *p_tile, t_vect p_sprite, int p_price)
{
	to_add = p_to_add;
	tile = p_tile;
	sprite = p_sprite;
	price = p_price;
}
