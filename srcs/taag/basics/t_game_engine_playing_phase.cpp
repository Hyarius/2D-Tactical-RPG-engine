#include "taag.h"

void				s_game_engine::handle_control_game(SDL_Event *event)
{
	if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_SPACE)
		next_turn();
	else if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)
	{
		if (gui.click() == false)
		{
			if (s_spell == -1)
				move_actor(board.get_mouse_pos());
			else
				cast_spell(board.get_mouse_pos());
			check_alive();
		}
	}
	if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_RIGHT && s_spell != -1)
	{
		s_spell = -1;
		calculated = false;
	}
	if (event->type == SDL_KEYDOWN)
	{
		t_actor *player = turn_order[turn_index % turn_order.size()];
		if (event->key.keysym.sym == SDLK_q)
		{
			if (player->stat.pa.value >= player->spell[0]->cost_pa && player->stat.pm.value >= player->spell[0]->cost_pm)
				s_spell = 0;
			else
				s_spell = -1;
		}
		else if (event->key.keysym.sym == SDLK_w)
		{
			if (player->stat.pa.value >= player->spell[1]->cost_pa && player->stat.pm.value >= player->spell[1]->cost_pm)
				s_spell = 1;
			else
				s_spell = -1;
		}
		else if (event->key.keysym.sym == SDLK_e)
		{
			if (player->stat.pa.value >= player->spell[2]->cost_pa && player->stat.pm.value >= player->spell[2]->cost_pm)
				s_spell = 2;
			else
				s_spell = -1;
		}
		else if (event->key.keysym.sym == SDLK_r)
		{
			if (player->stat.pa.value >= player->spell[3]->cost_pa && player->stat.pm.value >= player->spell[3]->cost_pm)
				s_spell = 3;
			else
				s_spell = -1;
		}
		else if (event->key.keysym.sym == SDLK_t)
		{
			if (player->stat.pa.value >= player->spell[4]->cost_pa && player->stat.pm.value >= player->spell[4]->cost_pm)
				s_spell = 4;
			else
				s_spell = -1;
		}
		else if (event->key.keysym.sym == SDLK_y)
		{
			if (player->stat.pa.value >= player->spell[5]->cost_pa && player->stat.pm.value >= player->spell[0]->cost_pm)
				s_spell = 5;
			else
				s_spell = -1;
		}
		calculated = false;
	}
}

void			s_game_engine::ending_fight()
{
	size_t count = 0;
	bool	leave_game = true;

	while (count < board.actor_list.size())
	{
		if (board.actor_list[count]->visual_info.size())
			leave_game = false;
		count++;
	}
	if (leave_game == true)
		exit(0);
}

void			s_game_engine::game_loop()
{
	SDL_Event	event;
	bool		play = true;

	initiate_turn_order();

	while (play)
	{
		prepare_screen();

		update_board();
		draw_board();
		draw_gui();

		if (board.enemy_list.size() == 0)
			ending_fight();

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(2, &gui, &play));
			handle_control_camera(&event);
			handle_control_game(&event);
		}
		render_screen(true);
	}
}
