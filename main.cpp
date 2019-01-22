#include "taag.h"

/*
TODO :
Ajouter au game_engine l'interface graphique

faire le print des infos sur la case en haut a droite
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

	t_game_engine	game = s_game_engine("ressources/map/map_test.map");

	game.board = board_generator(20, 20);

	game.placement_phase();

	game.game_loop();

	return (0);
}
