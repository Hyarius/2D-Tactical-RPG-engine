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

static void test(t_data data)
{

}

s_game_engine::s_game_engine(string p_path)
{
	read_tileset();
	board = t_game_board(p_path);
	gui = t_gui(30, 20);
	gui.add(new s_button(new t_image_button(t_image("ressources/assets/interface/GUI_Shortcut.png"), gui.unit * t_vect(7.5, 18), gui.unit * t_vect(15, 2)), NULL, NULL));
	gui.add(new s_button(new t_text_button("", BLACK, gui.unit * t_vect(22, 0), gui.unit * t_vect(8, 5), 4, t_color(0.3, 0.3, 0.3, 0.6), t_color(0.6, 0.6, 0.6, 0.6)), &test, NULL));
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
	calculate_distance();
}

void				s_game_engine::draw_board()
{
	board.draw_self();
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

void				s_game_engine::calc_cell(vector<t_vect> *to_calc, int i, int x, int j, int y)
{
	if (board.get_cell(i + x, j + y) != NULL && board.get_cell(i + x, j + y)->node->m_obs == false && board.get_cell(i + x, j + y)->actor == NULL &&
		board.get_cell(i + x, j + y)->m_dist > board.get_cell(i, j)->m_dist &&
		board.get_cell(i, j)->m_dist + board.get_cell(i, j)->node->cost <= turn_order[turn_index % turn_order.size()]->stat.pm.value)
	{
		board.get_cell(i + x, j + y)->m_dist = board.get_cell(i, j)->m_dist + board.get_cell(i, j)->node->cost;
		to_calc->push_back(t_vect(i + x, j + y));
	}
}


void				s_game_engine::calculate_distance()
{
	board.reset_board();
	t_actor 		*player = turn_order[turn_index % turn_order.size()];
	vector<t_vect>	to_calc;
	size_t i, j;

	to_calc.push_back(player->coord);

	i = 0;
	board.get_cell(to_calc[i].x, to_calc[i].y)->m_dist = 0;
	while (i < to_calc.size())
	{
		calc_cell(&to_calc, to_calc[i].x, 1, to_calc[i].y, 0);
		calc_cell(&to_calc, to_calc[i].x, -1, to_calc[i].y, 0);
		calc_cell(&to_calc, to_calc[i].x, 0, to_calc[i].y, 1);
		calc_cell(&to_calc, to_calc[i].x, 0, to_calc[i].y, -1);
		i++;
	}
	i = 1;
	while (i < to_calc.size())
	{
		board.get_cell(to_calc[i].x, to_calc[i].y)->cursor = t_vect(1, 2);
		i++;
	}
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
	if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)
	{
		if (gui.click() == false && turn_order.size())
			move_actor(board.get_mouse_pos());
	}
}

vector<t_vect>		s_game_engine::pathfinding(t_vect dest)
{
	vector<t_vect>	path;
	t_vect			actual = dest;
	t_vect			to_look = actual;
	t_vect			source = turn_order[turn_index % turn_order.size()]->coord;

	while (actual != source)
	{
		actual = to_look;
		if (board.get_cell(actual.x + 1, actual.y) && board.get_cell(actual.x + 1, actual.y)->m_dist < board.get_cell(actual.x, actual.y)->m_dist)
			to_look = t_vect(actual.x + 1, actual.y);
		else if (board.get_cell(actual.x - 1, actual.y) && board.get_cell(actual.x - 1, actual.y)->m_dist < board.get_cell(actual.x, actual.y)->m_dist)
			to_look = t_vect(actual.x - 1, actual.y);
		else if (board.get_cell(actual.x, actual.y + 1) && board.get_cell(actual.x, actual.y + 1)->m_dist < board.get_cell(actual.x, actual.y)->m_dist)
			to_look = t_vect(actual.x, actual.y + 1);
		else if (board.get_cell(actual.x, actual.y - 1) && board.get_cell(actual.x, actual.y - 1)->m_dist < board.get_cell(actual.x, actual.y)->m_dist)
			to_look = t_vect(actual.x, actual.y - 1);
		int i = 0;
		t_vect delta = ((to_look - actual) / 15);
		while (i < 15)
		{
			path.insert(path.begin(), actual + delta * i);
			i++;
		}
	}
	return (path);
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
		}
		i++;
	}
	if (turn_order.size() && turn_order[turn_index % turn_order.size()]->destination.size() == 0)
		calculate_distance();
}
