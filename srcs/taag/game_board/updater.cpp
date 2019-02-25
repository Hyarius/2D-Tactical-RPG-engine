#include "taag.h"

void				s_game_board::update_board()
{
	size_t i = 0;

	check_alive();
	t_actor *player = turn_order[turn_index % turn_order.size()];
	while (i < turn_order.size())
	{
		if (turn_order[i]->destination.size() != 0)
		{
			if (turn_order[i]->destination[0].x > turn_order[i]->coord.x && turn_order[i] == player)
				turn_order[i]->dir = 2;
			else if (turn_order[i]->destination[0].x < turn_order[i]->coord.x && turn_order[i] == player)
				turn_order[i]->dir = 1;
			else if (turn_order[i]->destination[0].y < turn_order[i]->coord.y && turn_order[i] == player)
				turn_order[i]->dir = 3;
			else if (turn_order[i]->destination[0].y > turn_order[i]->coord.y && turn_order[i] == player)
				turn_order[i]->dir = 0;
			turn_order[i]->coord = turn_order[i]->destination[0];
			turn_order[i]->destination.erase(turn_order[i]->destination.begin());
			calculated = false;
		}
		i++;
	}
	if (player->team == 1)
	{
		if (turn_order.size() && player->destination.size() == 0 && calculated == false && s_spell == -1)
			calculate_distance(turn_order[turn_index % turn_order.size()]->coord);
		if (turn_order.size() && player->destination.size() == 0 && s_spell != -1 && calculated == false)
		{
			if (player->spell[s_spell]->range_type == R_CIRCLE)
				calculate_vision_circle();
			else
				calculate_vision_line();
		}
		calculate_zone();
	}
}
