#include "taag.h"

/*
TODO :
Ajouter au game_engine l'interface graphique

faire le print des infos sur la case en haut a droite
*/

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
		game.draw_gui();

		render_screen(true);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
				exit(0);
			game.handle_control_camera(&event);
			game.handle_control_game(&event);
		}
	}
	return (0);
}
