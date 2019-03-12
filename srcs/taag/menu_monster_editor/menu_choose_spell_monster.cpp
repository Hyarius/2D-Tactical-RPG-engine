#include "taag.h"

t_spell		*spell_list_monster[18];

static void	modify_index(t_data data)
{
	int *index = (int *)(data.data[0]);
	int delta = (int &)(data.data[1]);

	if (*index + delta >= -12 && *index + delta < (int)(((spell_monster_name.size() / 6) * 6 + 6)))
		*index += delta;
	for (int i = 0; i < 18; i++)
	{
		if (*index + i >= (int)(spell_monster_name.size()) || *index + i < 0)
			spell_list_monster[i] = &(spell_map["NULL"]);
		else
			spell_list_monster[i] = &(spell_map[spell_monster_name[(*index + i) % spell_monster_name.size()]]);
	}
}

static void	quit_select_spell(t_data data)// &actor | num spell | i | &play
{
	t_actor		*player = (t_actor *)(data.data[0]);
	int			spell_num = (int &)(data.data[1]);
	int			i = (int &)(data.data[2]);
	bool		*play = (bool *)(data.data[3]);

	player->spell[spell_num] = spell_list_monster[i];
	*play = false;
}

void		menu_choose_spell_monster(t_data data)
{
	write_in_log("\n");
	write_in_log("\n");
	write_in_log(" --- Statement at menu_choose_spell_monster : ");
	printOpenGLError();
	printSDLError();
	write_in_log("Everything is settle properly\n");
	write_in_log("\n");
	t_gui		*old_gui = (t_gui *)(data.data[0]);
	int			spell_num = (int &)(data.data[1]);
	t_actor		*player = (t_actor *)(data.data[2]);
	bool		*old_play = (bool *)(data.data[3]);
	t_gui		gui;
	bool		play = true;
	SDL_Event	event;

	write_in_log("Gui background button : ");

	gui.add(new t_button(new t_text_button(
						"", BLACK,
						gui.unit * t_vect(0.3, 0.3), gui.unit * t_vect(29.4, 19.4), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						NULL, NULL));
	write_in_log("Creation complete\n");
	int index = 0;

	write_in_log("Setting spell_list : ");
	for (int i = 0; i < 18; i++)
	{
		if (index + i >= (int)(spell_monster_name.size()) || index + i < 0)
			spell_list_monster[i] = &(spell_map["NULL"]);
		else
			spell_list_monster[i] = &(spell_map[spell_monster_name[(index + i) % spell_monster_name.size()]]);
		gui.add(new t_spell_card(&(spell_list_monster[i]), gui.unit * t_vect(0.5 + (i % 6) * 4 + (0.2 * (i % 6)), (0.5 + (i / 6) * 6 + (0.2 * (i / 6)))), gui.unit * t_vect(4, 6), quit_select_spell, t_data(4, player, spell_num, i, &play)));// &actor | num spell | i | &play
	}
	write_in_log("DONE\n");

	write_in_log("\n");

	write_in_log("\n");

	write_in_log("Gui up button : ");

	gui.add(new t_button(new t_text_button(
						"up", BLACK,
						gui.unit * t_vect(26, 1), gui.unit * t_vect(3, 5), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						modify_index, t_data(2, &index, -6)));

	write_in_log("Creation complete\n");

	write_in_log("Gui down button : ");
	gui.add(new t_button(new t_text_button(
						"down", BLACK,
						gui.unit * t_vect(26, 14), gui.unit * t_vect(3, 5), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						modify_index, t_data(2, &index, 6)));
	write_in_log("Creation complete\n");

	write_in_log("\n");
	int a = 2;

	while (play == true)
	{
		if (a >= 2)
		{
			write_in_log("--- Starting menu_choose_spell_monster loop ---\n");
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
				menu_quit(t_data(3, &gui, &play, old_play));
			if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				play = false;
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click(&event);
			else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
				gui.key_press(&event);
			else if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_UP))
			{
				modify_index(t_data(2, &index, -6));
			}
			else if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_DOWN))
			{
				modify_index(t_data(2, &index, 6));
			}
			else if (event.type == SDL_MOUSEWHEEL)
			{
				if (event.wheel.y > 0)
					modify_index(t_data(2, &index, -6));
				else if (event.wheel.y < 0)
					modify_index(t_data(2, &index, 6));
			}
		}
	}
	(void)data;
}
