#include "taag.h"

vector<string>		list_file_place_monster;
string 				*text_list_place_monster[30];

static void			modify_index(t_data data)
{
	int *index = (int *)(data.data[0]);
	int delta = (int &)(data.data[1]);

	if (*index + delta >= -27 && *index + delta < (int)(((list_file_place_monster.size() / 3) * 3 + 3)))
		*index += delta;
	for (int i = 0; i < 30; i++)
	{
		if (*index + i >= (int)(list_file_place_monster.size()) || *index + i < 0)
			*(text_list_place_monster[i]) = "";
		else
			*(text_list_place_monster[i]) = list_file_place_monster[i + *index];
	}
}

static void			quit_place(t_data data)
{
	t_game_board	*board = (t_game_board *)(data.data[0]);
	t_vect			*coord = (t_vect *)(data.data[1]);
	bool 			*play = (bool *)(data.data[2]);
	int				i = (int &)(data.data[3]);

	if (*(text_list_place_monster[i]) != "")
	{
		t_actor *monster = new t_actor(read_actor(MONSTER_PATH + *(text_list_place_monster[i]) + ACTOR_EXT));
		monster->path = *(text_list_place_monster[i]);
		monster->team = 2;
		board->invoke_actor(monster, *coord);
		*play = false;
	}
}

static void			start_monster_editor(t_data data)
{
	int *index = (int *)(data.data[0]);
	menu_monster_editor(data);
	list_file_place_monster = list_files(MONSTER_PATH, ACTOR_EXT);
	for (int i = 0; i < 30; i++)
	{
		if (*index + i >= (int)(list_file_place_monster.size()) || *index + i < 0)
			*(text_list_place_monster[i]) = "";
		else
			*(text_list_place_monster[i]) = list_file_place_monster[i + *index];
	}
}

void				menu_place_monster(t_data data) //coord, board, gui
{
	t_vect			*coord = (t_vect *)(data.data[0]);
	t_game_board	*board = (t_game_board *)(data.data[1]);
	t_gui			*old_gui = (t_gui *)(data.data[2]);
	t_gui			gui;
	bool			play = true;
	SDL_Event		event;
	int 			index = 0;

	gui.add(new t_button(new t_text_button(
						"", BLACK,
						gui.unit * t_vect(1, 1), gui.unit * t_vect(28, 18), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						NULL, NULL));

	gui.add(new t_button(new t_text_button(
						"Which monster do you want to put here ?", BLACK,
						gui.unit * t_vect(1.5, 1.5), gui.unit * t_vect(27, 2), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						NULL, NULL));

	gui.add(new t_button(new t_text_button(
						"Create a new monster", BLACK,
						gui.unit * t_vect(6, 4), gui.unit * t_vect(18, 1.0), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						start_monster_editor, &index));

	list_file_place_monster = list_files(MONSTER_PATH, ACTOR_EXT);
	int i = 0;
	while (i < 30)
	{
		t_button *button = new t_button(new t_text_button(
					(i < (int)(list_file_place_monster.size()) ? list_file_place_monster[i] : ""), BLACK,
					gui.unit * t_vect(1.5 + ((i % 3) * 8.3 + (i % 3)), 6.5 + ((i / 3) * 1.2)),
					gui.unit * t_vect(8.3, 1.0),
					8,
					t_color(0.4, 0.4, 0.4),
					t_color(0.6, 0.6, 0.6)),
					quit_place, t_data(4, board, coord, &play, i));
		text_list_place_monster[i] = &(button->button->text);
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
			{
				gui.click(&event);
				list_file_place_monster = list_files(MONSTER_PATH, ACTOR_EXT);
			}
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
