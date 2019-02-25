#include "taag.h"

void				s_game_board::add_actor(t_actor *new_actor)
{
	actor_list.push_back(new_actor);
}

void				s_game_board::remove_actor(t_actor *old_actor)
{
	size_t count = 0;

	if (old_actor->team == 2)
	{
		while (count < enemy_list.size() && old_actor != enemy_list[count])
			count++;
		if (count < enemy_list.size())
			enemy_list.erase(enemy_list.begin() + count);
	}
	else if (old_actor->team == 1)
	{
		while (count < ally_list.size() && old_actor != ally_list[count])
			count++;
		if (count < ally_list.size())
			ally_list.erase(ally_list.begin() + count);
	}
	count = 0;
	while (count < actor_list.size() && old_actor != actor_list[count])
		count++;
	if (count < actor_list.size())
	{
		actor_list.erase(actor_list.begin() + count);
	}
}

void				s_game_board::insert_actor(t_actor *new_actor)
{
	size_t count = 0;
	while (count < turn_order.size() && new_actor->stat.init < turn_order[count]->stat.init)
		count++;
	turn_order.insert(turn_order.begin() + count, new_actor);
}

void				s_game_board::invoke_actor(t_actor *new_actor, t_vect coord)
{
	if (get_cell(coord) && get_cell(coord)->actor == NULL)
	{
		new_actor->coord = coord;
		get_cell(coord)->actor = new_actor;
		add_actor(new_actor);
		if (new_actor->team == 1)
			ally_list.push_back(new_actor);
	}
}

void				s_game_board::outvoke_actor(t_actor *new_actor)
{
	if (get_cell(new_actor->coord) && get_cell(new_actor->coord)->actor != NULL)
	{
		get_cell(new_actor->coord)->actor = NULL;
		new_actor->coord = t_vect(-1, -1);
		remove_actor(new_actor);
		delete new_actor;
	}
}

void				s_game_board::delete_actor(t_actor *new_actor)
{
	remove_actor(new_actor);
	size_t count = 0;
	while (count < turn_order.size() && new_actor != turn_order[count])
		count++;
	if (turn_index >= count)
		turn_index--;
	if (count < turn_order.size())
		turn_order.erase(turn_order.begin() + count);
	get_cell(new_actor->coord)->actor = NULL;
	delete new_actor;
}
