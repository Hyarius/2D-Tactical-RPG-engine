#include "taag.h"

void					generate_map_shop()
{
	ofstream		myfile;
	vector<string>		list = list_files("ressources/tmp/", ".map");

	for (size_t i = 0; i < list.size(); i++)
	{
		remove(("ressources/tmp/" + list[i] + ".map").c_str());
		myfile.open(SHOP_MAP_PATH + list[i] + SHOP_EXT);
		if (myfile.fail())
			error_exit();
		printf("%s\n", list[i].c_str());
		myfile << "string to add:" + list[i] + "\n";
		myfile << "tile:map_icons\n";
		myfile << "sprite:" + to_string(generate_nbr(0, 7)) + ":" + to_string(generate_nbr(0, 5)) + "\n";
		myfile << "price:" + to_string(generate_nbr(15, 100)) + "\n";
		myfile.close();
	}
}

void					generate_sprite_shop()
{
	ofstream		myfile;
	vector<string>		list = list_files("ressources/tmp/", ".tls");

	for (size_t i = 0; i < list.size(); i++)
	{
		remove(("ressources/tmp/" + list[i] + ".tls").c_str());
		myfile.open(SHOP_SPRITE_PATH + list[i] + SHOP_EXT);
		if (myfile.fail())
			error_exit();
		printf("%s\n", list[i].c_str());
		myfile << "string to add:" + list[i] + "\n";
		myfile << "tile:" + list[i] + "\n";
		myfile << "sprite:0:0\n";
		myfile << "price:" + to_string(generate_nbr(1, 30)) + "\n";
		myfile.close();
	}
}

void					generate_spell_shop()
{
	ofstream		myfile;
	vector<string>		list = list_files("ressources/tmp/", ".spell");

	for (size_t i = 0; i < list.size(); i++)
	{
		remove(("ressources/tmp/" + list[i] + ".spell").c_str());
		if (spell_map[spell_name[i + 1]].m_spell == 0)
		{
			myfile.open(SHOP_SPELL_PATH + list[i] + SHOP_EXT);
			if (myfile.fail())
				error_exit();
			printf("%s - %s\n", list[i].c_str(), spell_name[i + 1].c_str());
			myfile << "string to add:" + spell_name[i + 1] + "\n";
			myfile << "tile:simple_iconset\n";
			myfile << "sprite:" + to_string((int)(spell_map[spell_name[i + 1]].icon.x)) + ":" + to_string((int)(spell_map[spell_name[i + 1]].icon.y)) + "\n";
			myfile << "price:" + to_string(generate_nbr(10, 50)) + "\n";
			myfile.close();
		}
	}
}
