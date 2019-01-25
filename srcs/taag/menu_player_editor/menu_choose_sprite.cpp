#include "taag.h"

void			menu_choose_sprite(t_data data)
{
	bool			continu = true;
	SDL_Event		event;
	t_gui			gui;

	s_button *button = new s_button(new s_text_button(
					"", DARK_GREY,
					gui.unit * t_vect(2, 2), gui.unit * t_vect(26, 16), 8,
					t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
					NULL, NULL);
	gui.add(button);

	int i = 0;
	while (i < 8)
	{}

	size_t *index = *((size_t **)(data.data[1]));

	while (continu == true)
	{
		prepare_screen();
		printf("index = %zu\n", *index);

		if (data.data.size() != 0)
			(*((t_gui *)(data.data[0]))).draw_self();
		gui.draw_self();

		render_screen(true);

		if (SDL_PollEvent(&(event)) == 1)
		{
			if (event.type == SDL_QUIT)
				menu_quit(t_data(2, &gui, &continu));
			else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
				continu = false;
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click();
			else if (event.type == SDL_MOUSEWHEEL && event.wheel.y < 0)
				*index = ((int)(*index) - 1 < 0 ? sprite_name.size() - 1 : *index - 1);
			else if (event.type == SDL_MOUSEWHEEL && event.wheel.y > 0)
				*index = ((int)(*index) + 1 >= (int)(sprite_name.size()) ? 0 : *index + 1);
		}
	}
}
