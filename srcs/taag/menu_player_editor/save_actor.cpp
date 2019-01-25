#include "taag.h"

t_actor			read_actor(t_data data)
{
	string	p_path = *((string *)(data.data[0]));
	return (read_actor(p_path));
}

void			save_actor(t_data data)
{
	t_actor *to_save = (t_actor *)(data.data[0]);
	string	p_path = ACTOR_PATH + *((string *)(data.data[1])) + ACTOR_EXT;
	ofstream myfile;
	myfile.open (p_path);
	myfile << "name:" + (to_save->name == "" ? "Default" : to_save->name) + "\n";
	map<string, t_tileset>::const_iterator i;
	string name;

	for (i = tileset_map.begin(); i != tileset_map.end(); ++i)
	{
		if (&(i->second) == to_save->tile)
		{
			name = i->first;
			break;
		}
	}

	myfile << "tileset: " + name + "\n";
	myfile << "sprite_pos: " + to_string(to_save->sprite.x) + ":" + to_string(to_save->sprite.y) + "\n";
	myfile << "health:" + to_string(to_save->stat.hp.max) + "\n";
	myfile << "action:" + to_string(to_save->stat.pa.max) + "\n";
	myfile << "mouvement:" + to_string(to_save->stat.pm.max) + "\n";
	myfile << "initiative:" + to_string(to_save->stat.init) + "\n";
	myfile << "spell1:" + to_save->spell[0]->name + "\n";
	myfile << "spell2:" + to_save->spell[1]->name + "\n";
	myfile << "spell3:" + to_save->spell[2]->name + "\n";
	myfile << "spell4:" + to_save->spell[3]->name + "\n";
	myfile << "spell5:" + to_save->spell[4]->name + "\n";
	myfile << "spell6:" + to_save->spell[5]->name + "\n";
	myfile.close();
}

static void		quit_save(t_data data)
{
	save_actor(t_data(2, data.data[0], data.data[1]));
	bool *continu = (bool *)(data.data[2]);
	*continu = true;
}

static void			stand(t_data data)
{
	*((bool *)data.data[0]) = false;
}

void			menu_save_actor(t_data data)
{
	string name = (*((string *)(data.data[2])) == "" ? "default" : *((string *)(data.data[2])) );
	string full_path = ACTOR_PATH + name + ACTOR_EXT;

	t_gui		gui;
	SDL_Event	event;

	bool		continu = false;


	s_button *button = new s_button(new s_text_button(//button did you wanna quit
						"Did you want to save this file ?", DARK_GREY, //text info
						gui.unit * t_vect(4, 2), gui.unit * t_vect(7, 5), 8, //object info
						t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
						NULL, NULL);
	button->button->coord[2] = button->button->coord[2] - (gui.unit * t_vect(0, 1));

	gui.add(button);

	s_button *button2 = new s_button(new s_text_button(//button did you wanna quit
						full_path, DARK_GREY, //text info
						gui.unit * t_vect(4, 2), gui.unit * t_vect(7, 5), 8, //object info
						t_color(0, 0, 0, 0), t_color(0, 0, 0, 0)),
						NULL, NULL);

	gui.add(button2);

	gui.add(new s_button(new s_text_button(//button yes
						"YES", DARK_GREY, //text info
						gui.unit * t_vect(4.25, 5.25), gui.unit * t_vect(3, 1.5), 8, //object info
						t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
						quit_save, t_data(3, data.data[1], &name, &continu)));

	gui.add(new s_button(new s_text_button(//button no
						"NO", DARK_GREY, //text info
						gui.unit * t_vect(7.75, 5.25), gui.unit * t_vect(3, 1.5), 8, //object info
						t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
						stand, &continu));

	while (continu == false)
	{
		prepare_screen();

		if (data.data.size() != 0)
			(*((t_gui *)(data.data[0]))).draw_self();
		gui.draw_self();

		render_screen(true);

		if (SDL_PollEvent(&(event)) == 1)
		{
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
				continu = true;
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click();
		}
	}
}
