#include "taag.h"

void			s_game_engine::game_loop()
{
	SDL_Event	event;
	bool		play = true;

	initiate_turn_order();

	while (play)
	{
		prepare_screen();

		update_board();
		draw_board();
		draw_gui();

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
				play = false;
			handle_control_camera(&event);
			handle_control_game(&event);
		}
		render_screen(true);
	}
}
