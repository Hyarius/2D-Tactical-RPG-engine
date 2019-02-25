#include "taag.h"

void				s_game_board::placement_phase(vector<t_actor *> game_actor_list)
{
	bool		continu = true;
	SDL_Event	event;
	bool		play = true;
	int			index = 0;

	for (size_t i = 0; i < game_actor_list.size(); i++)
	{
		if (game_actor_list[i] != NULL)
			actor_pool.push_back(game_actor_list[i]);
	}

	while (play)
	{
		prepare_screen();

		draw_self();
		draw_cell_info_on_gui();
		draw_select_wheel(&index);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(3, &gui, &play, &continu));
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && actor_list.size() != enemy_list.size())
				play = false;
			handle_control_camera(&event);
			handle_actor_placement(&event, &index);
		}
		render_screen(true);
	}
	placement_list.clear();
	enemy_placement_list.clear();

	if (continu == true)
		game_loop();
}
