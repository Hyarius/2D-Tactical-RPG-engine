#include "taag.h"

void					menu_play(t_data data)
{
	(void)data;

	string path = "";

	menu_choose_map(t_data(1, &path));

	if (path != "")
	{
		t_game_engine	game = s_game_engine(MAP_PATH + path + MAP_EXT);

		if (game.board.board_size.x >= 5 && game.board.board_size.y >= 5)
			game.placement_phase();
	}
}
