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
						"Which map did you want to play ?", BLACK,
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

	while (play == true)
	{
		prepare_screen();

		gui.draw_self();

		render_screen(true);
		printOpenGLError();
		printSDLError();

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT)
				menu_quit(t_data(1, &gui));
			if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				play = false;
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click(&event);
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
