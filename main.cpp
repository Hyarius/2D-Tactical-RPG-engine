#include "taag.h"

/*
TODO :
struct game_engine qui prend le board, et qui controle le tour etc.

Faudra donc deplacer l'actor list du game_board au game_engine
*/

typedef struct			s_game_engine
{
	t_game_board		board;
	vector<t_actor *>	turn_order;
	size_t				turn_index;
						s_game_engine();
						s_game_engine(string p_path);
	void				draw_board();
	void				initiate_turn_order();
	void				next_turn();
	void				insert_actor(t_actor *new_actor);
	void				delete_actor(t_actor *old_actor);
	void				calc_cell(vector<t_vect> *to_calc, int i, int x, int j, int y);
	void				calculate_distance();
	vector<t_vect>		pathfinding(t_vect dest);
	void				move_actor(t_vect dest);
	void				update_board();
	void				handle_control_camera(SDL_Event *event);
	void				handle_control_game(SDL_Event *event);
}						t_game_engine;

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
	read_tileset();
	board = t_game_board(p_path);
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
	turn_order[turn_index % turn_order.size()]->selected = true;
	board.reset_board();
	calculate_distance();
}

void				s_game_engine::next_turn()
{
	turn_order[turn_index % turn_order.size()]->selected = false;
	turn_index = (turn_index + 1) % turn_order.size();
	turn_order[turn_index % turn_order.size()]->selected = true;
	turn_order[turn_index % turn_order.size()]->reset_value();
	calculate_distance();
}

void				s_game_engine::draw_board()
{
	board.draw_self();
}

void				s_game_engine::calc_cell(vector<t_vect> *to_calc, int i, int x, int j, int y)
{
	if (board.get_cell(i + x, j + y) != NULL && board.get_cell(i + x, j + y)->node->m_obs == false && board.get_cell(i + x, j + y)->actor == NULL &&
		board.get_cell(i + x, j + y)->m_dist > board.get_cell(i, j)->m_dist &&
		board.get_cell(i, j)->m_dist + board.get_cell(i, j)->node->cost <= turn_order[turn_index % turn_order.size()]->stat.pm.value)
	{
		board.get_cell(i + x, j + y)->m_dist = board.get_cell(i, j)->m_dist + board.get_cell(i, j)->node->cost;
		to_calc->push_back(t_vect(i + x, j + y));
	}
}

void				s_game_engine::calculate_distance()
{
	board.reset_board();
	t_actor 		*player = turn_order[turn_index % turn_order.size()];
	vector<t_vect>	to_calc;
	size_t i, j;

	to_calc.push_back(player->coord);

	i = 0;
	board.get_cell(to_calc[i].x, to_calc[i].y)->m_dist = 0;
	while (i < to_calc.size())
	{
		calc_cell(&to_calc, to_calc[i].x, 1, to_calc[i].y, 0);
		calc_cell(&to_calc, to_calc[i].x, -1, to_calc[i].y, 0);
		calc_cell(&to_calc, to_calc[i].x, 0, to_calc[i].y, 1);
		calc_cell(&to_calc, to_calc[i].x, 0, to_calc[i].y, -1);
		i++;
	}
	i = 1;
	while (i < to_calc.size())
	{
		board.get_cell(to_calc[i].x, to_calc[i].y)->cursor = t_vect(1, 2);
		i++;
	}

}

void				s_game_engine::handle_control_camera(SDL_Event *event)
{
	board.handle_mouvement(event);
	board.handle_zoom(event);
}

void				s_game_engine::handle_control_game(SDL_Event *event)
{
	if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_SPACE)
		next_turn();
	if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)
		move_actor(board.get_mouse_pos());
}

vector<t_vect>		s_game_engine::pathfinding(t_vect dest)
{
	vector<t_vect>	path;
	t_vect			actual = dest;
	t_vect			to_look = actual;
	t_vect			source = turn_order[turn_index % turn_order.size()]->coord;

	while (actual != source)
	{
		actual = to_look;
		if (board.get_cell(actual.x + 1, actual.y) && board.get_cell(actual.x + 1, actual.y)->m_dist < board.get_cell(actual.x, actual.y)->m_dist)
			to_look = t_vect(actual.x + 1, actual.y);
		if (board.get_cell(actual.x - 1, actual.y) && board.get_cell(actual.x - 1, actual.y)->m_dist < board.get_cell(actual.x, actual.y)->m_dist)
			to_look = t_vect(actual.x - 1, actual.y);
		if (board.get_cell(actual.x, actual.y + 1) && board.get_cell(actual.x, actual.y + 1)->m_dist < board.get_cell(actual.x, actual.y)->m_dist)
			to_look = t_vect(actual.x, actual.y + 1);
		if (board.get_cell(actual.x, actual.y - 1) && board.get_cell(actual.x, actual.y - 1)->m_dist < board.get_cell(actual.x, actual.y)->m_dist)
			to_look = t_vect(actual.x, actual.y - 1);
		int i = 0;
		t_vect delta = ((to_look - actual) / 15);
		while (i < 15)
		{
			path.insert(path.begin(), actual + delta * i);
			i++;
		}
	}
	return (path);
}

void				s_game_engine::move_actor(t_vect dest)
{
	if (turn_order[turn_index % turn_order.size()]->destination.size() == 0 &&
		board.get_cell(dest.x, dest.y) && board.get_cell(dest.x, dest.y)->m_dist <= turn_order[turn_index % turn_order.size()]->stat.pm.value)
	{
		turn_order[turn_index % turn_order.size()]->destination = pathfinding(board.get_mouse_pos());
		turn_order[turn_index % turn_order.size()]->stat.pm.value -= board.get_cell(dest.x, dest.y)->m_dist;
		board.get_cell(dest.x, dest.y)->actor = turn_order[turn_index % turn_order.size()];
		board.get_cell(turn_order[turn_index % turn_order.size()]->coord.x, turn_order[turn_index % turn_order.size()]->coord.y)->actor = NULL;
	}
}

void				s_game_engine::update_board()
{
	size_t i = 0;

	while (i < turn_order.size())
	{
		if (turn_order[i]->destination.size() != 0)
		{
			turn_order[i]->coord = turn_order[i]->destination[0];
			turn_order[i]->destination.erase(turn_order[i]->destination.begin());
		}
		i++;
	}
	if (turn_order[turn_index % turn_order.size()]->destination.size() == 0)
		calculate_distance();
}

int main(int argc, char **argv)
{
	SDL_Event event;

	window_initialisation("TAAG");

	t_game_engine	game = s_game_engine("ressources/map/map_test.map");

	while (1)
	{
		prepare_screen();

		game.update_board();
		game.draw_board();

		render_screen(true);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT)
				exit(0);
			game.handle_control_camera(&event);
			game.handle_control_game(&event);
		}
	}

	return (0);
}