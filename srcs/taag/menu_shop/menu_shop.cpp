#include "taag.h"

t_gui		gui_part_shop[3];

void		set_index(t_data data)
{
	int *index = (int *)(data.data[0]);
	int value = (int &)(data.data[1]);

	*index = value;
}

void menu_shop(t_data data)
{
	int index = 1;
	(void)data;
	SDL_Event	event;
	bool		play = true;
	t_gui gui;

	t_button *back = new t_button(new t_image_button(t_image(t_color(0.2, 0.2, 0.2)), t_vect(0, 0), get_win_size()), NULL, NULL);

	t_button *sprite_tabs = new t_button(new s_text_button(
			"SPRITE", DARK_GREY,
			t_vect(1, 1) * gui.unit, t_vect(3, 1) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			set_index, t_data(2, &index, 0));
	t_button *spell_tabs = new t_button(new s_text_button(
			"SPELL", DARK_GREY,
			t_vect(4.1, 1) * gui.unit, t_vect(3, 1) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			set_index, t_data(2, &index, 1));
	t_button *map_tabs = new t_button(new s_text_button(
			"MAP", DARK_GREY,
			t_vect(7.2, 1) * gui.unit, t_vect(3, 1) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			set_index, t_data(2, &index, 2));
	t_button *back_ground = new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(1, 2.1) * gui.unit, t_vect(28, 16.9) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			NULL, NULL);

	t_iterator *level_iterator = new s_iterator(&(account->level), NULL, 0, 1, 0, 150,
		new t_button(new s_text_button(
				"Level : ", DARK_GREY,
				t_vect(13, 1) * gui.unit, t_vect(2, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL,
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(15.2, 1) * gui.unit, t_vect(2, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL);

	t_iterator *gold_iterator = new s_iterator(&(account->gold), NULL, 0, 1, 0, 150,
		new t_button(new s_text_button(
				"Gold : ", DARK_GREY,
				t_vect(19, 1) * gui.unit, t_vect(2, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL,
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(21.2, 1) * gui.unit, t_vect(2, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL);

	int index_spell = 0;

	create_tab_sprite(t_data(1, &(gui_part_shop[0])));
	create_tab_spell(t_data(2, &(gui_part_shop[1]), &index_spell));
	create_tab_map(t_data(1, &(gui_part_shop[2])));

	gui.add(back);
	gui.add(sprite_tabs);
	gui.add(spell_tabs);
	gui.add(map_tabs);
	gui.add(back_ground);
	gui.add(level_iterator);
	gui.add(gold_iterator);

	while (play)
	{
		prepare_screen();

		gui.draw_self();
		gui_part_shop[index].draw_self();

		render_screen(true);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(2, &gui, &play));
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				gui.click(&event);
				gui_part_shop[index].click(&event);
			}
			else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
				gui.key_press(&event);
		}
	}
}
