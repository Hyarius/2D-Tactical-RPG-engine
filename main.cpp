#include "taag.h"

/*
TODO :
faire un menu start, et un menu play
*/

void					menu_play(t_data data)
{
	(void)data;
	SDL_Event	event;
	bool		play = true;
	t_gui 		gui;

	while (play)
	{
		prepare_screen();

		gui.draw_self();

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(2, &gui, &play));
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
			{
				if (gui.click() == false)
				{
					printf("clicked out of button\n");
				}
			}
		}
		render_screen(true);
	}
}

void					menu_player_editor(t_data data)
{
	(void)data;
	SDL_Event	event;
	bool		play = true;
	t_gui 		gui;

	while (play)
	{
		prepare_screen();

		gui.draw_self();

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(2, &gui, &play));
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
			{
				if (gui.click() == false)
				{
					printf("clicked out of button\n");
				}
			}
		}
		render_screen(true);
	}
}

void					menu_map_editor(t_data data)
{
	(void)data;
	SDL_Event	event;
	bool		play = true;
	t_gui 		gui;

	while (play)
	{
		prepare_screen();

		gui.draw_self();

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(2, &gui, &play));
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
			{
				if (gui.click() == false)
				{
					printf("clicked out of button\n");
				}
			}
		}
		render_screen(true);
	}
}

void		menu_start()
{
	SDL_Event	event;
	bool		play = true;
	t_gui gui;

	gui.add(new s_button(new s_image_button(
			t_image("ressources/assets/interface/Menu_start.png"),
			t_vect(0, 0), get_win_size()),
			NULL, NULL));

	gui.add(new t_button(new s_text_button(
			"Play", DARK_GREY,
			t_vect(21, 1) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_play, NULL));

	gui.add(new t_button(new s_text_button(
			"Player editor", DARK_GREY,
			t_vect(21, 4) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_player_editor, NULL));

	gui.add(new t_button(new s_text_button(
			"Map editor", DARK_GREY,
			t_vect(21, 7) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_map_editor, NULL));

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
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(2, &gui, &play));
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
			{
				if (gui.click() == false)
				{
					printf("clicked out of button\n");
				}
			}
		}
		render_screen(true);
	}
}

int main(int argc, char **argv)
{
	SDL_Event event;

	if (argc != 1)
		return (0);

	window_initialisation(argv[0]);

	read_tileset();
	init_effects();
	read_spell();

	menu_start();

	/*t_game_engine	game = s_game_engine("ressources/map/domolia.map");

	game.placement_phase();

	game.game_loop();
	*/

	return (0);
}
