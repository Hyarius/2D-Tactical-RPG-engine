#include "taag.h"

/*
TODO :
faire un menu start, et un menu play
*/

#define TILESET_CHARSET_PATH "ressources/tileset/charset/"
#define TILESET_CHARSET_EXT ".tls"

/*
tile_name:simple_actor
file:ressources/assets/charset/000-actor00.png
size:16:8
*/
int main(int argc, char **argv)
{
	if (argc != 1)
		return (0);

	generate_charset_tileset();

	/*window_initialisation(argv[0]);
	
	read_tileset();
	init_effects();
	read_spell();

	menu_start();*/

	/*t_game_engine	game = s_game_engine("ressources/map/domolia.map");

	game.placement_phase();

	game.game_loop();
	*/

	return (0);
}
