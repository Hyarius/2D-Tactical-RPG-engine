#include "taag.h"

void				s_game_board::update_board()
{
	static bool a = true;
	static int last_s_spell = -1;

	if (s_spell != last_s_spell)
	{
		last_s_spell = s_spell;
		a = true;
	}
	size_t i = 0;

	if (a == true)
		write_in_log("Checking character alive : ");
	check_alive();
	if (a == true)
		write_in_log("DONE\n");
	t_actor *player = turn_order[turn_index % turn_order.size()];
	if (a == true)
		write_in_log("Moving character and setting up them direction is needed : ");
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
	if (a == true)
		write_in_log("DONE\n");
	if (a == true)
		write_in_log("Calculation over ");
	if (player->team == 1)
	{
		if (turn_order.size() && player->destination.size() == 0 && calculated == false && s_spell == -1)
		{
			if (a == true)
				write_in_log("mouvement\n");
			calculate_distance(turn_order[turn_index % turn_order.size()]->coord);
		}
		else if (turn_order.size() && player->destination.size() == 0 && s_spell != -1 && calculated == false)
		{
			if (a == true)
				write_in_log("vision\n");
			if (player->spell[s_spell]->range_type == R_CIRCLE)
				calculate_vision_circle();
			else
				calculate_vision_line();
		}
		if (a == true)
			write_in_log("Calculation over zone\n");
		calculate_zone();
	}
	if (a == true)
		write_in_log("\n");
	if (a == true)
		a = false;
}
