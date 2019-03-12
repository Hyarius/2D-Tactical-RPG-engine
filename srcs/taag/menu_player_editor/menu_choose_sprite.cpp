#include "taag.h"

t_tileset **tileset_list[32];

static void		quit_choose_sprite(t_data data)
{
	size_t			*index = (size_t *)(data.data[0]);
	t_vect			*selected = (t_vect *)(data.data[1]);
	int				*tmp_index = (int *)(data.data[2]);
	int				i = (int &)(data.data[3]);
	t_vect			*tmp_selected = (t_vect *)(data.data[4]);
	bool			*play = (bool *)(data.data[5]);
	vector<string>	*vector_name = (vector<string> *)(data.data[6]);

	*index = (*tmp_index + i) % vector_name->size();
	*selected = *tmp_selected;
	*play = false;
}

static void		increment_index(t_data data)
{
	size_t			*index = (size_t *)(data.data[0]);
	int				delta = (int &)(data.data[1]);
	vector<string>	*vector_name = (vector<string> *)(data.data[2]);

	if (*index == 0 && delta < 0)
		*index = (*vector_name).size() + delta;
	else
		*index = (*index + delta) % (vector_name->size());
	int i = 0;
	while (i < 32)
	{
		*(tileset_list[i]) = get_sprite_tile((*vector_name)[(*index + i / 8) % ((*vector_name).size())]);
		i++;
	}
}

void			menu_choose_sprite(t_data data)
{
	write_in_log("\n");
	write_in_log("\n");
	write_in_log(" --- Statement at menu_choose_sprite : ");
	printOpenGLError();
	printSDLError();
	write_in_log("Everything is settle properly\n");
	write_in_log("\n");
	bool			play = true;
	SDL_Event		event;
	t_gui			gui;

	write_in_log("Getting variable : ");
	size_t *index = ((size_t *)(data.data[1]));
	t_vect *target = ((t_vect *)(data.data[2]));
	vector<string> *vector_name = (vector<string> *)(data.data[3]);
	size_t tmp_index = (int)(*index) % vector_name->size();
	write_in_log("Done\n");

	write_in_log("\n");

	write_in_log("Gui background button : ");
	s_button *button = new s_button(new s_text_button(
		"", DARK_GREY,
		gui.unit * t_vect(0.5, 0.5), gui.unit * t_vect(29, 19), 8,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		NULL, NULL);
	gui.add(button);
	write_in_log("Creation complete\n");

	write_in_log("Gui up button : ");
	button = new s_button(new s_text_button(
		"UP", DARK_GREY,
		gui.unit * t_vect(13, 1), gui.unit * t_vect(4, 1), 8,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		increment_index, t_data(3, &tmp_index, -1, vector_name));
	gui.add(button);
	write_in_log("Creation complete\n");

	write_in_log("Gui down button : ");
	button = new s_button(new s_text_button(
		"DOWN", DARK_GREY,
		gui.unit * t_vect(13, 18), gui.unit * t_vect(4, 1), 8,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		increment_index, t_data(3, &tmp_index, 1, vector_name));
	gui.add(button);
	write_in_log("Creation complete\n");

	write_in_log("\n");

	write_in_log("Setting info for sprite button : ");
	t_vect coord = t_vect(2, 2.25);
	t_vect size = t_vect((26 - (0.25 * 8)) / 8, (15.75 - (0.25 * 4)) / 4);
	t_vect space = t_vect(0.25, 0.25);
	t_vect selected = t_vect(0, 0);
	t_vect increment = t_vect(3, 4);
	write_in_log("Setting complete\n");

	write_in_log("Gui sprite button : ");
	int i = 0;
	while (i < 32)
	{
		t_button *button = new t_button(new t_tileset_button(
						NULL,
						selected,
						t_vect(4, 0),
						t_vect(coord.x + (size.x + space.x) * (i % 8), coord.y + (size.y + space.y) * (i / 8)) * gui.unit,
						size * gui.unit,
						5),
						quit_choose_sprite, NULL);
		button->button->data_left = t_data(7, index, target, &tmp_index, i / 8, &(((t_tileset_button *)(button->button))->selected), &play, vector_name);
		tileset_list[i] = &(((t_tileset_button *)(button->button))->tile);
		*(tileset_list[i]) = get_sprite_tile((*vector_name)[(tmp_index + i / 8) % (*vector_name).size()]);
		gui.add(button);
		selected.x += increment.x;
		if (selected.x >= get_sprite_tile((*vector_name)[tmp_index])->nb_sprite.x)
			selected = t_vect(0, selected.y + increment.y);
		if (selected.y >= get_sprite_tile((*vector_name)[tmp_index])->nb_sprite.y)
			selected = t_vect(0, 0);
		i++;
	}
	write_in_log("Creation complete\n");

	write_in_log("\n");
	int a = 2;
	while (play == true)
	{
		if (a >= 2)
		{
			write_in_log("--- Starting menu_choose_sprite loop ---\n");
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
		gui.draw_self();
		if ((size_t)(account->tuto_state) < gui_tutorial.size())
			gui_tutorial[account->tuto_state].draw_self();

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

		if (SDL_PollEvent(&(event)) == 1)
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
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				if ((size_t)(account->tuto_state) < gui_tutorial.size() && gui_tutorial[account->tuto_state].object_list.size() && gui_tutorial[account->tuto_state].click(&event) == true)
				{
					increment_tutorial(NULL);
					gui.click(&event);
				}
				else if ((size_t)(account->tuto_state) >= gui_tutorial.size())
					gui.click(&event);
			}
			else if (event.type == SDL_MOUSEWHEEL && event.wheel.y < 0)
				increment_index(t_data(3, &tmp_index, +1, vector_name));
			else if (event.type == SDL_MOUSEWHEEL && event.wheel.y > 0)
				increment_index(t_data(3, &tmp_index, -1, vector_name));
		}
	}
}
