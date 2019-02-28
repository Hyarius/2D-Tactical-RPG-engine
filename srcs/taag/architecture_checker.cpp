#include "taag.h"

vector<string>	to_test = {
		"ressources",
		"ressources/assets",
		"ressources/assets/charset",
		"ressources/assets/iconset",
		"ressources/assets/interface",
		"ressources/assets/tileset",
		"ressources/font",
		"ressources/game_object",
		"ressources/game_object/animation",
		"ressources/game_object/monster",
		"ressources/game_object/node",
		"ressources/game_object/actor",
		"ressources/game_object/game_engine",
		"ressources/map",
		"ressources/shader",
		"ressources/spell",
		"ressources/tileset",
		"ressources/tileset/animation",
		"ressources/tileset/charset",
		"ressources/tileset/interface",
		"ressources/tileset/tileset",
		"ressources/shop",
		"ressources/shop/map",
		"ressources/shop/spell",
		"ressources/shop/sprite",
};

string 					create_new_game_engine()
{
	string text = "level:0\n";
	text.append("exp:0\n");
	text.append("gold:0\n");
	text.append("tuto_state:0\n");
	text.append("char:NULL:NULL:NULL:NULL:NULL:NULL\n");
	text.append("spell:Aero-sword:Aqua-sword:Electro-sword:Fire-sword:Frozen-sword:Grounded-sword:Minor heal\n");
	text.append("tile:H00:H01:H11:H16\n");
	text.append("map:road-00\n");
	return (text);
}

bool					can_create_game_engine()
{
	return (true);
}

void 					check_prog_architecture()
{
	for (size_t i = 0; i < to_test.size(); i++)
	{
		DIR *dir = opendir(to_test[i].c_str());
		if (dir == NULL)
		{
			#ifdef __APPLE__
				mkdir(to_test[i].c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
			#elif defined _WIN32
			CreateDirectoryA(to_test[i].c_str(), NULL);
			#endif
		}
	}
	if (check_file_exist("ressources/game_object/game_engine/saved_game.eng") == false || can_create_game_engine() == false)
	{
		rewrite_on_file("ressources/game_object/game_engine/saved_game.eng", create_new_game_engine());
	}
}
