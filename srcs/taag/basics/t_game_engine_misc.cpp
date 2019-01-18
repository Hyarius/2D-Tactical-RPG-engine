#include "taag.h"

s_game_engine::s_game_engine()
{

}

void				s_game_engine::insert_actor(t_actor *new_actor)
{
	size_t count = 0;
	while (count < turn_order.size() && new_actor->stat.init < turn_order[count]->stat.init)
		count++;
	turn_order.insert(turn_order.begin() + count, new_actor);
}

void				s_game_engine::invoke_actor(t_actor *new_actor, t_vect coord)
{
	if (board.get_cell(coord) && board.get_cell(coord)->actor == NULL)
	{
		new_actor->coord = coord;
		board.get_cell(coord)->actor = new_actor;
		board.add_actor(new_actor);
	}
}

void				s_game_engine::outvoke_actor(t_actor *new_actor)
{
	if (board.get_cell(new_actor->coord) && board.get_cell(new_actor->coord)->actor != NULL)
	{
		board.get_cell(new_actor->coord)->actor = NULL;
		new_actor->coord = t_vect(-1, -1);
		board.remove_actor(new_actor);
	}
}

void				s_game_engine::delete_actor(t_actor *new_actor)
{
	size_t count = 0;
	while (count < board.actor_list.size() && new_actor != board.actor_list[count])
		count++;
	if (count < board.actor_list.size())
		board.actor_list.erase(board.actor_list.begin() + count);
	count = 0;
	while (count < turn_order.size() && new_actor != turn_order[count])
		count++;
	if (count < turn_order.size())
		turn_order.erase(turn_order.begin() + count);
	board.get_cell(new_actor->coord)->actor = NULL;
	delete new_actor;
}

void				s_game_engine::check_alive()
{
	size_t i = 0;
	while (i < board.actor_list.size())
	{
		if (board.actor_list[i]->stat.hp.value <= 0)
			delete_actor(board.actor_list[i]);
		else
			i++;
	}
}

static void			change_s_spell(t_data data)
{
	int *s_spell = (int *)(data.data[0]);
	bool	*calculated = (bool *)(data.data[1]);
	int value = ((int &)(data.data[2]));

	*calculated = false;
	*s_spell = value;
}

s_game_engine::s_game_engine(string p_path)
{
	calculated = false;
	s_spell = -1;
	read_tileset();
	init_effects();
	read_spell();
	board = t_game_board(p_path);
	gui = t_gui(30, 20);
	gui.add(new s_button(new t_image_button(t_image("ressources/assets/interface/GUI_Shortcut.png"), t_vect(0, 0), gui.unit * t_vect(30, 20)), NULL, NULL));
	gui.add(new s_button(new t_text_button("", BLACK, gui.unit * t_vect(22, 0), gui.unit * t_vect(8, 5), 4, t_color(0.3, 0.3, 0.3, 0.6), t_color(0.6, 0.6, 0.6, 0.6)), NULL, NULL));
	for (int i = 0; i < 6; i++)
		gui.add(SPELL_BUTTON, new s_button(new t_text_button("", BLACK, gui.unit * t_vect((i < 3 ? 8 : 18) + ((i % 3) * 1.5), 18.5), gui.unit, 0, t_color(0.3, 0.3, 0.3, 0.0), t_color(0.6, 0.6, 0.6, 0.0)), change_s_spell, t_data(3, &s_spell, &calculated, i)));
}

void				s_game_engine::initiate_turn_order()
{
	turn_index = 0;

	size_t i = 0;
	while (i < board.actor_list.size())
	{
		if (board.actor_list[i]->team >= 1 && board.actor_list[i]->team < 4)
			insert_actor(board.actor_list[i]);
		i++;
	}
	if (turn_order.size())
		turn_order[turn_index % turn_order.size()]->selected = true;
	board.reset_board();
	if (turn_order.size())
		calculate_distance();
}

void				s_game_engine::next_turn()
{
	if (turn_order.size())
		turn_order[turn_index % turn_order.size()]->selected = false;
	turn_index = (turn_index + 1) % turn_order.size();
	if (turn_order.size())
		turn_order[turn_index % turn_order.size()]->selected = true;
	if (turn_order.size())
		turn_order[turn_index % turn_order.size()]->reset_value();
	calculated = false;
	s_spell = -1;
}

void				s_game_engine::update_board()
{
	size_t i = 0;

	t_actor *player = turn_order[turn_index % turn_order.size()];
	while (i < turn_order.size())
	{
		if (turn_order[i]->destination.size() != 0)
		{
			if (turn_order[i]->destination[0].x > turn_order[i]->coord.x)
				turn_order[i]->dir = 1;
			else if (turn_order[i]->destination[0].x < turn_order[i]->coord.x)
				turn_order[i]->dir = 3;
			else if (turn_order[i]->destination[0].y < turn_order[i]->coord.y)
				turn_order[i]->dir = 2;
			else if (turn_order[i]->destination[0].y > turn_order[i]->coord.y)
				turn_order[i]->dir = 0;
			turn_order[i]->coord = turn_order[i]->destination[0];
			turn_order[i]->destination.erase(turn_order[i]->destination.begin());
			calculated = false;
		}
		i++;
	}
	if (turn_order.size() && player->destination.size() == 0 && calculated == false && s_spell == -1)
		calculate_distance();
	if (turn_order.size() && player->destination.size() == 0 && s_spell != -1 && calculated == false)
	{
		if (player->spell[s_spell]->range_type == R_CIRCLE)
			calculate_vision_circle();
		else
			calculate_vision_line();
	}
	if (player->destination.size() == 0 && s_spell != -1)
		calculate_zone();
}
