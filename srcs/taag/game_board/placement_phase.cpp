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

	while (play)
	{
		prepare_screen();

		draw_self();
		draw_cell_info_on_gui();
		draw_select_wheel(index);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(3, &gui, &play, &continu));
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && actor_list.size() != enemy_list.size())
				play = false;
			handle_control_camera(&event);
			handle_actor_placement(&event, &index, &actor_pool);
		}
		render_screen(true);
	}
	placement_list.clear();
	enemy_placement_list.clear();

	if (continu == true)
		game_loop();
}
