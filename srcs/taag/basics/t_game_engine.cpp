#include "taag.h"

s_game_engine::s_game_engine()
{

}

void				s_game_engine::insert_actor(t_actor *new_actor)
{
	size_t count = 0;
	while (count < turn_order.size() && new_actor->stat.init < turn_order[count]->stat.init)
		count++;
	turn_order.insert(turn_order.begin() + count, new_actor);
}

void				s_game_engine::delete_actor(t_actor *new_actor)
{
	size_t count = 0;
	while (count < turn_order.size() && new_actor != turn_order[count])
		count++;
	turn_order.erase(turn_order.begin() + count);
}

s_game_engine::s_game_engine(string p_path)
{
	calculated = false;
	s_spell = -1;
	read_tileset();
	read_spell();
	board = t_game_board(p_path);
	gui = t_gui(30, 20);
	gui.add(new s_button(new t_image_button(t_image("ressources/assets/interface/GUI_Shortcut.png"), t_vect(0, 0), gui.unit * t_vect(30, 20)), NULL, NULL));
	gui.add(new s_button(new t_text_button("", BLACK, gui.unit * t_vect(22, 0), gui.unit * t_vect(8, 5), 4, t_color(0.3, 0.3, 0.3, 0.6), t_color(0.6, 0.6, 0.6, 0.6)), NULL, NULL));
	initiate_turn_order();
}

void				s_game_engine::initiate_turn_order()
{
	turn_index = 0;

	size_t i = 0;
	while (i < board.actor_list.size())
	{
		insert_actor(board.actor_list[i]);
		i++;
	}
	if (turn_order.size())
		turn_order[turn_index % turn_order.size()]->selected = true;
	board.reset_board();
	if (turn_order.size())
		calculate_distance();
}

void				s_game_engine::next_turn()
{
	if (turn_order.size())
		turn_order[turn_index % turn_order.size()]->selected = false;
	turn_index = (turn_index + 1) % turn_order.size();
	if (turn_order.size())
		turn_order[turn_index % turn_order.size()]->selected = true;
	if (turn_order.size())
		turn_order[turn_index % turn_order.size()]->reset_value();
	calculated = false;
	s_spell = -1;
}

void				s_game_engine::draw_path()
{
	vector<t_vect>	path = calc_path(board.get_mouse_pos());

	size_t i = 0;
	while (i < path.size())
	{
		draw_centred_rectangle((path[i] + board.target) * board.sprite_unit * board.zoom + board.offset + board.sprite_unit * board.zoom / 2, board.sprite_unit * board.zoom / 2, t_color(0.5, 0.5, 1.0));
		i++;
	}
}

void				s_game_engine::draw_board()
{
	board.draw_cell_layer();
	board.draw_cursor_layer();
	draw_path();
	board.draw_actor_list();
	board.draw_mouse_cursor();
}

void				s_game_engine::draw_actor_info_on_gui()
{
	t_actor *player = turn_order[turn_index % turn_order.size()];
	string text = to_string(player->stat.hp.value) + "/" + to_string(player->stat.hp.max);
	draw_centred_text(text, calc_text_max_size(text, gui.unit * t_vect(1.7, 0.9)), gui.unit * t_vect(15, 19), BLACK);
	text = to_string(player->stat.pa.value);
	draw_centred_text(text, calc_text_max_size(text, gui.unit * t_vect(1.7, 0.9)), gui.unit * t_vect(13, 19), BLACK);
	text = to_string(player->stat.pm.value);
	draw_centred_text(text, calc_text_max_size(text, gui.unit * t_vect(1.7, 0.9)), gui.unit * t_vect(17, 19), BLACK);
	for (int i = 0; i < 6; i++)
	{
		if (player->spell[i]->tile != NULL)
			player->spell[i]->tile->draw_self(gui.unit * t_vect((i < 3 ? 8 : 18) + ((i % 3) * 1.5), 18.5), gui.unit, player->spell[i]->icon);
	}
}

