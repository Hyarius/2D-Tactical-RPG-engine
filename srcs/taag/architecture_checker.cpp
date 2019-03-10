#include "taag.h"


string 					create_new_game_engine()
{
	string text = "level:1\n";
	text.append("exp:0\n");
	text.append("gold:0\n");
	text.append("tuto_state:0\n");
	text.append("keyboard:" + to_string(SDLK_q) + ":" + to_string(SDLK_w) + ":" + to_string(SDLK_e) + ":" + to_string(SDLK_r) + ":" + to_string(SDLK_t) + ":" + to_string(SDLK_y) + "\n");
	text.append("char:NULL:NULL:NULL:NULL:NULL:NULL\n");
	text.append("spell:Basic attack\n");
	text.append("tile:H00:H04:H05:H17\n");
	text.append("map:\n");
	return (text);
}

bool					can_create_game_engine()
{
	return (true);
}

void 					check_prog_architecture()
{
	if (check_file_exist(GAME_ENGINE_PATH + "saved_game" + GAME_ENGINE_EXT) == false || can_create_game_engine() == false)
	{
		rewrite_on_file(GAME_ENGINE_PATH + "saved_game" + GAME_ENGINE_EXT, create_new_game_engine());
	}
}
