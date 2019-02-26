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
	bool *play = (bool *)(data.data[1]);
	*play = false;
}

void			menu_save_actor(t_data data) //0 - gui / 1 - t_actor * / 2 - & file name
{
	t_actor 	*to_save = (t_actor *)(data.data[1]);
	string		*path = (string *)(data.data[2]);
	size_t		*tile_index = *(size_t **)(data.data[3]);
	t_vect		*sprite_target = *(t_vect **)(data.data[4]);
	t_gui		gui = t_gui(15, 10);
	SDL_Event	event;

	bool		play = true;



	s_button *button = new s_button(new s_text_button(//button did you wanna quit
						"Did you want to save this actor ?", DARK_GREY, //text info
						gui.unit * t_vect(4, 2), gui.unit * t_vect(7, 5), 8, //object info
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
						NULL, NULL);
	button->button->image_coord = button->button->image_coord - gui.unit * t_vect(0, 1);

	gui.add(button);

	gui.add(new s_button(new s_text_button(//button yes
						"YES", DARK_GREY, //text info
						gui.unit * t_vect(4.25, 5.25), gui.unit * t_vect(3, 1.5), 8, //object info
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
						quit_save, t_data(4, data.data[1], &play, tile_index, path)));

	gui.add(new s_button(new s_text_button(//button no
						"NO", DARK_GREY, //text info
						gui.unit * t_vect(7.75, 5.25), gui.unit * t_vect(3, 1.5), 8, //object info
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
						stand, &play));

	while (play == true)
	{
		prepare_screen();

		if (data.data.size() != 0)
			(*((t_gui *)(data.data[0]))).draw_self();
		gui.draw_self();

		render_screen(true);

		if (SDL_PollEvent(&(event)) == 1)
		{
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
				play = false;
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click(&event);
		}
	}

}
