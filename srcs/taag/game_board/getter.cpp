#include "taag.h"

t_cell				*s_game_board::get_cell(int x, int y)
{
	if (x < 0 || x >= board_size.x || y < 0 || y >= board_size.y)
		return (NULL);
	if (cell_layer[x][y].node == NULL)
		return (NULL);
	return (&(cell_layer[x][y]));
}

t_cell				*s_game_board::get_cell(t_vect target)
{
	if ((int)(target.x) < 0 || (int)(target.x) >= board_size.x || (int)(target.y) < 0 || (int)(target.y) >= board_size.y)
		return (NULL);
	if (cell_layer[(int)(target.x)][(int)(target.y)].node == NULL)
		return (NULL);
	return (&(cell_layer[(int)(target.x)][(int)(target.y)]));
}

t_vect				s_game_board::get_mouse_pos()
{
	t_vect mouse = (get_mouse_coord() - offset) / (sprite_unit * zoom) - target;

	if (mouse.x < 0)
		return (t_vect(-1, -1));
	else if (mouse.x >= board_size.x)
		return (t_vect(-1, -1));
	if (mouse.y < 0)
		return (t_vect(-1, -1));
	else if (mouse.y >= board_size.y)
		return (t_vect(-1, -1));

	return (t_vect((int)(mouse.x), (int)(mouse.y)));
}
