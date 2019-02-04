#include "taag.h"

void					menu_map_editor(t_data data)
{
	(void)data;
	t_game_board board;
	SDL_Event	event;
	bool		play = true;
	t_gui 		gui;
	
	double i = 0;

	i = 13;
	t_button	*save_button = new t_button(new s_text_button(
		"Save map", DARK_GREY,
		t_vect(1, 1 + (1.2 * i)) * gui.unit, t_vect(8, 1) * gui.unit, 5,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		NULL, NULL);

	t_button	*generate_button = new t_button(new s_text_button(
		"Generate map", DARK_GREY,
		t_vect(10, 1 + (1.2 * i)) * gui.unit,
		t_vect(8, 1) * gui.unit,
		5,
		t_color(0.4, 0.4, 0.4),
		t_color(0.6, 0.6, 0.6)),
		menu_generate_board, t_data(2, &gui, &board));

	i++;

	t_button	*load_button = new t_button(new s_text_button(
		"Load map", DARK_GREY,
		t_vect(1, 1 + (1.2 * i)) * gui.unit, t_vect(8, 1) * gui.unit, 5,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		NULL, NULL);

	t_button	*delete_button = new t_button(new s_text_button(
		"Delete map", DARK_GREY,
		t_vect(10, 1 + (1.2 * i)) * gui.unit,
		t_vect(8, 1) * gui.unit,
		5,
		t_color(0.4, 0.4, 0.4),
		t_color(0.6, 0.6, 0.6)),
		NULL, NULL);

	t_button	*quit_button = new t_button(new s_text_button(
		"Quit", DARK_GREY,
		t_vect(19, 1 + (1.2 * i)) * gui.unit, t_vect(8, 1) * gui.unit, 5,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		menu_quit, t_data(2, &gui, &play));
	i++;

	gui.add(save_button);
	gui.add(generate_button);
	gui.add(load_button);
	gui.add(delete_button);
	gui.add(quit_button);

	while (play)
	{
		prepare_screen(t_color(0.2, 0.2, 0.2));

		board.draw_self();

		gui.draw_self();

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(2, &gui, &play));
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click();
			else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
				gui.key_press(&event);
			board.handle_mouvement(&event);
			board.handle_zoom(&event);
		}
		render_screen(true);
	}
}
