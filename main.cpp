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

	t_game_engine	game = s_game_engine("ressources/map/map_test.map");

	printf("here\n");

	game.placement_phase();

	printf("here\n");

	game.game_loop();

	return (0);
}
