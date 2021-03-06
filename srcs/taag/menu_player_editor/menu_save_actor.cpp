#include "taag.h"

static void			stand(t_data data)
{
	*((bool *)data.data[0]) = false;
}

static void			save_actor(t_data data) // 0 - t_actor * / 1 - file name
{
	t_actor *to_save = (t_actor *)(data.data[0]);
	string	*path = (string *)(data.data[2]);
	string name = (to_save->name == "" ? "default" : to_save->name);
	string p_path = name;

	remove((ACTOR_PATH + *path + ACTOR_EXT).c_str());

	if (name == "default" && check_file_exist(ACTOR_PATH + p_path + ACTOR_EXT) == true)
	{
		for (int i = 0; check_file_exist(ACTOR_PATH + p_path + ACTOR_EXT) == true; i++)
			p_path = name + "(" + to_string(i) + ")";
	}

	*path = p_path;

	size_t	*tile_index = (size_t *)(data.data[1]);

	ofstream myfile;
	myfile.open (ACTOR_PATH + p_path + ACTOR_EXT);
	myfile << "name:" + name + "\n";

	name = account->tile_unlock[*tile_index % account->tile_unlock.size()];

	myfile << "tileset:" + name + "\n";
	myfile << "sprite_pos:" + to_string((int)(to_save->sprite.x)) + ":" + to_string((int)(to_save->sprite.y)) + "\n";
	myfile << "health:" + to_string(to_save->stat.hp.max) + "\n";
	myfile << "action:" + to_string(to_save->stat.pa.max) + "\n";
	myfile << "mouvement:" + to_string(to_save->stat.pm.max) + "\n";
	myfile << "initiative:" + to_string(to_save->stat.init) + "\n";
	for (size_t i = 0; i < 6; i++)
	{
		myfile << "spell" + to_string(i) + ":" + to_save->spell[i]->name + "\n";
	}
	myfile.close();
}

static void		quit_save(t_data data)
{
	save_actor(t_data(3, data.data[0], data.data[2], data.data[3]));
	if (account->tuto_state < 30)
		account->tuto_state++;
	bool *play = (bool *)(data.data[1]);
	*play = false;
	bool *play_2 = (bool *)(data.data[4]);
	*play_2 = false;
}

void			menu_save_actor(t_data data) //0 - gui / 1 - t_actor * / 2 - & file name
{
	write_in_log("\n");
	write_in_log("\n");
	write_in_log(" --- Statement at menu_save_actor : ");
	printOpenGLError();
	printSDLError();
	write_in_log("Everything is settle properly\n");
	write_in_log("\n");

	if (account->tuto_state < TUTO_SIZE)
		account->tuto_state--;
	write_in_log("Setting data value : ");
	t_actor 	*to_save = (t_actor *)(data.data[1]);
	string		*path = (string *)(data.data[2]);
	size_t		*tile_index = *(size_t **)(data.data[3]);
	t_vect		*sprite_target = *(t_vect **)(data.data[4]);
	bool		*old_play = (bool *)(data.data[5]);
	t_gui		gui = t_gui(15, 10);
	SDL_Event	event;
	bool		play = true;
	write_in_log("Setting complete\n");

	write_in_log("Gui question button : ");

	s_button *button = new s_button(new s_text_button(//button did you wanna quit
						"Did you want to save this actor ?", DARK_GREY, //text info
						gui.unit * t_vect(4, 2), gui.unit * t_vect(7, 5), 8, //object info
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
						NULL, NULL);
	button->button->image_coord = button->button->image_coord - gui.unit * t_vect(0, 1);

	gui.add(button);
	write_in_log("Creation complete\n");

	write_in_log("Gui yes button : ");

	gui.add(new s_button(new s_text_button(//button yes
						"YES", DARK_GREY, //text info
						gui.unit * t_vect(4.25, 5.25), gui.unit * t_vect(3, 1.5), 8, //object info
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
						quit_save, t_data(5, data.data[1], &play, tile_index, path, old_play)));

	write_in_log("Creation complete\n");
	write_in_log("Gui no button : ");
	gui.add(new s_button(new s_text_button(//button no
						"NO", DARK_GREY, //text info
						gui.unit * t_vect(7.75, 5.25), gui.unit * t_vect(3, 1.5), 8, //object info
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
						stand, &play));
	write_in_log("Creation complete\n");

	write_in_log("\n");
	int a = 2;

	while (play == true)
	{
		if (a >= 2)
		{
			write_in_log("--- Starting menu_load_actor loop ---\n");
			write_in_log("Prepare screen : ");
		}
		prepare_screen();

		if (a >= 2)
		{
			write_in_log("DONE\n");
			write_in_log("Drawing gui : ");
		}

		if (data.data.size() != 0)
			(*((t_gui *)(data.data[0]))).draw_self();
		if ((size_t)(account->tuto_state) < gui_tutorial.size())
			gui_tutorial[account->tuto_state].draw_self();
		gui.draw_self();

		if (a >= 2)
		{
			write_in_log("DONE\n");
			write_in_log("Render screen : ");
		}

		render_screen(true);

		if (a >= 2)
		{
			write_in_log("DONE\n");
			write_in_log("Event loop : ");
			a--;
		}

		if (SDL_PollEvent(&event) == 1)
		{
			if (a >= 1)
			{
				write_in_log("DONE\n");
				write_in_log("Check SDL/OpenGL : ");
				printOpenGLError();
				printSDLError();
				write_in_log("No error\n");
				a--;
			}
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
				play = false;
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				gui.click(&event);
			}
		}
	}
}
