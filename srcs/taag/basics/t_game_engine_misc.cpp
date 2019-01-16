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

void				s_game_engine::delete_actor(t_actor *new_actor)
{
	size_t count = 0;
	while (count < turn_order.size() && new_actor != turn_order[count])
		count++;
	turn_order.erase(turn_order.begin() + count);
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
	initiate_turn_order();
}

void				s_game_engine::initiate_turn_order()
{
	turn_index = 0;

	size_t i = 0;
	while (i < board.actor_list.size())
	{
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
	if (turn_order.size() && turn_order[turn_index % turn_order.size()]->destination.size() == 0 && calculated == false && s_spell == -1)
		calculate_distance();
	if (turn_order.size() && turn_order[turn_index % turn_order.size()]->destination.size() == 0 && s_spell != -1 && calculated == false)
	{
		if (turn_order[turn_index % turn_order.size()]->spell[s_spell]->type == CIRCLE)
			calculate_vision_circle();
		else
			calculate_vision_line();
	}
}
