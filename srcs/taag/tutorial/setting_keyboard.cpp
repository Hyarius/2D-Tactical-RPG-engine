#include "taag.h"

static void		set_azerty(t_data data)
{
	bool *play = (bool *)(data.data[0]);
	account->keyboard[0] = SDLK_a;
	account->keyboard[1] = SDLK_z;
	account->keyboard[2] = SDLK_e;
	account->keyboard[3] = SDLK_r;
	account->keyboard[4] = SDLK_t;
	account->keyboard[5] = SDLK_y;
	*play = false;
}

static void		set_qwerty(t_data data)
{
	bool *play = (bool *)(data.data[0]);
	account->keyboard[0] = SDLK_q;
	account->keyboard[1] = SDLK_w;
	account->keyboard[2] = SDLK_e;
	account->keyboard[3] = SDLK_r;
	account->keyboard[4] = SDLK_t;
	account->keyboard[5] = SDLK_y;
	*play = false;
}

void		setting_keyboard(t_gui *gui)
{
	SDL_Event	event;
	bool		play = true;
	t_gui		tuto_gui = t_gui(15, 10);

	t_button *button = new s_button(new s_text_button(//button did you wanna quit
		"What kind of keyboard did you got ?", DARK_GREY, //text info
		tuto_gui.unit * t_vect(4, 2), tuto_gui.unit * t_vect(7, 5), 16, //object info
		t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
		NULL, NULL);
	button->button->image_coord = button->button->image_coord - tuto_gui.unit * t_vect(0, 1);
	tuto_gui.add(button);

	tuto_gui.add(new s_button(new s_text_button(//button yes
		"AZERTY", DARK_GREY, //text info
		tuto_gui.unit * t_vect(4.25, 5.25), tuto_gui.unit * t_vect(3, 1.5), 8, //object info
		t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
		set_azerty, &play));

	tuto_gui.add(new s_button(new s_text_button(//button no
		"QWERTY", DARK_GREY, //text info
		tuto_gui.unit * t_vect(7.75, 5.25), tuto_gui.unit * t_vect(3, 1.5), 8, //object info
		t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
		set_qwerty, &play));

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