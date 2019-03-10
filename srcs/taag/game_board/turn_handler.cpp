#include "taag.h"

void				s_game_board::enemy_turn()
{
	reset_board();

	bool play = true;
	SDL_Event	event;

	while (execute_gambit(turn_order[turn_index % turn_order.size()]) == false && play == true)
	{
		prepare_screen();

		update_board();
		draw_self();
		draw_gui();

		if (enemy_list.size() == 0 || ally_list.size() == 0)
			ending_fight(&play);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				exit(0);
			handle_control_camera(&event);
		}
		render_screen(true);
	}
	if (play == true)
		next_turn();
}

void				s_game_board::next_turn()
{
	check_alive();
	if (turn_order.size())
	{
		turn_order[turn_index % turn_order.size()]->selected = false;
		turn_order[turn_index % turn_order.size()]->reset_value();
	}
	turn_order[turn_index % turn_order.size()]->handle_effect_duration();
	turn_index = (turn_index + 1) % turn_order.size();
	turn_order[turn_index % turn_order.size()]->apply_effect(0);
	if (turn_order.size())
	{
		turn_order[turn_index % turn_order.size()]->selected = true;
		for (int i = 0; i < 6; i++)
		{
			if (turn_order[turn_index % turn_order.size()]->cooldown[i] > 0)
				turn_order[turn_index % turn_order.size()]->cooldown[i]--;
		}

	}
	calculated = false;
	s_spell = -1;
	if (turn_order[turn_index % turn_order.size()]->team != 1)
	{
		enemy_turn();
	}
}
