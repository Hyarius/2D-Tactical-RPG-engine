#include "taag.h"

static void			stand(t_data data)
{
	*((bool *)data.data[0]) = false;
}

static void		quit_load(t_data data)// player / entry_name / entry_path / pool / play / num
{
	bool 		*play = (bool *)(data.data[0]);
	t_actor		*player = (t_actor *)(data.data[1]);
	string		*entry_path = (string *)(data.data[2]);
	size_t		*tile_index = (size_t *)(data.data[3]);
	t_vect		*sprite_target = *(t_vect **)(data.data[4]);
	int	*pool_value = (int *)(data.data[5]);

	*player = read_actor(ACTOR_PATH + *(entry_path) + ACTOR_EXT);
	size_t t = 0;
	while (t < account->tile_unlock.size() && get_sprite_tile(account->tile_unlock[t]) != player->tile)
		t++;
	*tile_index = t;
	*sprite_target = player->sprite;
	t_actor base;
	*pool_value = account->calc_pool() - ((player->stat.hp.max - base.stat.hp.max) + (player->stat.pa.max - base.stat.pa.max) * 3 + (player->stat.pm.max - base.stat.pm.max) * 3 + (player->stat.init - base.stat.init));
	*play = false;
}

void			menu_load_actor(t_data data) // 0 - t_gui * / 1 - t_actor * / 2 - &name / 3 - &path / 4 - &pool
{
	t_gui		*old_gui = (t_gui *)(data.data[0]);
	t_actor		*player = (t_actor *)(data.data[1]);
	string		*entry_name = (string *)(data.data[2]);
	string		*entry_path = (string *)(data.data[3]);
	int			*pool_value = (int *)(data.data[4]);
	size_t		*tile_index = *(size_t **)(data.data[5]);
	t_vect		*sprite_target = *(t_vect **)(data.data[6]);
	t_gui		gui = t_gui(15, 10);
	bool		play = true;
	SDL_Event	event;

	s_button *button = new s_button(new s_text_button(//button did you wanna quit
						"Do you want to load this actor again ?", DARK_GREY, //text info
						gui.unit * t_vect(4, 2), gui.unit * t_vect(7, 5), 8, //object info
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
						NULL, NULL);
	button->button->image_coord = button->button->image_coord - gui.unit * t_vect(0, 1);
	gui.add(button);

	gui.add(new s_button(new s_text_button(//button yes
						"YES", DARK_GREY, //text info
						gui.unit * t_vect(4.25, 5.25), gui.unit * t_vect(3, 1.5), 8, //object info
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
						quit_load, t_data(5, &play, player, entry_path, tile_index, &sprite_target)));

	gui.add(new s_button(new s_text_button(//button no
						"NO", DARK_GREY, //text info
						gui.unit * t_vect(7.75, 5.25), gui.unit * t_vect(3, 1.5), 8, //object info
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
						stand, &play));

	while (play == true)
	{
		prepare_screen();

		if (old_gui != NULL)
			old_gui->draw_self();

		gui.draw_self();
		if ((size_t)(account->tuto_state) < gui_tutorial.size())
			gui_tutorial[account->tuto_state].draw_self();

		render_screen(true);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT)
				menu_quit(t_data(1, &gui));
			if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				play = false;
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				if ((size_t)(account->tuto_state) < gui_tutorial.size() && gui_tutorial[account->tuto_state].object_list.size() && gui_tutorial[account->tuto_state].click(&event) == true)
				{
					increment_tutorial(NULL);
					gui.click(&event);
				}
				else if ((size_t)(account->tuto_state) >= gui_tutorial.size())
					gui.click(&event);
			}
			else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
				gui.key_press(&event);
		}
	}
}
