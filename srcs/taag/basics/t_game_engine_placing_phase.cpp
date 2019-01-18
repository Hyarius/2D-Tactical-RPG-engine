#include "taag.h"

void			s_game_engine::handle_actor_placement(SDL_Event *event)
{
	if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)
	{
		if (board.actor_pool.size())
		{
			invoke_actor(board.actor_pool[0], board.get_mouse_pos());
			board.actor_pool.erase(board.actor_pool.begin());
		}

	}
	if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_RIGHT)
	{
		t_vect mouse = board.get_mouse_pos();

		if (board.get_cell(mouse) && board.get_cell(mouse)->actor != NULL)
		{
			printf("here 2\n");
			board.actor_pool.insert(board.actor_pool.begin(), board.get_cell(mouse)->actor);
			outvoke_actor(board.get_cell(mouse)->actor);
		}
	}
}

void			s_game_engine::placement_phase()
{
	SDL_Event	event;
	bool		play = true;

	vector<t_vect>	placement = board.placement_list;
	printf("test\n");

	while (play)
	{
		prepare_screen();

		draw_board();

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
				exit(0);
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
				play = false;
			handle_control_camera(&event);
			handle_actor_placement(&event);
		}
		render_screen(true);
	}

}