void				s_game_engine::draw_cell_info_on_gui()
{
	t_vect mouse = board.get_mouse_pos();
	int i = 0;

	if (mouse != t_vect(-1, -1))
	{
		t_cell *cell = board.get_cell(mouse.x, mouse.y);
		string text = "Coord : " + to_string((int)(mouse.x)) + " / " + to_string((int)(mouse.y)) + " - " + (cell ? cell->node->name : "Empty tile");
		draw_lined_text(text, calc_text_max_size(text, gui.unit * t_vect(7, 0.5)), gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
		if (cell == NULL || cell->node->m_obs == true)
			text = "Cost to pass : ";
		else
			text = "Cost to pass : " + to_string(cell->node->cost);
		draw_lined_text(text, calc_text_max_size(text, gui.unit * t_vect(7, 0.5)), gui.unit * t_vect(22.2, 0.5 * i + 0.5), BLACK);
		if (cell == NULL || cell->m_dist == 999)
			text = "Cost to reach : ";
		else
			text = "Cost to reach : " + to_string(cell->m_dist);
		draw_lined_text(text, calc_text_max_size(text, gui.unit * t_vect(7, 0.5)), gui.unit * t_vect(25.75, 0.5 * i++ + 0.5), BLACK);
		if (cell && cell->actor != NULL)
		{

			t_actor *player = cell->actor;
			text = "Actor : " + player->name;
			draw_lined_text(text, calc_text_max_size(text, gui.unit * t_vect(7, 0.5)), gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
			string text = "Health : " + to_string(player->stat.hp.value) + "/" + to_string(player->stat.hp.max);
			draw_lined_text(text, calc_text_max_size(text, gui.unit * t_vect(7, 0.5)), gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
			text = "Action points : " + to_string(player->stat.pa.value);
			draw_lined_text(text, calc_text_max_size(text, gui.unit * t_vect(7, 0.5)), gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
			text = "Mouvement points : " + to_string(player->stat.pm.value);
			draw_lined_text(text, calc_text_max_size(text, gui.unit * t_vect(7, 0.5)), gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
		}
	}
}

void				s_game_engine::draw_gui()
{
	gui.draw_self();
	if (turn_order.size())
		draw_actor_info_on_gui();
	draw_cell_info_on_gui();
}

void				s_game_engine::handle_control_camera(SDL_Event *event)
{
	board.handle_mouvement(event);
	board.handle_zoom(event);
}

void				s_game_engine::handle_control_game(SDL_Event *event)
{
	if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_SPACE)
		next_turn();
	else if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT && s_spell == -1)
	{
		if (gui.click() == false && turn_order.size())
			move_actor(board.get_mouse_pos());
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
		board.get_cell(dest.x, dest.y) && board.get_cell(dest.x, dest.y)->m_dist <= turn_order[turn_index % turn_order.size()]->stat.pm.value)
	{
		turn_order[turn_index % turn_order.size()]->destination = pathfinding(board.get_mouse_pos());
		turn_order[turn_index % turn_order.size()]->stat.pm.value -= board.get_cell(dest.x, dest.y)->m_dist;
		board.get_cell(dest.x, dest.y)->actor = turn_order[turn_index % turn_order.size()];
		board.get_cell(turn_order[turn_index % turn_order.size()]->coord.x, turn_order[turn_index % turn_order.size()]->coord.y)->actor = NULL;
		board.reset_board();
	}
}

void				s_game_engine::v_calc_cell(vector<t_vect> *to_calc, t_vect target, int prev_dist)
{
	t_vect 			player_coord = turn_order[turn_index % turn_order.size()]->coord;
	vector<t_vect>	vision_tl = calc_line_2d(t_vect(player_coord.x + 0.49, player_coord.y + 0.49) * 10, t_vect(target.x + 0.49, target.y + 0.49) * 10);
	vector<t_vect>	vision_tr = calc_line_2d(t_vect(player_coord.x + 0.51, player_coord.y + 0.49) * 10, t_vect(target.x + 0.51, target.y + 0.49) * 10);
	vector<t_vect>	vision_dl = calc_line_2d(t_vect(player_coord.x + 0.49, player_coord.y + 0.51) * 10, t_vect(target.x + 0.49, target.y + 0.51) * 10);
	vector<t_vect>	vision_dr = calc_line_2d(t_vect(player_coord.x + 0.51, player_coord.y + 0.51) * 10, t_vect(target.x + 0.51, target.y + 0.51) * 10);

	if (board.get_cell(target) && board.get_cell(target)->node->v_obs == false && board.get_cell(target)->v_dist >= prev_dist)
	{
		size_t i = 0;
		bool visible = true;
		while (i < vision_tl.size() && visible == true)
		{
			if (board.get_cell(vision_tl[i] / 10)->node->v_obs == true || (board.get_cell(vision_tl[i] / 10)->actor != NULL && board.get_cell(vision_tl[i] / 10)->actor != turn_order[turn_index % turn_order.size()]))
				if (board.get_cell(vision_tr[i] / 10)->node->v_obs == true || (board.get_cell(vision_tr[i] / 10)->actor != NULL && board.get_cell(vision_tr[i] / 10)->actor != turn_order[turn_index % turn_order.size()]))
					if (board.get_cell(vision_dl[i] / 10)->node->v_obs == true || (board.get_cell(vision_dl[i] / 10)->actor != NULL && board.get_cell(vision_dl[i] / 10)->actor != turn_order[turn_index % turn_order.size()]))
						if (board.get_cell(vision_dl[i] / 10)->node->v_obs == true || (board.get_cell(vision_dr[i] / 10)->actor != NULL && board.get_cell(vision_dr[i] / 10)->actor != turn_order[turn_index % turn_order.size()]))
							visible = false;
			i++;
		}
		if (visible == true)
		{
			board.get_cell(target)->v_dist = prev_dist + 1;
			if (board.get_cell(target)->v_dist <= turn_order[turn_index % turn_order.size()]->spell[s_spell]->range[1])
			{
				if (board.get_cell(target)->v_dist >= turn_order[turn_index % turn_order.size()]->spell[s_spell]->range[0])
				{
					board.get_cell(target)->cursor = t_vect(0, 2);
				}
				if (to_calc != NULL)
					to_calc->push_back(target);
			}
		}
	}
}

void				s_game_engine::calculate_vision_circle()
{
	board.reset_board();
	t_actor 		*player = turn_order[turn_index % turn_order.size()];
	vector<t_vect>	to_calc;
	size_t i;

	to_calc.push_back(player->coord);
	board.get_cell(to_calc[0])->v_dist = 0;
	i = 0;
	while (i < to_calc.size())
	{
		v_calc_cell(&to_calc, t_vect(to_calc[i].x + 1, to_calc[i].y), board.get_cell(to_calc[i])->v_dist);
		v_calc_cell(&to_calc, t_vect(to_calc[i].x - 1, to_calc[i].y), board.get_cell(to_calc[i])->v_dist);
		v_calc_cell(&to_calc, t_vect(to_calc[i].x, to_calc[i].y + 1), board.get_cell(to_calc[i])->v_dist);
		v_calc_cell(&to_calc, t_vect(to_calc[i].x, to_calc[i].y - 1), board.get_cell(to_calc[i])->v_dist);
		i++;
	}
	calculated = true;
}

void				s_game_engine::calculate_vision_line()
{
	board.reset_board();
	t_actor 		*player = turn_order[turn_index % turn_order.size()];
	int i;

	board.get_cell(player->coord)->v_dist = 0;
	i = 0;
	while (i <= player->spell[s_spell]->range[1])
	{
		v_calc_cell(NULL, t_vect(player->coord.x + i, player->coord.y), i - 1);
		v_calc_cell(NULL, t_vect(player->coord.x - i, player->coord.y), i - 1);
		v_calc_cell(NULL, t_vect(player->coord.x, player->coord.y + i), i - 1);
		v_calc_cell(NULL, t_vect(player->coord.x, player->coord.y - i), i - 1);
		i++;
	}
	calculated = true;
}

void				s_game_engine::update_board()
{
	size_t i = 0;

	while (i < turn_order.size())
	{
		if (turn_order[i]->destination.size() != 0)
		{
			if (turn_order[i]->destination[0].x > turn_order[i]->coord.x)
				turn_order[i]->dir = 1;
			else if (turn_order[i]->destination[0].x < turn_order[i]->coord.x)
				turn_order[i]->dir = 3;
			else if (turn_order[i]->destination[0].y < turn_order[i]->coord.y)
				turn_order[i]->dir = 2;
			else if (turn_order[i]->destination[0].y > turn_order[i]->coord.y)
				turn_order[i]->dir = 0;
			turn_order[i]->coord = turn_order[i]->destination[0];
			turn_order[i]->destination.erase(turn_order[i]->destination.begin());
			calculated = false;
		}
		i++;
	}
	if (turn_order.size() && turn_order[turn_index % turn_order.size()]->destination.size() == 0 && calculated == false && s_spell == -1)
		calculate_distance();
	if (turn_order.size() && turn_order[turn_index % turn_order.size()]->destination.size() == 0 && s_spell != -1 && calculated == false)
	{
		if (turn_order[turn_index % turn_order.size()]->spell[s_spell]->type == CIRCLE)
			calculate_vision_circle();
		else
			calculate_vision_line();
	}
}
