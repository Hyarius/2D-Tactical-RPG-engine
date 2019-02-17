#include "taag.h"

void			s_game_engine::handle_actor_placement(SDL_Event *event, int *index)
{
	t_vect 			mouse = board.get_mouse_pos();

	if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)
	{
		if (actor_pool.size())
		{
			size_t count = 0;
			while (count < board.placement_list.size() && mouse != board.placement_list[count])
				count++;
			if (count < board.placement_list.size())
			{
				invoke_actor(new t_actor(*(actor_pool[*index % actor_pool.size()])), board.get_mouse_pos());
			}
		}
	}
	else if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_RIGHT)
	{

		if (board.get_cell(mouse) && board.get_cell(mouse)->actor != NULL)
		{
			size_t count = 0;
			while (count < board.placement_list.size() && mouse != board.placement_list[count])
				count++;
			if (count < board.placement_list.size())
				outvoke_actor(board.get_cell(mouse)->actor);
		}
	}
	else if (event->type == SDL_KEYDOWN)
	{
		if (event->key.keysym.sym == SDLK_UP)
			(*index)--;
		else if (event->key.keysym.sym == SDLK_DOWN)
			(*index)++;
	}
}

void			s_game_engine::placement_phase()
{
	bool		continu = true;
	SDL_Event	event;
	bool		play = true;
	int			index = 0;

	vector<string>	actor_file = list_files(ACTOR_PATH, ACTOR_EXT);
	for (size_t i = 0; i < actor_file.size(); i++)
	{
		t_actor *new_actor = new t_actor(read_actor(ACTOR_PATH + actor_file[i] + ACTOR_EXT));
		new_actor->team = 1;
		actor_pool.push_back(new_actor);
	}

	while (play)
	{
		prepare_screen();

		draw_board();
		draw_cell_info_on_gui();
		draw_select_wheel(&index);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(3, &gui, &play, &continu));
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
				play = false;
			handle_control_camera(&event);
			handle_actor_placement(&event, &index);
		}
		render_screen(true);
	}
	board.placement_list.clear();
	board.enemy_placement_list.clear();

	if (continu == true)
		game_loop();
}
