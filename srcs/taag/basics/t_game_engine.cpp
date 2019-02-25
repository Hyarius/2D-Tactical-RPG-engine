#include "taag.h"

s_game_engine::s_game_engine()
{
	board = t_game_board();
	set_game_engine(&(this->board));
}

void		s_game_engine::start_game(string path)
{
	board = t_game_board(path);
	set_game_engine(&(this->board));
}
