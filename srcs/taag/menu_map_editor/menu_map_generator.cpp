#include "taag.h"

t_game_board		map_generator(int p_x, int p_y, t_node *p_node_type)
{
	t_game_board	board = t_game_board();
	int x;
	int y;

	y = 0;
	while (y < p_y)
	{
		x = 0;
		while (x < p_x)
		{
			if (board.board_size.x <= x)
			{
				board.cell_layer.resize(x + 1);
				if ((board.board_size).x < x + 1)
					(board.board_size).x = x + 1;
			}
			if (board.board_size.y <= y)
			{
				board.cell_layer[x].resize(y + 1);
				if ((board.board_size).y < y + 1)
					(board.board_size).y = y + 1;
			}
			board.cell_layer[x][y] = s_cell(t_vect(x, y), p_node_type);
			x++;
		}
		y++;
	}
	return (board);
}