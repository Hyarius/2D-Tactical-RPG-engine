#include "taag.h"

void				s_game_board::game_loop()
{
	SDL_Event	event;
	bool		play = true;

	initiate_turn_order();

	while (play)
	{
		prepare_screen();

		update_board();
		draw_self();
		draw_gui();

		if (enemy_list.size() == 0 || ally_list.size() == 0)
			ending_fight(&play);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(2, &gui, &play));
			handle_control_camera(&event);
			handle_control_game(&event);
		}
		render_screen(true);
	}
}
