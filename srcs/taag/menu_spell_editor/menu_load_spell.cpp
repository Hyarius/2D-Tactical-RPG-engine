#include "taag.h"

vector<string>		list_file_load_spell;
string 				*text_list_load_spell[30];

static void	modify_index(t_data data)
{
	int *index = (int *)(data.data[0]);
	int delta = (int &)(data.data[1]);

	if (*index + delta >= -27 && *index + delta < (int)(((list_file_load_spell.size() / 3) * 3 + 3)))
		*index += delta;
	for (int i = 0; i < 30; i++)
	{
		if (*index + i >= (int)(list_file_load_spell.size()) || *index + i < 0)
			*(text_list_load_spell[i]) = "";
		else
			*(text_list_load_spell[i]) = list_file_load_spell[i + *index];
	}
}

static void		quit_load(t_data data)// spell / entry_name / entry_path / pool / play / num
{
	t_spell		*spell = (t_spell *)(data.data[0]);
	string		*entry_name = (string *)(data.data[1]);
	string		*entry_path = (string *)(data.data[2]);
	bool 		*play = (bool *)(data.data[3]);
	t_vect		*sprite_target = (t_vect *)(data.data[4]);
	int			i = (int &)(data.data[5]);
	string		**text = (string **)(data.data[6]);

	if (*(text_list_load_spell[i]) != "")
	{
		*spell = read_one_spell(SPELL_PATH + *(text_list_load_spell[i]) + SPELL_EXT);
		*sprite_target = spell->icon;
		*entry_path = *(text_list_load_spell[i]);
		*entry_name = spell->name;
		*play = false;
		for (int i = 0; i < 6; i++)
		{
			int count = 0;

			while (count < (int)(g_effects.size()) && g_effects[count] != spell->effect[i].effect)
				count++;
			if (count == (int)(g_effects.size()))
				count = -1;
			else
				count++;
			*(text[i]) = (count != -1 ? list_effect_name[count] : list_effect_name[0]);
		}
	}
}

void			menu_load_spell(t_data data) // 0 - t_gui * / 1 - t_spell * / 2 - &name / 3 - &path / 4 - &sprite
{
	t_gui		*old_gui = (t_gui *)(data.data[0]);
	t_spell		*spell = (t_spell *)(data.data[1]);
	string		*entry_name = (string *)(data.data[2]);
	string		*entry_path = (string *)(data.data[3]);
	t_vect		*sprite_target = *(t_vect **)(data.data[4]);
	string		**text = (string **)(data.data[5]);
	t_gui		gui;
	bool		play = true;
	SDL_Event	event;
	int 		index = 0;

	gui.add(new t_button(new t_text_button(
						"", BLACK,
						gui.unit * t_vect(1, 1), gui.unit * t_vect(28, 18), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						NULL, NULL));

	gui.add(new t_button(new t_text_button(
						"Which file did you want to load ?", BLACK,
						gui.unit * t_vect(1.5, 1.5), gui.unit * t_vect(27, 2), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						NULL, NULL));

	list_file_load_spell = list_files(SPELL_PATH, SPELL_EXT);
	int i = 0;
	while (i < 30)
	{
		t_button *button = new t_button(new t_text_button(
					(i < (int)(list_file_load_spell.size()) ? list_file_load_spell[i] : ""), BLACK,
					gui.unit * t_vect(1.5 + ((i % 3) * 8.3 + (i % 3)), 4 + ((i / 3) * 1.3)),
					gui.unit * t_vect(8.3, 1.1),
					8,
					t_color(0.4, 0.4, 0.4),
					t_color(0.6, 0.6, 0.6)),
					quit_load, t_data(7, spell, entry_name, entry_path, &play, sprite_target, i, text));// spell / entry_name / entry_path / pool / play / num
		text_list_load_spell[i] = &(button->button->text);
		gui.add(button);
		i++;
	}

	while (play == true)
	{
		prepare_screen();

		if (old_gui != NULL)
			old_gui->draw_self();

		gui.draw_self();

		render_screen(true);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT)
				menu_quit(t_data(1, &gui));
			if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				play = false;
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click();
			else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
				gui.key_press(&event);
			else if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_UP))
			{
				modify_index(t_data(2, &index, -3));
			}
			else if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_DOWN))
			{
				modify_index(t_data(2, &index, 3));
			}
			else if (event.type == SDL_MOUSEWHEEL)
			{
				if (event.wheel.y > 0)
					modify_index(t_data(2, &index, -3));
				else if (event.wheel.y < 0)
					modify_index(t_data(2, &index, 3));
			}
		}
	}
}
