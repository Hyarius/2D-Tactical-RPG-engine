#include "taag.h"


gambit_command command[10] = {
	&s_game_engine::get_close_enemy,
	NULL,
	&s_game_engine::flee_enemy,
	&s_game_engine::get_close_ally,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};

bool					s_game_engine::execute_gambit(t_actor *source)
{
	if (board.check_anim() == false || source->destination.size() != 0)
		return (false);
	size_t i = 0;
	while (i < source->gambit.size())
	{
		int index = source->gambit[i].value[0];
		t_ai_helper data = source->gambit[i];
		if ((this->*(command[index]))(data) == true)
			return (false);
		i++;
	}
	return (true);
}

void				s_game_engine::enemy_turn()
{
	board.reset_board();
	
	SDL_Event	event;

	while (execute_gambit(turn_order[turn_index % turn_order.size()]) == false)
	{
		prepare_screen();

		update_board();
		draw_board();
		draw_gui();

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				exit(0);
			handle_control_camera(&event);
		}
		render_screen(true);
	}
	next_turn();
}

void				s_game_engine::next_turn()
{
	if (turn_order.size())
	{
		turn_order[turn_index % turn_order.size()]->selected = false;
		turn_order[turn_index % turn_order.size()]->reset_value();
	}
	turn_index = (turn_index + 1) % turn_order.size();
	if (turn_order.size())
	{
		turn_order[turn_index % turn_order.size()]->selected = true;
	}
	calculated = false;
	s_spell = -1;
	if (turn_order[turn_index % turn_order.size()]->team != 1)
	{
		enemy_turn();
	}
}
