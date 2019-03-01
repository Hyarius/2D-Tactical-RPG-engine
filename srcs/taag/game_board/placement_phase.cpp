#include "taag.h"

void				s_game_board::placement_phase(t_actor *game_actor_list[])
{
	bool		continu = true;
	SDL_Event	event;
	bool		play = true;
	int			index = 0;

	for (size_t i = 0; i < 6; i++)
	{
		if (game_actor_list[i] != NULL)
			actor_pool.push_back(game_actor_list[i]);
	}

	if (actor_pool.size() == 0)
		return ;

	gui = t_gui(30, 20);
	gui.add(new s_button(new t_image_button(t_image("ressources/assets/interface/GUI_Shortcut.png"), gui.unit * t_vect(7.5, 18), gui.unit * t_vect(15, 2)), NULL, NULL));
	for (int i = 0; i < 6; i++)
		gui.add(SPELL_BUTTON, new s_button(new t_text_button("", BLACK, gui.unit * t_vect((i < 3 ? 8 : 18) + ((i % 3) * 1.5), 18.5), gui.unit, 0, t_color(0.3, 0.3, 0.3, 0.0), t_color(0.6, 0.6, 0.6, 0.0)), change_s_spell, t_data(3, &s_spell, &calculated, i)));

	if (gui_tutorial[25].object_list.size() == 0)
	{
		gui_tutorial[25] = t_gui(30, 20);

		gui_tutorial[25].add(0, new t_button(new s_paragraph_button(
				"Welcome to the main TAAG actual gameplay : fighting ! \n What you are looking at right now is the game preparation mode : you will place your character on the map. If you have more than 1 character, you can select the one you want to place with the top/down arrow. \n Try to place your first character on one of the blue square on the map, and press \"space\" to end the placement phase, and go to the actual fight !", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(5, 16) * gui_tutorial[0].unit, t_vect(20, 4) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), NULL, t_data(2, NULL, NULL)));
	}

	while (play)
	{
		prepare_screen();

		draw_self();
		draw_cell_info_on_gui();
		draw_select_wheel(index);

		if (account->tuto_state < TUTO_SIZE)
			gui_tutorial[account->tuto_state].draw_self();

		render_screen(true);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(3, &gui, &play, &continu));
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && actor_list.size() != enemy_list.size())
			{
				account->tuto_state++;
				play = false;
			}
			handle_control_camera(&event);
			handle_actor_placement(&event, &index, &actor_pool);
		}
	}
	placement_list.clear();
	enemy_placement_list.clear();

	if (continu == true)
		game_loop();
}
