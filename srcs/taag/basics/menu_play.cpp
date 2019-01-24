#include "taag.h"

void					menu_play(t_data data)
{
	(void)data;
	/*SDL_Event	event;
	bool		play = true;
	t_gui 		gui;

	while (play)
	{
		prepare_screen();

		gui.draw_self();

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(2, &gui, &play));
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click();
		}
		render_screen(true);
	}*/
	t_game_engine	game = s_game_engine("ressources/map/domolia.map");

	game.placement_phase();

	game.game_loop();
}
