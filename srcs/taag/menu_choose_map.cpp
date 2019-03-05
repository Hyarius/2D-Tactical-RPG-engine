#include "taag.h"

vector<string>		list_file_choose_map;
string 				*text_list_choose_map[30];

static void	modify_index(t_data data)
{
	int *index = (int *)(data.data[0]);
	int delta = (int &)(data.data[1]);

	if (*index + delta >= -27 && *index + delta < (int)(((list_file_choose_map.size() / 3) * 3 + 3)))
		*index += delta;
	for (int i = 0; i < 30; i++)
	{
		if (*index + i >= (int)(list_file_choose_map.size()) || *index + i < 0)
			*(text_list_choose_map[i]) = "";
		else
			*(text_list_choose_map[i]) = list_file_choose_map[i + *index];
	}
}

static void		quit_load(t_data data)// entry_path / play / num
{
	string		*entry_path = (string *)(data.data[0]);
	bool 		*play = (bool *)(data.data[1]);
	int			i = (int &)(data.data[2]);

	if (*(text_list_choose_map[i]) != "" && list_files(ACTOR_PATH, ACTOR_EXT).size() != 0)
	{
		*entry_path = *(text_list_choose_map[i]);
		*play = false;
	}
}

void			menu_choose_map(t_data data) // 0 - &path
{
	string		*entry_path = (string *)(data.data[0]);
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
						"Which map do you want to play ?", BLACK,
						gui.unit * t_vect(1.5, 1.5), gui.unit * t_vect(27, 2), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						NULL, NULL));

	list_file_choose_map = account->map_unlock;
	int i = 0;
	while (i < 30)
	{
		t_button *button = new t_button(new t_text_button(
					(i < (int)(list_file_choose_map.size()) ? list_file_choose_map[i] : ""), BLACK,
					gui.unit * t_vect(1.5 + ((i % 3) * 8.3 + (i % 3)), 4 + ((i / 3) * 1.3)),
					gui.unit * t_vect(8.3, 1.1),
					8,
					t_color(0.4, 0.4, 0.4),
					t_color(0.6, 0.6, 0.6)),
					quit_load, t_data(3, entry_path, &play, i));// entry_path / play / num
		text_list_choose_map[i] = &(button->button->text);
		gui.add(button);
		i++;
	}
	if (gui_tutorial[15].object_list.size() == 0)
	{
		gui_tutorial[15] = t_gui(30, 20);

		gui_tutorial[15].add(TUTORIAL_NUM, new s_tutorial_button(new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(1.5, 4) * gui.unit, t_vect(26.9, 5) * gui.unit, 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			NULL, NULL),
			new t_button(new s_paragraph_button(
				"Looks like we don't have any map to play with ... We should go to the shop and buy some ! \n \n \n Press the echap key to go back to the main menu", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(10.8, 9.3) * gui_tutorial[0].unit, t_vect(8.3, 4) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), tmp_function, NULL), false
		));
	}

	if (gui_tutorial[24].object_list.size() == 0)
	{
		gui_tutorial[24] = t_gui(30, 20);

		gui_tutorial[24].add(TUTORIAL_NUM, new s_tutorial_button(new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(1.5, 4) * gui.unit, t_vect(8.3, 1.1) * gui.unit, 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			quit_load, t_data(3, entry_path, &play, 0)),
			new t_button(new s_paragraph_button(
				"Select the \"tutorial-00\" map and let's play it !", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(10.8, 9.3) * gui_tutorial[0].unit, t_vect(8.3, 4) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), tmp_function, NULL), true
		));
	}

	while (play == true)
	{
		prepare_screen();

		gui.draw_self();
		if ((size_t)(account->tuto_state) < gui_tutorial.size())
			gui_tutorial[account->tuto_state].draw_self();

		render_screen(true);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT)
			{
				menu_quit(t_data(1, &gui));
			}
			else if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
			{
				play = false;
				if ((size_t)(account->tuto_state) < TUTO_SIZE)
					account->tuto_state = 15;
			}
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
			{
				if ((size_t)(account->tuto_state) < gui_tutorial.size() && gui_tutorial[account->tuto_state].object_list.size())
					gui_tutorial[account->tuto_state].click(&event);
				else
					gui.click(&event);
			}
			else if ((event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN) && (size_t)(account->tuto_state) >= gui_tutorial.size())
				gui.key_press(&event);
			else if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_UP) && (size_t)(account->tuto_state) >= gui_tutorial.size())
			{
				modify_index(t_data(2, &index, -3));
			}
			else if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_DOWN) && (size_t)(account->tuto_state) >= gui_tutorial.size())
			{
				modify_index(t_data(2, &index, 3));
			}
			else if (event.type == SDL_MOUSEWHEEL && (size_t)(account->tuto_state) >= gui_tutorial.size())
			{
				if (event.wheel.y > 0)
					modify_index(t_data(2, &index, -3));
				else if (event.wheel.y < 0)
					modify_index(t_data(2, &index, 3));
			}
		}
	}
	if (account->tuto_state == 15)
		account->tuto_state++;
}
