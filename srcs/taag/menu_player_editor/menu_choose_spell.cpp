#include "taag.h"

t_spell		*spell_list[18];

static void	modify_index(t_data data)
{
	int *index = (int *)(data.data[0]);
	int delta = (int &)(data.data[1]);
	t_gui *gui = (t_gui *)(data.data[2]);

	if (*index + delta >= -12 && *index + delta < (int)(((spell_heros_name.size() / 6) * 6 + 6)))
		*index += delta;
	for (int i = 0; i < 18; i++)
	{
		if (*index + i >= (int)(spell_heros_name.size()) || *index + i < 0)
			spell_list[i] = &(spell_map["NULL"]);
		else
			spell_list[i] = &(spell_map[spell_heros_name[(*index + i) % spell_heros_name.size()]]);
	}
}

static void	quit_select_spell(t_data data)// &actor | num spell | i | &play
{
	t_actor		*player = (t_actor *)(data.data[0]);
	int			spell_num = (int &)(data.data[1]);
	int			i = (int &)(data.data[2]);
	bool		*play = (bool *)(data.data[3]);

	player->spell[spell_num] = spell_list[i];
	*play = false;
}

void		menu_choose_spell(t_data data)
{
	t_gui		*old_gui = (t_gui *)(data.data[0]);
	int			spell_num = (int &)(data.data[1]);
	t_actor		*player = (t_actor *)(data.data[2]);
	t_gui		gui;
	bool		play = true;
	SDL_Event	event;


	gui.add(new t_button(new t_text_button(
						"", BLACK,
						gui.unit * t_vect(0.3, 0.3), gui.unit * t_vect(29.4, 19.4), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						NULL, NULL));
	int index = 0;

	for (int i = 0; i < 18; i++)
	{
		if (index + i >= (int)(spell_heros_name.size()) || index + i < 0)
			spell_list[i] = &(spell_map["NULL"]);
		else
			spell_list[i] = &(spell_map[spell_heros_name[(index + i) % spell_heros_name.size()]]);
		gui.add(new t_spell_card(&(spell_list[i]), gui.unit * t_vect(0.5 + (i % 6) * 4 + (0.2 * (i % 6)), (0.5 + (i / 6) * 6 + (0.2 * (i / 6)))), gui.unit * t_vect(4, 6), quit_select_spell, t_data(4, player, spell_num, i, &play)));// &actor | num spell | i | &play
	}

	gui.add(new t_button(new t_text_button(
						"up", BLACK,
						gui.unit * t_vect(26, 1), gui.unit * t_vect(3, 5), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						modify_index, t_data(3, &index, -6, &gui)));

	gui.add(new t_button(new t_text_button(
						"down", BLACK,
						gui.unit * t_vect(26, 14), gui.unit * t_vect(3, 5), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						modify_index, t_data(3, &index, 6, &gui)));

	while (play == true)
	{
		prepare_screen();

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
				modify_index(t_data(3, &index, -6, &gui));
			}
			else if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_DOWN))
			{
				modify_index(t_data(3, &index, 6, &gui));
			}
			else if (event.type == SDL_MOUSEWHEEL)
			{
				if (event.wheel.y > 0)
					modify_index(t_data(3, &index, -6, &gui));
				else if (event.wheel.y < 0)
					modify_index(t_data(3, &index, 6, &gui));
			}
		}
	}
	(void)data;
}
