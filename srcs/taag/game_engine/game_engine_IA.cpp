#include "taag.h"

t_actor				*s_game_engine::get_nearest_player()
{
	
}

void				s_game_engine::enemy_turn()
{
	printf("not player turn\n");

	next_turn();
}

void				s_game_engine::next_turn()
{
	if (turn_order.size())
	{
		turn_order[turn_index % turn_order.size()]->selected = false;
		turn_order[turn_index % turn_order.size()]->reset_value();
	}
	turn_index = (turn_index + 1) % turn_order.size();
	if (turn_order.size())
	{
		turn_order[turn_index % turn_order.size()]->selected = true;
	}
	calculated = false;
	s_spell = -1;
	if (turn_order[turn_index % turn_order.size()]->team != 1)
	{
		enemy_turn();
	}
}