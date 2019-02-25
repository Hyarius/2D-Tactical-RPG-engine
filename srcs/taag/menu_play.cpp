#include "taag.h"

void					menu_play(t_data data)
{
	t_game_engine *game = (t_game_engine *)(data.data[0]);

	string path = "";

	menu_choose_map(t_data(1, &path));

	if (path != "")
	{
		game->board = s_game_board(MAP_PATH + path + MAP_EXT);

		if (game->board.board_size.x >= 5 && game->board.board_size.y >= 5)
			game->board.placement_phase();
	}
}
