#include "taag.h"

vector<string>		list_file_delete_monster;
string 				*text_list_delete_monster[30];

static void	modify_index(t_data data)
{
	int *index = (int *)(data.data[0]);
	int delta = (int &)(data.data[1]);

	if (*index + delta >= -27 && *index + delta < (int)(((list_file_delete_monster.size() / 3) * 3 + 3)))
		*index += delta;
	for (int i = 0; i < 30; i++)
	{
		if (*index + i >= (int)(list_file_delete_monster.size()) || *index + i < 0)
			*(text_list_delete_monster[i]) = "";
		else
			*(text_list_delete_monster[i]) = list_file_delete_monster[i + *index];
	}
}

static void		quit_delete(t_data data)// player / entry_name / entry_path / pool / play / num
{
	t_actor		*player = (t_actor *)(data.data[0]);
	string		*entry_name = (string *)(data.data[1]);
	string		*entry_path = (string *)(data.data[2]);
	bool 		*play = (bool *)(data.data[3]);
	size_t		*tile_index = *(size_t **)(data.data[5]);
	t_vect		*sprite_target = *(t_vect **)(data.data[6]);
	int			i = (int &)(data.data[4]);

	if (*(text_list_delete_monster[i]) != "")
	{
		remove((MONSTER_PATH + *(text_list_delete_monster[i]) + MONSTER_EXT).c_str());
		*player = t_actor();
		*tile_index = 0;
		while (sprite_name[*tile_index][0] != 'M')
			(*tile_index)++;
		*sprite_target = t_vect(0, 0);
		*entry_path = "";
		*entry_name = "";
		*play = true;
		list_file_delete_monster = list_files(MONSTER_PATH, MONSTER_EXT);
		for (int i = 0; i < 30; i++)
			*(text_list_delete_monster[i]) = (i < (int)(list_file_delete_monster.size()) ? list_file_delete_monster[i] : "");
	}
}

void			menu_delete_monster(t_data data) // 0 - t_gui * / 1 - t_actor * / 2 - &name / 3 - &path / 4 - &pool
{
	write_in_log("\n");
	write_in_log("\n");
	write_in_log(" --- Statement at menu_choose_spell : ");
	printOpenGLError();
	printSDLError();
	write_in_log("Everything is settle properly\n");
	write_in_log("\n");

	write_in_log("Setting data value : ");
	t_gui		*old_gui = (t_gui *)(data.data[0]);
	t_actor		*player = (t_actor *)(data.data[1]);
	string		*entry_name = (string *)(data.data[2]);
	string		*entry_path = (string *)(data.data[3]);
	size_t		*tile_index = *(size_t **)(data.data[4]);
	t_vect		*sprite_target = *(t_vect **)(data.data[5]);
	t_gui		gui;
	bool		play = true;
	SDL_Event	event;
	int 		index = 0;
	write_in_log("Setting complete\n");

	write_in_log("Gui background button : ");

	gui.add(new t_button(new t_text_button(
						"", BLACK,
						gui.unit * t_vect(1, 1), gui.unit * t_vect(28, 18), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						NULL, NULL));
	write_in_log("Creation complete\n");

	write_in_log("Gui question button : ");

	gui.add(new t_button(new t_text_button(
						"Which file do you want to delete ?", BLACK,
						gui.unit * t_vect(1.5, 1.5), gui.unit * t_vect(27, 2), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						NULL, NULL));
	write_in_log("Creation complete\n");

	write_in_log("Setting delete file list : ");

	list_file_delete_monster = list_files(MONSTER_PATH, MONSTER_EXT);
	int i = 0;
	while (i < 30)
	{
		t_button *button = new t_button(new t_text_button(
					(i < (int)(list_file_delete_monster.size()) ? list_file_delete_monster[i] : ""), BLACK,
					gui.unit * t_vect(1.5 + ((i % 3) * 8.3 + (i % 3)), 4 + ((i / 3) * 1.3)),
					gui.unit * t_vect(8.3, 1.1),
					8,
					t_color(0.4, 0.4, 0.4),
					t_color(0.6, 0.6, 0.6)),
					quit_delete, t_data(7, player, entry_name, entry_path, &play, i, &tile_index, &sprite_target));// player / entry_name / entry_path / pool / play / num
		text_list_delete_monster[i] = &(button->button->text);
		gui.add(button);
		i++;
	}
	write_in_log("DONE\n");

	write_in_log("\n");
	int a = 2;

	while (play == true)
	{
		if (a >= 2)
		{
			write_in_log("--- Starting menu_delete_actor loop ---\n");
			write_in_log("Prepare screen : ");
		}
		prepare_screen();

		if (a >= 2)
		{
			write_in_log("DONE\n");
			write_in_log("Drawing gui : ");
		}

		if (old_gui != NULL)
			old_gui->draw_self();

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
	printf("menu_delete_actor\n");
}
