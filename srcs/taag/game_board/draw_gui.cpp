#include "taag.h"

void				s_game_board::draw_actor_info_on_gui()
{
	t_actor *player = turn_order[turn_index % turn_order.size()];
	if (player->team != 1)
		return ;
	string text = to_string(player->stat.hp.value) + "/" + to_string(player->stat.hp.max);
	static int text_size = calc_text_max_size(text, gui.unit * t_vect(1.7, 0.9));
	draw_centred_text(text, text_size, gui.unit * t_vect(15, 19), BLACK);
	text = to_string(player->stat.pa.value);
	draw_centred_text(text, text_size, gui.unit * t_vect(13, 19), BLACK);
	text = to_string(player->stat.pm.value);
	draw_centred_text(text, text_size, gui.unit * t_vect(17, 19), BLACK);
	for (int i = 0; i < 6; i++)
	{
		if (player->spell[i]->tile != NULL && i == s_spell)
			draw_rectangle(gui.unit * t_vect((i < 3 ? 8 : 18) + ((i % 3) * 1.5), 18.5) - 5, gui.unit + 10, t_color(230, 230, 0));
		if (player->spell[i]->tile != NULL)
		{
			player->spell[i]->tile->draw_self(gui.unit * t_vect((i < 3 ? 8 : 18) + ((i % 3) * 1.5), 18.5), gui.unit, player->spell[i]->icon);
			if (player->spell[i]->cost_pa > player->stat.pa.value || player->spell[i]->cost_pm > player->stat.pm.value || player->cooldown[i] != 0)
				draw_rectangle(gui.unit * t_vect((i < 3 ? 8 : 18) + ((i % 3) * 1.5), 18.5), gui.unit, t_color(0.3, 0.3, 0.3, 0.7));
			if (player->cooldown[i] != 0)//int type, int outline,
				draw_centred_text(to_string(player->cooldown[i]), text_size, NORMAL, 4, gui.unit * t_vect((i < 3 ? 8 : 18) + ((i % 3) * 1.5), 18.5) + gui.unit / 2, DARK_GREEN, BLACK);
			if (player->spell[i]->cost_pa > 0)
			{
				get_interface_tile("simple_cursor")->draw_self(gui.unit * t_vect((i < 3 ? 8.5 : 18.5) + ((i % 3) * 1.5), 18.1), gui.unit * 0.8, t_vect(0, 3));
				draw_centred_text(to_string(player->spell[i]->cost_pa), 25, gui.unit * t_vect((i < 3 ? 8.5 : 18.5) + ((i % 3) * 1.5), 18.1) + gui.unit * 0.4, BLACK);
			}
			if (player->spell[i]->cost_pm > 0)
			{
				get_interface_tile("simple_cursor")->draw_self(gui.unit * t_vect((i < 3 ? 7.7 : 17.7) + ((i % 3) * 1.5), 18.1), gui.unit * 0.8, t_vect(1, 3));
				draw_centred_text(to_string(player->spell[i]->cost_pm), 25, gui.unit * t_vect((i < 3 ? 7.7 : 17.7) + ((i % 3) * 1.5), 18.1) + gui.unit * 0.4, BLACK);
			}
		}
	}
}

