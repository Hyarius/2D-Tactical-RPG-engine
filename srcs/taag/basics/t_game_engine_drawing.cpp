#include "taag.h"

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
	draw_path();
	board.draw_actor_list();
	board.draw_cursor_layer();
	board.draw_mouse_cursor();
	board.draw_actor_visual_info();
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
		if (player->spell[i]->tile != NULL && i == s_spell)
			draw_rectangle(gui.unit * t_vect((i < 3 ? 8 : 18) + ((i % 3) * 1.5), 18.5) - 5, gui.unit + 10, t_color(230, 230, 0));
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
