#include "taag.h"

vector<string>		list_file_delete_map;
string 				*text_list_delete_map[30];

static void	modify_index(t_data data)
{
	int *index = (int *)(data.data[0]);
	int delta = (int &)(data.data[1]);

	if (*index + delta >= -27 && *index + delta < (int)(((list_file_delete_map.size() / 3) * 3 + 3)))
		*index += delta;
	for (int i = 0; i < 30; i++)
	{
		if (*index + i >= (int)(list_file_delete_map.size()) || *index + i < 0)
			*(text_list_delete_map[i]) = "";
		else
			*(text_list_delete_map[i]) = list_file_delete_map[i + *index];
	}
}

static void		quit_delete(t_data data)// board / entry_path / play / num
{
	t_game_board	*board = (t_game_board *)(data.data[0]);
	string		*entry_path = (string *)(data.data[1]);
	bool 		*play = (bool *)(data.data[2]);
	int			i = (int &)(data.data[3]);

	if (*(text_list_delete_map[i]) != "")
	{
		remove((MAP_PATH + *(text_list_delete_map[i]) + MAP_EXT).c_str());
		*board = t_game_board();
		*entry_path = "";
		*play = true;
		list_file_delete_map = list_files(MAP_PATH, MAP_EXT);
		for (int i = 0; i < 30; i++)
			*(text_list_delete_map[i]) = (i < (int)(list_file_delete_map.size()) ? list_file_delete_map[i] : "");
	}
}

void			menu_delete_map(t_data data) // 0 - t_gui * / 1 - game_board * / 2 - &path
{
	t_gui		*old_gui = (t_gui *)(data.data[0]);
	t_game_board	*board = (t_game_board *)(data.data[1]);
	string		*entry_path = (string *)(data.data[2]);
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
						"Which file do you want to delete ?", BLACK,
						gui.unit * t_vect(1.5, 1.5), gui.unit * t_vect(27, 2), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						NULL, NULL));

	list_file_delete_map = list_files(MAP_PATH, MAP_EXT);
	int i = 0;
	while (i < 30)
	{
		t_button *button = new t_button(new t_text_button(
					(i < (int)(list_file_delete_map.size()) ? list_file_delete_map[i] : ""), BLACK,
					gui.unit * t_vect(1.5 + ((i % 3) * 8.3 + (i % 3)), 4 + ((i / 3) * 1.3)),
					gui.unit * t_vect(8.3, 1.1),
					8,
					t_color(0.4, 0.4, 0.4),
					t_color(0.6, 0.6, 0.6)),
					quit_delete, t_data(4, board, entry_path, &play, i));// board / entry_path / play / num
		text_list_delete_map[i] = &(button->button->text);
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
	(void)data;
	printf("menu_delete_map\n");
}
