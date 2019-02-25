#include "taag.h"

int main(int argc, char **argv)
{
	(void)argv;
	if (argc != 1)
		return (0);


	//generate_charset_tileset();
	//generate_animation_tileset();
	//generate_node_file();

	//check_prog_architecture();

	bool play = true;

	if (play == true)
	{
		window_initialisation(argv[0]);
		read_tileset();
		init_actions();
		read_spell();

		menu_start();
	}

	return (0);
}
