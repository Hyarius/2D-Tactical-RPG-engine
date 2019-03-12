#include "taag.h"

string user_profile;
string app_data;
size_t len;

int main(int argc, char **argv)
{
	if (getenv("USERPROFILE") != NULL)
		user_profile = string(getenv("USERPROFILE")) + "/documents/";
	else
		user_profile = "";
	len = user_profile.size();
	for (size_t i = 0; i < len; i++)
	{
		if (user_profile[i] == '\\')
			user_profile[i] = '/' ;
	}
	if (getenv("APPDATA") != NULL)
		app_data = string(getenv("APPDATA")) + "/";
	else
		app_data = "";
	len = app_data.size();
	for (size_t i = 0; i < len; i++)
	{
		if (app_data[i] == '\\')
			app_data[i] = '/';
	}
	(void)argv;
	if (argc != 1)
		return (0);

	//generate_charset_tileset();
	//generate_animation_tileset();
	//generate_node_file();
	//generate_map_shop();

	check_prog_architecture();

	charge_log_file();

	write_in_log("user_profile = " + user_profile + "\n");
	write_in_log("app_data = " + app_data + "\n");

	bool play = true;

	if (play == true)
	{
		write_in_log("\n");
		write_in_log("Window inistialisation:\n");
		window_initialisation(argv[0]);
		write_in_log("\n");

		write_in_log("Read tileset:\n");
		read_tileset();
		write_in_log("\n");

		write_in_log("Init action:\n");
		init_actions();
		write_in_log("\n");

		write_in_log("Read spell:\n");
		read_spell();

		//generate_sprite_shop();
		//generate_spell_shop();

		menu_start();
	}

	return 0;
}
