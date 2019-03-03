#include "taag.h"

static void			stand(t_data data)
{
	*((bool *)data.data[0]) = false;
}


static void			create_new_monster(t_data data)
{
	t_actor		*actor = (t_actor *)(data.data[0]);
	string		*file_name = (string *)(data.data[1]);
	string		*actor_name = (string *)(data.data[2]);
	bool		*play = (bool *)(data.data[3]);
	size_t		*tile_index = *(size_t **)(data.data[4]);
	t_vect		*sprite_target = *(t_vect **)(data.data[5]);

	*actor = t_actor();
	*file_name = "";
	*actor_name = "";
	size_t i = 0;
	while (sprite_name[i] != "M00")
		i++;
	*tile_index = i;
	*sprite_target = t_vect(0, 0);
	*play = false;
}

void			menu_new_monster(t_data data) //0 - gui / 1 - t_actor * / 2 - & file name / actor_name / tile index / sprite_target
{
	t_actor		*actor = (t_actor *)(data.data[1]);
	string		*file_name = (string *)(data.data[2]);
	string		*actor_name = (string *)(data.data[3]);
	size_t		**tile_index = (size_t **)(data.data[4]);
	t_vect		**sprite_target = (t_vect **)(data.data[5]);
	t_gui		gui = t_gui(15, 10);
	SDL_Event	event;

	bool		play = true;


	s_button *button = new s_button(new s_text_button(//button did you wanna quit
		"Did you want create a new monster ?", DARK_GREY, //text info
		gui.unit * t_vect(4, 2), gui.unit * t_vect(7, 5), 8, //object info
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		NULL, NULL);
	button->button->image_coord = button->button->image_coord - (gui.unit * t_vect(0, 1));

	gui.add(button);

	gui.add(new s_button(new s_text_button(//button yes
		"YES", DARK_GREY, //text info
		gui.unit * t_vect(4.25, 5.25), gui.unit * t_vect(3, 1.5), 8, //object info
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		create_new_monster, t_data(6, actor, file_name, actor_name, &play, tile_index, sprite_target)));

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
