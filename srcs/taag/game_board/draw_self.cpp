#include "taag.h"

void				s_game_board::draw_self()
{
	draw_board();
	draw_cell_layer();
	draw_placement();
	if (turn_order.size())
		draw_path();
	draw_actor_list();
	draw_cursor_layer();
	draw_mouse_cursor();
	//draw_cell_border();
	draw_animation();
	draw_visual_info();
}
