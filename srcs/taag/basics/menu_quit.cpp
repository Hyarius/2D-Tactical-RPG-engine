#include "taag.h"

static void			stand(t_data data)
{
	*((bool *)data.data[0]) = false;
}

static void			quit(t_data data)
{
	if (data.data[0] == NULL)
		exit(0);
	else
	{
		for (size_t i = 0; i < data.data.size(); i++)
			*((bool *)data.data[i]) = false;
	}
}

void			menu_quit(t_data data)
{
	SDL_FlushEvents(SDL_KEYUP, SDL_KEYDOWN);

	SDL_Event	event;
	bool		play = true;

	t_gui		gui = t_gui(15, 10);

	t_button *button = new s_button(new s_text_button(//button did you wanna quit
						"Did you really want to quit ?", DARK_GREY, //text info
						gui.unit * t_vect(4, 2), gui.unit * t_vect(7, 5), 16, //object info
						t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
						NULL, NULL);
	button->button->image_coord = button->button->image_coord - gui.unit * t_vect(0, 1);
	gui.add(button);

	t_data new_data = (data.data.size() == 1 ? NULL : (data.data.size() == 2 ? t_data(2, data.data[1], &play) : data.data.size() == 3 ? t_data(3, data.data[1], data.data[2], &play) : NULL));

	gui.add(new s_button(new s_text_button(//button yes
						"YES", DARK_GREY, //text info
						gui.unit * t_vect(4.25, 5.25), gui.unit * t_vect(3, 1.5), 8, //object info
						t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
						quit, new_data));

	gui.add(new s_button(new s_text_button(//button no
						"NO", DARK_GREY, //text info
						gui.unit * t_vect(7.75, 5.25), gui.unit * t_vect(3, 1.5), 8, //object info
						t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
						stand, &play));

	while (play)
	{
		prepare_screen();

		if (data.data.size() != 0)
			(*((t_gui *)(data.data[0]))).draw_self();
		gui.draw_self();

		render_screen();

		if (SDL_PollEvent(&(event)) == 1)
		{
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
				play = false;
			if (event.type == SDL_MOUSEBUTTONUP)
				gui.click(&event);
		}
	}
}
