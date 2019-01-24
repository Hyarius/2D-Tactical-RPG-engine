#include "taag.h"

/*
TODO :
faire un menu start, et un menu play
*/

int main(int argc, char **argv)
{
	SDL_Event event;

	if (argc != 1)
		return (0);

	window_initialisation(argv[0]);

	read_tileset();
	init_effects();
	read_spell();

	menu_start();

	/*t_game_engine	game = s_game_engine("ressources/map/domolia.map");

	game.placement_phase();

	game.game_loop();
	*/

	return (0);
}
