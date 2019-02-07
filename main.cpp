#include "taag.h"

/*
TODO :
faire un menu start, et un menu play
*/

#define TILESET_CHARSET_PATH "ressources/tileset/charset/"
#define TILESET_CHARSET_EXT ".tls"

int main(int argc, char **argv)
{
	(void)argv;
	if (argc != 1)
		return (0);

	window_initialisation(argv[0]);


	read_tileset();
	init_effects();
	read_spell();

	menu_start();

	return (0);
}
