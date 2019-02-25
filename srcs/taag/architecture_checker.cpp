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
};

vector<string>	file_to_detect = {
	"ressources/game_object/game_engine/saved_game.eng",
};

string 					create_new_game_engine()
{
	string text = "level:0\n";
	text.append("exp:0\n");
	text.append("gold:0\n");
	text.append("char:NULL:NULL:NULL:NULL:NULL:NULL\n");
	text.append("spell availible:aero-sword:aqua-sword:electro-sword:fire-sword:frozen-sword:grounded-sword:minor heal\n");
	text.append("tile availible:H00:H01:H11:H16\n");
	return (text);
}

void 					check_prog_architecture()
{
	for (size_t i = 0; i < to_test.size(); i++)
	{
		DIR *dir = opendir(to_test[i].c_str());
		if (dir == NULL)
			mkdir(to_test[i].c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
	}
	for (size_t i = 0; i < file_to_detect.size(); i++)
	{
		if (check_file_exist(file_to_detect[i]) == false)
		{
			write_on_file(file_to_detect[i], create_new_game_engine());
		}
	}
}
