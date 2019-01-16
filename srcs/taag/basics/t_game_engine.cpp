#include "taag.h"

void				s_game_engine::handle_control_camera(SDL_Event *event)
{
	board.handle_mouvement(event);
	board.handle_zoom(event);
}

void				s_game_engine::cast_spell(t_vect mouse)
{
	t_actor *player = turn_order[turn_index % turn_order.size()];
	if (board.get_cell(mouse)->v_dist >= player->spell[s_spell]->range[0] &&
		board.get_cell(mouse)->v_dist <= player->spell[s_spell]->range[1])
	{
		size_t i = 0;
		while (i < player->spell[s_spell]->effect.size())
		{
			player->spell[s_spell]->effect[i].effect(player, board.get_cell(mouse)->actor, player->spell[s_spell]->effect[i].stat);
			i++;
		}
	}

}

void				s_game_engine::handle_control_game(SDL_Event *event)
{
	if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_SPACE)
		next_turn();
	else if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)
	{
		if (gui.click() == false && turn_order.size())
		{
			if (s_spell == -1)
				move_actor(board.get_mouse_pos());
			else
				cast_spell(board.get_mouse_pos());
		}
	}
	if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_RIGHT && s_spell != -1)
	{
		s_spell = -1;
		calculated = false;
	}
	if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_q)
		s_spell = 0;
	else if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_w)
		s_spell = 1;
	else if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_e)
		s_spell = 2;
	else if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_r)
		s_spell = 3;
	else if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_t)
		s_spell = 4;
	else if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_y)
		s_spell = 5;
	if (s_spell != -1)
		calculated = false;
}

void				s_game_engine::move_actor(t_vect dest)
{
	if (turn_order[turn_index % turn_order.size()]->destination.size() == 0 &&
		board.get_cell(dest) && board.get_cell(dest.x, dest.y)->m_dist <= turn_order[turn_index % turn_order.size()]->stat.pm.value)
	{
		turn_order[turn_index % turn_order.size()]->destination = pathfinding(board.get_mouse_pos());
		turn_order[turn_index % turn_order.size()]->stat.pm.value -= board.get_cell(dest.x, dest.y)->m_dist;
		board.get_cell(dest)->actor = turn_order[turn_index % turn_order.size()];
		board.get_cell(turn_order[turn_index % turn_order.size()]->coord.x, turn_order[turn_index % turn_order.size()]->coord.y)->actor = NULL;
		board.reset_board();
	}
}
