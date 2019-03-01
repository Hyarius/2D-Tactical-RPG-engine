#include "taag.h"

static void		play_tutorial(t_data data)
{
	(void)data;
	bool *play = (bool *)(data.data[0]);

	account->tuto_state = 1;
	*play = false;
}

static void		don_t_play_tutorial(t_data data)
{
	(void)data;
	bool *play = (bool *)(data.data[0]);

	account->tuto_state = TUTO_SIZE;
	*play = false;
}

void			start_tutorial(t_gui *gui)
{
	SDL_Event	event;
	bool		play = true;
	t_gui		tuto_gui = t_gui(15, 10);

	t_button *button = new s_button(new s_text_button(//button did you wanna quit
						"Did you want to run the tutorial ?", DARK_GREY, //text info
						tuto_gui.unit * t_vect(4, 2), tuto_gui.unit * t_vect(7, 5), 16, //object info
						t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
						NULL, NULL);
	button->button->image_coord = button->button->image_coord - tuto_gui.unit * t_vect(0, 1);
	tuto_gui.add(button);

	tuto_gui.add(new s_button(new s_text_button(//button yes
						"YES", DARK_GREY, //text info
						tuto_gui.unit * t_vect(4.25, 5.25), tuto_gui.unit * t_vect(3, 1.5), 8, //object info
						t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
						play_tutorial, t_data(1, &play)));

	tuto_gui.add(new s_button(new s_text_button(//button no
						"NO", DARK_GREY, //text info
						tuto_gui.unit * t_vect(7.75, 5.25), tuto_gui.unit * t_vect(3, 1.5), 8, //object info
						t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
						don_t_play_tutorial, t_data(1, &play)));

	while (play)
	{
		prepare_screen();

		gui->draw_self();
		tuto_gui.draw_self();

		render_screen(true);

		if (SDL_PollEvent(&(event)) == 1)
		{
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
				play = false;
			if (event.type == SDL_MOUSEBUTTONUP)
				tuto_gui.click(&event);
		}
	}
}