void				s_game_board::draw_cell_info_on_gui()
{
	t_vect mouse = get_mouse_pos();
	int i = 0;

	if (mouse != t_vect(-1, -1))
	{
		t_cell *cell = get_cell(mouse.x, mouse.y);
		t_actor *player = cell->actor;
		size_t j = (player != NULL ? player->effect_list.change_pa.size() + player->effect_list.change_pm.size() + player->effect_list.regeneration.size() + player->effect_list.poison.size() : 0) ;
		draw_border_rectangle(gui.unit * t_vect(22, 0), gui.unit * t_vect(8, (4 + (double)j / 2.0)), 4, t_color(0.3, 0.3, 0.3, 0.6), t_color(0.6, 0.6, 0.6, 0.6));

		string text = "Coord : " + to_string((int)(mouse.x)) + " / " + to_string((int)(mouse.y)) + " - " + (cell ? cell->node->name : "Empty tile");
		static int text_size = calc_text_max_size(text, gui.unit * t_vect(7, 0.5));
		draw_lined_text(text, text_size, gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
		if (cell == NULL || cell->node->m_obs == true)
			text = "Cost to pass : ";
		else
			text = "Cost to pass : " + to_string(cell->node->cost);
		draw_lined_text(text, text_size, gui.unit * t_vect(22.2, 0.5 * i + 0.5), BLACK);
		if (cell == NULL || cell->m_dist == 999)
			text = "Cost to reach : ";
		else
			text = "Cost to reach : " + to_string(cell->m_dist);
		draw_lined_text(text, text_size, gui.unit * t_vect(25.75, 0.5 * i++ + 0.5), BLACK);
		if (cell && cell->actor != NULL && cell->actor->team != 0)
		{
			i++;
			text = "Actor : " + player->name;
			draw_lined_text(text, text_size, gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
			string text = "Health : " + to_string(player->stat.hp.value) + "/" + to_string(player->stat.hp.max);
			draw_lined_text(text, text_size, gui.unit * t_vect(22.2, 0.5 * i + 0.5), BLACK);
			if (player->stat.armor != 0)
			{
				text = "Armor : " + to_string(player->stat.armor);
				draw_lined_text(text, text_size, gui.unit * t_vect(26.1, 0.5 * i + 0.5), BLACK);
			}
			i++;
			text = "Action points : " + to_string(player->stat.pa.value);
			draw_lined_text(text, text_size, gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
			text = "Mouvement points : " + to_string(player->stat.pm.value);
			draw_lined_text(text, text_size, gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
			for (size_t j = 0; j < player->effect_list.poison.size(); j++)
				draw_lined_text(parse_effect_poison_desc(&(player->effect_list.poison[j])), text_size, gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
			for (size_t j = 0; j < player->effect_list.regeneration.size(); j++)
				draw_lined_text(parse_effect_regeneration_desc(&(player->effect_list.regeneration[j])), text_size, gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
			for (size_t j = 0; j < player->effect_list.change_pa.size(); j++)
				draw_lined_text(parse_effect_change_pa_desc(&(player->effect_list.change_pa[j])), text_size, gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
			for (size_t j = 0; j < player->effect_list.change_pm.size(); j++)
				draw_lined_text(parse_effect_change_pm_desc(&(player->effect_list.change_pm[j])), text_size, gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
		}
		else if (cell && cell->actor != NULL && cell->actor->team == 0)
		{
			i++;
			t_actor *player = cell->actor;
			text = "Obstacle : " + player->name;
			draw_lined_text(text, text_size, gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
			string text = "Health : " + to_string(player->stat.hp.value) + "/" + to_string(player->stat.hp.max);
			draw_lined_text(text, text_size, gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
		}
	}
}

int					tmp(int i)
{
	if (i == 3)
		return (1);
	if (i == 4)
		return (0);
	return (i);
}

void				s_game_board::draw_select_wheel(int index)
{
	(void)index;
	if (actor_pool.size() == 0)
		return;

	t_vect coord = t_vect(1, 1);
	t_vect size[] = {t_vect(2, 2), t_vect(2.4, 2.4), t_vect(2.8, 2.8)};
	t_vect space[] = {t_vect(0.4, 0.2), t_vect(0.2, 0.2), t_vect(0, 0.2)};

	int type = get_frame_state(4);
	if (type == 3)
		type = 1;

	for (int i = 0; i < 5; i++)
	{
		draw_border_rectangle(gui.unit * (coord + space[tmp(i)]), gui.unit * size[tmp(i)], 12, t_color(0.5, 0.5, 0.5), t_color(0.4, 0.4, 0.4));
		int tile_index = (index + (i - 2)) % actor_pool.size();
		actor_pool[tile_index]->tile->draw_self(gui.unit * (coord + space[tmp(i)]) + 12, gui.unit * size[tmp(i)] - 24, actor_pool[tile_index]->sprite + t_vect(type, 0));
		coord.y += size[tmp(i)].y + space[tmp(i)].y;
	}

	coord.y += space[tmp(4)].y;
	size[0] = t_vect(4.5, 6);
	draw_border_rectangle(gui.unit * coord, gui.unit * size[0], 12, t_color(0.5, 0.5, 0.5), t_color(0.4, 0.4, 0.4));
	int i = 0;
	t_actor *player = actor_pool[(index) % actor_pool.size()];
	string text = "Actor : " + player->name;
	static int text_size = calc_text_max_size(text, gui.unit * t_vect(4.5, 0.5)) / 1.3;
	draw_lined_text(text, text_size, gui.unit * t_vect(coord.x + 0.25, coord.y + 0.5 * i++ + 0.5), BLACK);
	text = "Health : " + to_string(player->stat.hp.value) + "/" + to_string(player->stat.hp.max);
	draw_lined_text(text, text_size, gui.unit * t_vect(coord.x + 0.25, coord.y + 0.5 * i++ + 0.5), BLACK);
	text = "Action points : " + to_string(player->stat.pa.value);
	draw_lined_text(text, text_size, gui.unit * t_vect(coord.x + 0.25, coord.y + 0.5 * i++ + 0.5), BLACK);
	text = "Mouvement points : " + to_string(player->stat.pm.value);
	draw_lined_text(text, text_size, gui.unit * t_vect(coord.x + 0.25, coord.y + 0.5 * i++ + 0.5), BLACK);
	while (i < 10)
	{
		text = "Spell [" + to_string(i - 4) + "] : " + (player->spell[i - 4]->name != "NULL" ? player->spell[i - 4]->name : "");
		draw_lined_text(text, text_size , gui.unit * t_vect(coord.x + 0.25, coord.y + 0.5 * i++ + 0.5), BLACK);
	}
}

void				s_game_board::draw_gui()
{
	gui.draw_self();
	if (turn_order.size())
		draw_actor_info_on_gui();
	draw_cell_info_on_gui();
	t_vect mouse = get_mouse_coord();
	for (int i = 0; i < 6 ; i++)
	{
		t_actor *player = turn_order[turn_index % turn_order.size()];
		t_button_comp *button = ((t_button *)(gui.object_list[SPELL_BUTTON][i]))->button;
		if (mouse.x > button->coord[0].x && mouse.x < button->coord[0].x + button->size[0].x &&
			mouse.y > button->coord[0].y && mouse.y < button->coord[0].y + button->size[0].y && player->spell[i]->tile != NULL)
			draw_spell_card(player->spell[i], gui.unit * t_vect(0.9, 0.9), gui.unit * t_vect(4.5, 7.5));
	}
}
