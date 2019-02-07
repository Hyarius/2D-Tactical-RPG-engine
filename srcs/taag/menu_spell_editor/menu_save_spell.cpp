#include "taag.h"

static void			stand(t_data data)
{
	*((bool *)data.data[0]) = false;
}

static void			save_spell(t_data data) // 0 - t_spell * / 1 - file name
{
	t_spell *spell = (t_spell *)(data.data[0]);
	string	p_path = SPELL_PATH + *((string *)(data.data[1])) + SPELL_EXT;
	ofstream myfile;
	myfile.open (p_path);
	myfile << "name:" + (spell->name == "" ? "default" : spell->name) + "\n";
	myfile << "description:" + (spell->desc == "" ? "default" : spell->desc) + "\n";
	myfile << "icon:simple_iconset\n";
	myfile << "sprite:" + to_string((int)(spell->icon.x)) + ":" + to_string((int)(spell->icon.y)) + "\n";
	myfile << "m_spell:" + to_string((spell->m_spell == false ? 0 : 1)) + "\n";
	myfile << "pa:" + to_string(spell->cost_pa) + "\n";
	myfile << "pm:" + to_string(spell->cost_pm) + "\n";
	myfile << "range:" + to_string(spell->range[0]) + ":" + to_string(spell->range[1]) + ":" + to_string((spell->block == false ? 0 : 1)) + "\n";
	myfile << "type:" + to_string((int)(spell->range_type)) + "\n";
	myfile << "on target:" + to_string(spell->on_target) + "\n";
	myfile << "zone:" + to_string((int)(spell->zone_type)) + ":" + to_string(spell->zone_size) + "\n";
	for (int i = 0; i < 6; i++)
	{
		if (spell->effect[i].effect != NULL)
		{
			size_t k = 0;
			while (k < g_effects.size() && spell->effect[i].effect != g_effects[k])
				k++;
			if (k != g_effects.size())
				myfile << "effect" + to_string(i + 1) + ":" + to_string(k) + ":" +
					to_string(spell->effect[i].stat.value[0]) + ":" +
					to_string(spell->effect[i].stat.value[1]) + ":" +
					to_string(spell->effect[i].stat.value[2]) + ":" +
					to_string(spell->effect[i].stat.value[3]) + "\n";
		}
		else
			myfile << "effect" + to_string(i + 1) + ":0:0:0:0:0\n";
	}
	myfile.close();
}

static void		quit_save(t_data data)
{
	save_spell(t_data(2, data.data[0], data.data[1]));
	bool *play = (bool *)(data.data[2]);
	*play = false;
}

void			menu_save_spell(t_data data) //0 - gui / 1 - t_spell * / 2 - & file name
{
	string name = (*((string *)(data.data[2])) == "" ? "default" : *((string *)(data.data[2])) );
	string full_path = SPELL_PATH + name + SPELL_EXT;

	t_gui		gui = t_gui(15, 10);
	SDL_Event	event;

	bool		play = true;


	s_button *button = new s_button(new s_text_button(//button did you wanna quit
						"Did you want to save this file ?", DARK_GREY, //text info
						gui.unit * t_vect(4, 2), gui.unit * t_vect(7, 5), 8, //object info
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
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
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
						quit_save, t_data(3, data.data[1], &name, &play)));

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
				gui.click();
		}
	}

}
