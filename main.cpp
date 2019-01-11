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
	void				handle_control_camera(SDL_Event *event);
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
}

void				s_game_engine::next_turn()
{
	turn_order[turn_index % turn_order.size()]->selected = false;
	turn_index = (turn_index + 1) % turn_order.size();
	turn_order[turn_index % turn_order.size()]->selected = true;
}

void				s_game_engine::draw_board()
{
	board.draw_self();
}

void				s_game_engine::handle_control_camera(SDL_Event *event)
{
	board.handle_mouvement(event);
	board.handle_zoom(event);
}

int main(int argc, char **argv)
{
	SDL_Event event;

	window_initialisation("TAAG");

	t_game_engine	game = s_game_engine("ressources/map/map_test.map");

	while (1)
	{
		prepare_screen();

		game.draw_board();

		render_screen(true);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT)
				exit(0);
			game.handle_control_camera(&event);
		}
	}

	return (0);
}
