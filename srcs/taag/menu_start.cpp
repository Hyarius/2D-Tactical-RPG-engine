#include "taag.h"

int type = 1;

t_game_engine	 *account;

void		menu_start()
{
	SDL_Event	event;
	bool		play = true;
	t_gui gui;

	account = new t_game_engine();

	gui.add(new s_button(new s_image_button(
			t_image("ressources/assets/interface/Menu_start.png"),
			t_vect(0, 0), get_win_size()),
			NULL, NULL));

	gui.add(new t_button(new s_text_button(
			"Play", DARK_GREY,
			t_vect(21, 1) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_play, NULL));

	t_vect size = t_vect(4, 6);
	for (size_t i = 0; i < 6; i++)
	{
		t_vect coord = t_vect(1 + (size.x + 0.5) * (i % 3), 1 + (size.y + 0.5) * (i / 3));
		gui.add(new t_button(new s_text_button(
				"Player num " + to_string(i), DARK_GREY,
				coord * gui.unit, size * gui.unit, 8,
				t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
				menu_actor_editor, t_data(1, &(account->actor[i]))));
	}

	/*gui.add(new t_button(new s_text_button(
			"Player editor", DARK_GREY,
			t_vect(21, 4) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_player_editor, NULL));*/

	if (type == 1)
	{
		gui.add(new t_button(new s_text_button(
			"Monster editor", DARK_GREY,
			t_vect(21, 7) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_monster_editor, NULL));

		gui.add(new t_button(new s_text_button(
			"Map editor", DARK_GREY,
			t_vect(21, 10) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_map_editor, NULL));

		gui.add(new t_button(new s_text_button(
			"Spell editor", DARK_GREY,
			t_vect(21, 13) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_spell_editor, NULL));
	}

	gui.add(new t_button(new s_text_button(
			"QUIT", DARK_GREY,
			t_vect(21, 17) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_quit, t_data(2, &gui, &play)));



	while (play)
	{

		prepare_screen();

		gui.draw_self();

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(2, &gui, &play));
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click(&event);
			else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
				gui.key_press(&event);
		}
		render_screen(true);
		printOpenGLError();
		printSDLError();
	}
}
