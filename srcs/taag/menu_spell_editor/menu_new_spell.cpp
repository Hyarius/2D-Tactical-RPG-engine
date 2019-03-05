#include "taag.h"

static void			stand(t_data data)
{
	*((bool *)data.data[0]) = false;
}

static void quit_new_spell(t_data data)
{
	t_spell		*spell = (t_spell *)(data.data[0]);
	string		*entry_name = (string *)(data.data[1]);
	string		*entry_path = (string *)(data.data[2]);
	t_vect		*sprite_target = *(t_vect **)(data.data[3]);
	string		**text = (string **)(data.data[4]);
	bool 		*play = (bool *)(data.data[5]);

	*spell = t_spell();
	*sprite_target = t_vect(0, 0);
	*entry_path = "";
	*entry_name = "";
	*play = false;
	for (size_t i = 0; i < 6; i++)
		*(text[i]) = list_action_name[0];
}

void			menu_new_spell(t_data data) //0 - gui / 1 - t_actor * / 2 - & file name / actor_name / tile index / sprite_target
{
	t_gui		gui = t_gui(15, 10);
	SDL_Event	event;

	bool		play = true;

	//t_data(6, &gui, spell, &(entry_name->entry->text), &(entry_path->entry->text), &selected, text)

	s_button *button = new s_button(new s_text_button(//button did you wanna quit
		"Do you want to create a new spell ?", DARK_GREY, //text info
		gui.unit * t_vect(4, 2), gui.unit * t_vect(7, 5), 8, //object info
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		NULL, NULL);
	button->button->image_coord = button->button->image_coord - (gui.unit * t_vect(0, 1));

	gui.add(button);

	gui.add(new s_button(new s_text_button(//button yes
		"YES", DARK_GREY, //text info
		gui.unit * t_vect(4.25, 5.25), gui.unit * t_vect(3, 1.5), 8, //object info
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		quit_new_spell, t_data(6, data.data[1], data.data[2], data.data[3], data.data[4], data.data[5], &play)));

	gui.add(new s_button(new s_text_button(//button no
		"NO", DARK_GREY, //text info
		gui.unit * t_vect(7.75, 5.25), gui.unit * t_vect(3, 1.5), 8, //object info
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		stand, &play));

	while (play == true)
	{
		prepare_screen();

		if (data.data.size() != 0)
			(*((t_gui *)(data.data[0]))).draw_self();
		gui.draw_self();

		render_screen(true);

		if (SDL_PollEvent(&(event)) == 1)
		{
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
				play = false;
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click(&event);
		}
	}

}
