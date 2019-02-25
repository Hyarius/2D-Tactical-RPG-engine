#include "taag.h"

void				s_game_board::reset_board()
{
	int i = 0;

	while ((size_t)i < board_size.x)
	{
		int j = 0;
		while ((size_t)j < board_size.y)
		{
			cell_layer[i][j].m_dist = 999;
			cell_layer[i][j].v_dist = 999;
			cell_layer[i][j].cursor = t_vect(0, 0);
			j++;
		}
		i++;
	}
}

void				s_game_board::reset_vision(int min, int max)
{
	int x = 0;
	while (x < board_size.x)
	{
		int y = 0;
		while (y < board_size.y)
		{
			get_cell(x, y)->cursor = t_vect(0, 0);
			if (get_cell(x, y)->v_dist >= min && get_cell(x, y)->v_dist != 999 &&
				get_cell(x, y)->v_dist <= max)
				get_cell(x, y)->cursor = t_vect(0, 2);
			y++;
		}
		x++;
	}
}
