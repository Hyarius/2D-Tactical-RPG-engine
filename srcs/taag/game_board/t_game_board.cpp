#include "taag.h"

static void			change_s_spell(t_data data)
{
	int *s_spell = (int *)(data.data[0]);
	bool	*calculated = (bool *)(data.data[1]);
	int value = ((int &)(data.data[2]));

	*calculated = false;
	*s_spell = value;
}

s_game_board::s_game_board()
{
	vector<string>	node_file = list_files(NODE_PATH, NODE_EXT);

	size_t i = 0;
	while (i < node_file.size())
	{
		node_list.push_back(read_node(NODE_PATH + node_file[i] + NODE_EXT));
		i++;
	}
	board_size = t_vect(0, 0);
	zoom = 1.5;
	sprite_unit = t_vect(32, 32);
	offset = get_win_size() / 2;
	target = t_vect(-board_size.x / 2, -board_size.y / 2);
	cursor_tile = get_interface_tile("simple_cursor");
	calculated = false;
	s_spell = -1;
	turn_num = 0;
	gui = t_gui(30, 20);
	gui.add(new s_button(new t_image_button(t_image("ressources/assets/interface/GUI_Shortcut.png"), gui.unit * t_vect(7.5, 18), gui.unit * t_vect(15, 2)), NULL, NULL));
	for (int i = 0; i < 6; i++)
		gui.add(SPELL_BUTTON, new s_button(new t_text_button("", BLACK, gui.unit * t_vect((i < 3 ? 8 : 18) + ((i % 3) * 1.5), 18.5), gui.unit, 0, t_color(0.3, 0.3, 0.3, 0.0), t_color(0.6, 0.6, 0.6, 0.0)), change_s_spell, t_data(3, &s_spell, &calculated, i)));
}

s_game_board::s_game_board(string p_path)
{
	ifstream		myfile;
	vector<string>	tab;
	t_vect			coord;
	size_t			index;

	myfile.open(p_path);
	if (myfile.fail())
		printf("can't open such file : %s\n", p_path.c_str());

	vector<string>	node_file = list_files(NODE_PATH, NODE_EXT);

	size_t i = 0;
	while (i < node_file.size())
	{
		node_list.push_back(read_node(NODE_PATH + node_file[i] + NODE_EXT));
		i++;
	}
	while (!(myfile.eof()))
	{
		tab = get_strsplit(&myfile, ":", -1);
		if (tab.size() < 3)
			break;
		coord = t_vect(atoi(tab[0].c_str()), atoi(tab[1].c_str()));
		index = atoi(tab[2].c_str());
		if (board_size.x <= coord.x)
		{
			board_size.x = coord.x + 1;
			cell_layer.resize((size_t)(board_size.x));
		}
		if (board_size.y <= coord.y)
		{
			board_size.y = coord.y + 1;
			size_t count = 0;
			while (count < cell_layer.size())
			{
				cell_layer[count].resize((size_t)(board_size.y + 1));
				count++;
			}
		}
		if (cell_layer[(size_t)(coord.x)].size() < (size_t)(board_size.y))
			cell_layer[(size_t)(coord.x)].resize((size_t)(board_size.y + 1));
		if (index < node_list.size())
			cell_layer[(size_t)(coord.x)][(size_t)(coord.y)] = t_cell(coord, &(node_list[index]));
		if (tab.size() >= 5)
		{
			int team = atoi(tab[4].c_str());
			t_actor *new_actor;
			if (team == 0)
				new_actor = new t_actor(read_actor(OBS_PATH + tab[3] + OBS_EXT));
			else
				new_actor = new t_actor(read_actor(MONSTER_PATH + tab[3] + ACTOR_EXT));
			new_actor->path = tab[3];
			new_actor->coord = coord;
			new_actor->team = atoi(tab[4].c_str());
			get_cell(coord)->actor = new_actor;
			actor_list.push_back(get_cell(coord)->actor);
			if (get_cell(coord)->actor->team == 2)
				enemy_list.push_back(get_cell(coord)->actor);
			else if (get_cell(coord)->actor->team == 1)
				ally_list.push_back(get_cell(coord)->actor);
		}
	}
	while (!myfile.eof())
	{
		tab = get_strsplit(&myfile, ":", -1);
		if (tab.size() <= 1)
			break;
		int team = atoi(tab[2].c_str());
		if (team == 0)
			placement_list.push_back(t_vect(atoi(tab[0].c_str()), atoi(tab[1].c_str())));
		if (team == 2)
			enemy_placement_list.push_back(t_vect(atoi(tab[0].c_str()), atoi(tab[1].c_str())));
	}
	myfile.close();

	zoom = 1.5;
	sprite_unit = t_vect(32, 32);
	offset = get_win_size() / 2;
	target = t_vect(-board_size.x / 2, -board_size.y / 2);
	cursor_tile = &interface_map["simple_cursor"];
	calculated = false;
	s_spell = -1;
	turn_num = 0;
	gui = t_gui(30, 20);
	gui.add(new s_button(new t_image_button(t_image("ressources/assets/interface/GUI_Shortcut.png"), gui.unit * t_vect(7.5, 18), gui.unit * t_vect(15, 2)), NULL, NULL));
	for (int i = 0; i < 6; i++)
		gui.add(SPELL_BUTTON, new s_button(new t_text_button("", BLACK, gui.unit * t_vect((i < 3 ? 8 : 18) + ((i % 3) * 1.5), 18.5), gui.unit, 0, t_color(0.3, 0.3, 0.3, 0.0), t_color(0.6, 0.6, 0.6, 0.0)), change_s_spell, t_data(3, &s_spell, &calculated, i)));

}

void				s_game_board::draw_cell_border()
{
	t_vect size = sprite_unit * zoom;

	int i = 0;
	while ((size_t)i < board_size.x)
	{
		int j = 0;
		while ((size_t)j < board_size.y)
		{
			//((t_vect(i, j) + target) * size + offset
			t_vect coord = (t_vect(i, j) + target) * size + offset;
			if (i == 0)
				draw_rectangle(coord, t_vect(1, size.y), t_color(0.4, 0.4, 0.4));
			if (j == 0)
				draw_rectangle(coord, t_vect(size.x, 1), t_color(0.4, 0.4, 0.4));
			draw_rectangle(coord + t_vect(size.x, 0), t_vect(1, size.y), t_color(0.4, 0.4, 0.4));
			draw_rectangle(coord + t_vect(0, size.y), t_vect(size.x, 1), t_color(0.4, 0.4, 0.4));
			j++;
		}
		i++;
	}
}

void				s_game_board::draw_animation()
{
	t_vect size = sprite_unit * zoom;

	int i = 0;
	while ((size_t)i < board_size.x)
	{
		int j = 0;
		while ((size_t)j < board_size.y)
		{
			if (get_cell(i, j))
			{
				size_t count = 0;
				while (count < get_cell(i, j)->animation.size())
				{
					get_cell(i, j)->animation[count].draw_self(t_vect(i, j) + 0.5, target, offset, size);
					if (get_cell(i, j)->animation[count].index >= get_cell(i, j)->animation[count].len)
						get_cell(i, j)->animation.erase(get_cell(i, j)->animation.begin() + count);
					else
						count++;
				}
			}
			j++;
		}
		i++;
	}
}

t_cell				*s_game_board::get_cell(int x, int y)
{
	if (x < 0 || x >= board_size.x || y < 0 || y >= board_size.y)
		return (NULL);
	if (cell_layer[x][y].node == NULL)
		return (NULL);
	return (&(cell_layer[x][y]));
}

t_cell				*s_game_board::get_cell(t_vect target)
{
	if ((int)(target.x) < 0 || (int)(target.x) >= board_size.x || (int)(target.y) < 0 || (int)(target.y) >= board_size.y)
		return (NULL);
	if (cell_layer[(int)(target.x)][(int)(target.y)].node == NULL)
		return (NULL);
	return (&(cell_layer[(int)(target.x)][(int)(target.y)]));
}

t_vect				s_game_board::get_mouse_pos()
{
	t_vect mouse = (get_mouse_coord() - offset) / (sprite_unit * zoom) - target;

	if (mouse.x < 0)
		return (t_vect(-1, -1));
	else if (mouse.x >= board_size.x)
		return (t_vect(-1, -1));
	if (mouse.y < 0)
		return (t_vect(-1, -1));
	else if (mouse.y >= board_size.y)
		return (t_vect(-1, -1));

	return (t_vect((int)(mouse.x), (int)(mouse.y)));
}

void				s_game_board::draw_cursor(t_vect coord, t_vect target, t_vect size, t_vect offset, t_vect sprite)
{
	cursor_tile->prepare_print((coord + target) * size + offset, size, sprite);
}

void				s_game_board::draw_self()
{
	t_vect size = sprite_unit * zoom;
	//(coord + target) * size + offset, size, sprite);
		t_tileset *tile= get_interface_tile("simple_board");
	int i = 0;
	while (i < board_size.x)
	{
		int j = 0;
		while (j < board_size.y)
		{
			if (i == 0 && j == 0) // top - left corner
			{
				tile->prepare_print((t_vect(-1 , -1) + target) * size + offset, size, t_vect(0, 0));
				tile->prepare_print((t_vect(-1 , 0) + target) * size + offset, size, t_vect(0, 1));
				tile->prepare_print((t_vect(0 , -1) + target) * size + offset, size, t_vect(1, 0));
				tile->prepare_print((t_vect(0 , 0) + target) * size + offset, size, t_vect(1, 1));
			}
			else if (i == board_size.x - 1 && j == 0) // top - right corner
			{
				tile->prepare_print((t_vect(i , j - 1) + target) * size + offset, size, t_vect(3, 0));
				tile->prepare_print((t_vect(i , j) + target) * size + offset, size, t_vect(3, 1));
				tile->prepare_print((t_vect(i + 1 , j - 1) + target) * size + offset, size, t_vect(4, 0));
				tile->prepare_print((t_vect(i + 1, j) + target) * size + offset, size, t_vect(4, 1));
			}
			else if (i == 0 && j == board_size.y - 1) // down - left corner
			{
				tile->prepare_print((t_vect(i - 1, j) + target) * size + offset, size, t_vect(0, 3));
				tile->prepare_print((t_vect(i - 1, j + 1) + target) * size + offset, size, t_vect(0, 4));
				tile->prepare_print((t_vect(i , j) + target) * size + offset, size, t_vect(1, 3));
				tile->prepare_print((t_vect(i , j + 1) + target) * size + offset, size, t_vect(1, 4));
			}
			else if (i == board_size.x - 1 && j == board_size.y - 1) // down - right corner
			{
				tile->prepare_print((t_vect(i , j) + target) * size + offset, size, t_vect(3, 3));
				tile->prepare_print((t_vect(i , j + 1) + target) * size + offset, size, t_vect(3, 4));
				tile->prepare_print((t_vect(i + 1, j) + target) * size + offset, size, t_vect(4, 3));
				tile->prepare_print((t_vect(i + 1, j + 1) + target) * size + offset, size, t_vect(4, 4));
			}
			else if (i == 0) // left side
			{
				tile->prepare_print((t_vect(i - 1, j) + target) * size + offset, size, t_vect(0, 2));
				tile->prepare_print((t_vect(i , j) + target) * size + offset, size, t_vect(1, 2));
			}
			else if (i == board_size.x - 1) // right side
			{
				tile->prepare_print((t_vect(i , j) + target) * size + offset, size, t_vect(3, 2));
				tile->prepare_print((t_vect(i + 1, j) + target) * size + offset, size, t_vect(4, 2));
			}
			else if (j == 0) // top side
			{
				tile->prepare_print((t_vect(i , j) + target) * size + offset, size, t_vect(2, 1));
				tile->prepare_print((t_vect(i , j - 1) + target) * size + offset, size, t_vect(2, 0));
			}
			else if (j == board_size.y - 1) // down side
			{
				tile->prepare_print((t_vect(i , j) + target) * size + offset, size, t_vect(2, 3));
				tile->prepare_print((t_vect(i , j + 1) + target) * size + offset, size, t_vect(2, 4));
			}
			else
				tile->prepare_print((t_vect(i , j) + target) * size + offset, size, t_vect(2, 2));
			j++;
		}
		i++;
	}
	render_triangle_texture(tile->texture_id);
}


void				s_game_board::add_actor(t_actor *new_actor)
{
	actor_list.push_back(new_actor);
}

void				s_game_board::remove_actor(t_actor *old_actor)
{
	size_t count = 0;

	if (old_actor->team == 2)
	{
		while (count < enemy_list.size() && old_actor != enemy_list[count])
			count++;
		if (count < enemy_list.size())
			enemy_list.erase(enemy_list.begin() + count);
	}
	else if (old_actor->team == 1)
	{
		while (count < ally_list.size() && old_actor != ally_list[count])
			count++;
		if (count < ally_list.size())
			ally_list.erase(ally_list.begin() + count);
	}
	count = 0;
	while (count < actor_list.size() && old_actor != actor_list[count])
		count++;
	if (count < actor_list.size())
	{
		actor_list.erase(actor_list.begin() + count);
	}
}

void				s_game_board::draw_mouse_cursor()
{
	t_vect mouse = get_mouse_pos();

	t_vect size = sprite_unit * zoom;
	if (mouse != t_vect(-1, -1))
		draw_cursor(mouse, target, size, offset, t_vect(1, 0));
	render_triangle_texture(cursor_tile->texture_id);

}

bool				s_game_board::check_anim()
{
	int i = 0;

	while ((size_t)i < board_size.x)
	{
		int j = 0;
		while ((size_t)j < board_size.y)
		{

			if (get_cell(i, j))
			{
				size_t k = 0;
				while (k < get_cell(i, j)->animation.size())
				{
					if (get_cell(i, j)->animation[k].index < get_cell(i, j)->animation[k].len)
						return (false);
					k++;
				}
			}
			j++;
		}
		i++;
	}
	return (true);
}

bool				s_game_board::check_visual()
{
	for (size_t i = 0; i < actor_list.size(); i++)
		if (actor_list[i]->visual_info.size() != 0)
			return (false);
	return (true);
}

void				s_game_board::draw_cell_layer()
{
	int i = 0;
	t_vect size = sprite_unit * zoom;

	while ((size_t)i < board_size.x)
	{
		int j = 0;
		while ((size_t)j < board_size.y)
		{
			cell_layer[i][j].draw_cell(target, offset, size);
			j++;
		}
		i++;
	}
	if (board_size == t_vect(0, 0))
		return;
	render_triangle_texture(node_list[0].tile->texture_id);
}

void				s_game_board::draw_visual_info()
{
	size_t i = 0;
	t_vect size = sprite_unit * zoom;

	for (size_t i = 0; i < actor_list.size(); i++)
	{
		for (int j = 0; j < (int)(actor_list[i]->visual_info.size()); j++)
		{
			actor_list[i]->visual_info[j].draw_self(target, offset, size, zoom);
			if (actor_list[i]->visual_info[j].index >= (int)(actor_list[i]->visual_info[j].text_coord.size()))
			{
				actor_list[i]->visual_info.erase(actor_list[i]->visual_info.begin() + j);
				j--;
			}
		}
	}
}

void				s_game_board::draw_cursor_layer()
{
	int i = 0;
	t_vect size = sprite_unit * zoom;

	while ((size_t)i < board_size.x)
	{
		int j = 0;
		while ((size_t)j < board_size.y)
		{
			if (cell_layer[i][j].cursor != t_vect(0, 0))
				draw_cursor(t_vect(i, j), target, size, offset, cell_layer[i][j].cursor);
			j++;
		}
		i++;
	}
	render_triangle_texture(interface_map["simple_cursor"].texture_id);
}

void				s_game_board::draw_placement()
{
	t_vect size = sprite_unit * zoom;

	for (size_t i = 0; i < placement_list.size(); i++)
		cursor_tile->draw_self((placement_list[i] + target) * size + offset, size, t_vect(3, 0));
	for (size_t i = 0; i < enemy_placement_list.size(); i++)
		cursor_tile->draw_self((enemy_placement_list[i] + target) * size + offset, size, t_vect(4, 0));
}

void				s_game_board::reset_board()
{
	int i = 0;

	while ((size_t)i < board_size.x)
	{
		int j = 0;
		while ((size_t)j < board_size.y)
		{
			cell_layer[i][j].m_dist = 999;
			cell_layer[i][j].v_dist = 999;
			cell_layer[i][j].cursor = t_vect(0, 0);
			j++;
		}
		i++;
	}
}

void				s_game_board::draw_actor_list()
{
	size_t i = 0;
	t_vect size = sprite_unit * zoom;

	while (i < actor_list.size())
	{
		actor_list[i]->draw_self(target, offset, size);
		i++;
	}
}


void				s_game_board::handle_mouvement(SDL_Event *event)
{
	if (event->type == SDL_MOUSEMOTION && event->button.button == SDL_BUTTON_MIDDLE)
	{
		target.x += (double)(event->motion.xrel) / (sprite_unit.x * zoom);
		target.y += (double)(event->motion.yrel) / (sprite_unit.y * zoom);
		SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
	}
	else
		SDL_EventState(SDL_MOUSEBUTTONUP, SDL_ENABLE);
}

void				s_game_board::handle_zoom(SDL_Event *event)
{
	if (event->type == SDL_MOUSEWHEEL)
	{
		if (event->wheel.y > 0 && zoom < 10)
			zoom *= 1.1;
		else if (event->wheel.y < 0 && zoom >= 1)
			zoom *= 0.9;
	}
}

void				s_game_board::insert_actor(t_actor *new_actor)
{
	size_t count = 0;
	while (count < turn_order.size() && new_actor->stat.init < turn_order[count]->stat.init)
		count++;
	turn_order.insert(turn_order.begin() + count, new_actor);
}

void				s_game_board::invoke_actor(t_actor *new_actor, t_vect coord)
{
	if (get_cell(coord) && get_cell(coord)->actor == NULL)
	{
		new_actor->coord = coord;
		get_cell(coord)->actor = new_actor;
		add_actor(new_actor);
		if (new_actor->team == 1)
			ally_list.push_back(new_actor);
	}
}

void				s_game_board::outvoke_actor(t_actor *new_actor)
{
	if (get_cell(new_actor->coord) && get_cell(new_actor->coord)->actor != NULL)
	{
		get_cell(new_actor->coord)->actor = NULL;
		new_actor->coord = t_vect(-1, -1);
		remove_actor(new_actor);
		delete new_actor;
	}
}

void				s_game_board::delete_actor(t_actor *new_actor)
{
	remove_actor(new_actor);
	size_t count = 0;
	while (count < turn_order.size() && new_actor != turn_order[count])
		count++;
	if (turn_index >= count)
		turn_index--;
	if (count < turn_order.size())
		turn_order.erase(turn_order.begin() + count);
	get_cell(new_actor->coord)->actor = NULL;
	delete new_actor;
}

void				s_game_board::check_alive()
{
	size_t i = 0;
	while (i < actor_list.size())
	{
		if (actor_list[i]->stat.hp.value <= 0 && check_visual() == true && check_anim() == true)
			delete_actor(actor_list[i]);
		else
			i++;
	}
}

void				s_game_board::initiate_turn_order()
{
	turn_index = 0;

	size_t i = 0;
	while (i < actor_list.size())
	{
		if (actor_list[i]->team >= 1 && actor_list[i]->team < 4)
			insert_actor(actor_list[i]);
		i++;
	}
	if (turn_order.size())
		turn_order[turn_index % turn_order.size()]->selected = true;
	if (turn_order[turn_index % turn_order.size()]->team != 1)
	{
		enemy_turn();
	}
	else
	{
		reset_board();
		if (turn_order.size())
			calculate_distance(turn_order[turn_index % turn_order.size()]->coord);
	}
}

void				s_game_board::update_board()
{
	size_t i = 0;

	check_alive();
	t_actor *player = turn_order[turn_index % turn_order.size()];
	while (i < turn_order.size())
	{
		if (turn_order[i]->destination.size() != 0)
		{
			if (turn_order[i]->destination[0].x > turn_order[i]->coord.x && turn_order[i] == player)
				turn_order[i]->dir = 2;
			else if (turn_order[i]->destination[0].x < turn_order[i]->coord.x && turn_order[i] == player)
				turn_order[i]->dir = 1;
			else if (turn_order[i]->destination[0].y < turn_order[i]->coord.y && turn_order[i] == player)
				turn_order[i]->dir = 3;
			else if (turn_order[i]->destination[0].y > turn_order[i]->coord.y && turn_order[i] == player)
				turn_order[i]->dir = 0;
			turn_order[i]->coord = turn_order[i]->destination[0];
			turn_order[i]->destination.erase(turn_order[i]->destination.begin());
			calculated = false;
		}
		i++;
	}
	if (player->team == 1)
	{
		if (turn_order.size() && player->destination.size() == 0 && calculated == false && s_spell == -1)
			calculate_distance(turn_order[turn_index % turn_order.size()]->coord);
		if (turn_order.size() && player->destination.size() == 0 && s_spell != -1 && calculated == false)
		{
			if (player->spell[s_spell]->range_type == R_CIRCLE)
				calculate_vision_circle();
			else
				calculate_vision_line();
		}
		calculate_zone();
	}
}

void				s_game_board::handle_control_camera(SDL_Event *event)
{
	handle_mouvement(event);
	handle_zoom(event);
}

bool				s_game_board::cast_spell(t_vect mouse)
{
	t_actor *player = turn_order[turn_index % turn_order.size()];
	if (get_cell(mouse) == NULL || get_cell(mouse)->node == NULL ||
		(player->spell[s_spell]->on_target == 0 && get_cell(mouse)->actor == NULL) ||
		(player->spell[s_spell]->on_target == 2 && get_cell(mouse)->actor != NULL) ||
		player->cooldown[s_spell] != 0)
		return (false) ;
	if (get_cell(mouse)->v_dist >= player->spell[s_spell]->range[0] &&
		get_cell(mouse)->v_dist <= player->spell[s_spell]->range[1] &&
		player->stat.pm.value >= player->spell[s_spell]->cost_pm &&
		player->stat.pa.value >= player->spell[s_spell]->cost_pa)
	{
		turn_order[turn_index % turn_order.size()]->apply_effect(1);
		if (player->spell[s_spell]->cost_pa > 0)
			player->change_stat_pa(-(player->spell[s_spell]->cost_pa));
		if (player->spell[s_spell]->cost_pm > 0)
			player->change_stat_pm(-(player->spell[s_spell]->cost_pm));
		size_t i = 0;
		vector<t_vect>	target_list;
		if (player->spell[s_spell]->zone_type == Z_DIAM)
			target_list = calc_diam(player->spell[s_spell]->zone_size);
		else if (player->spell[s_spell]->zone_type == Z_CROSS)
			target_list = calc_cross(player->spell[s_spell]->zone_size);
		else if (player->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = player->coord - mouse;
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
								((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			target_list = calc_line(player->spell[s_spell]->zone_size, dir);
		}
		else if (player->spell[s_spell]->zone_type == Z_SQUARE)
			target_list = calc_square(player->spell[s_spell]->zone_size);
		if (player->spell[s_spell]->anim_type == 0)
			get_cell(mouse)->animation.push_back(player->spell[s_spell]->target_anim);
		while (i < target_list.size())
		{
			size_t j = 0;
			while (j < player->spell[s_spell]->effect.size())
			{
				if (player->spell[s_spell]->effect[j].effect == move_caster)
					set_coord_target(mouse + target_list[i]);
				if (get_cell(mouse + target_list[i]) && player->spell[s_spell]->effect[j].effect != NULL)
				{
					player->spell[s_spell]->effect[j].effect(player, get_cell(mouse + target_list[i])->actor, player->spell[s_spell]->effect[j].stat);
					if (player->spell[s_spell]->anim_type == 1 || (player->spell[s_spell]->anim_type == 2 && get_cell(mouse + target_list[i])->actor != NULL))
						get_cell(mouse + target_list[i])->animation.push_back(player->spell[s_spell]->target_anim);
				}
				j++;
			}
			i++;
		}
		player->cooldown[s_spell] = player->spell[s_spell]->cooldown;
		s_spell = -1;
		calculated = false;
	}
	return (true);
}

void				s_game_board::move_actor(t_actor *player, t_vect dest, double speed)
{
	if (get_cell(dest.x, dest.y) &&
		((get_cell(dest.x, dest.y)->m_dist != 999 ||
		(get_cell(dest.x, dest.y)->m_dist == 999 && get_cell(dest.x, dest.y)->actor != NULL)) || speed != 1))
	{
		player->destination = pathfinding(player, dest, speed);
		if (player->destination.size() != 0)
		{
			t_vect final_dest = player->destination[player->destination.size() - 1];
			if (speed == 1)
			{
				player->change_stat_pm(-get_cell(final_dest)->m_dist);
				player->apply_effect(2);
			}
			get_cell(final_dest)->actor = player;
			get_cell(player->coord)->actor = NULL;
			reset_board();

		}
	}
}

static vector<t_vect>	AI_calc_diam(int size_min, int size_max)
{
	vector<t_vect>	result;

	int i = -size_max;
	while (i <= size_max)
	{
		int j = -(size_max - abs(i));
		while (abs(i) + j <= size_max)
		{
			if (abs(i) + abs(j) >= size_min)
				result.push_back(t_vect(i, j));
			j++;
		}
		i++;
	}

	return (result);
}

static vector<t_vect>	AI_calc_cross(int size_min, int size_max)
{
	vector<t_vect>	result;

	int i = -size_max;
	while (i <= size_max)
	{
		if (abs(i) >= size_min)
		{
			result.push_back(t_vect(0, i));
			if (i != 0)
				result.push_back(t_vect(i, 0));
		}
		i++;
	}

	return (result);
}

bool				s_game_board::get_close_enemy_percent(t_ai_helper data)
{
	if (turn_order[turn_index % turn_order.size()]->stat.pm.value == 0)
		return (false);
	int old_pm = turn_order[turn_index % turn_order.size()]->stat.pm.value;
	int distance = data.value[1];
	int delta = data.value[2];
	int type = data.value[3];
	calculate_distance(turn_order[turn_index % turn_order.size()]->coord);
	vector<t_vect> 	to_calc;
	size_t i = 0, j;
	t_actor *saved = NULL;
	while (i < turn_order.size())
	{
		if (turn_order[i]->team != turn_order[turn_index % turn_order.size()]->team && (saved == NULL || saved->stat.hp.percent() > turn_order[i]->stat.hp.percent()))
			saved = turn_order[i];
		i++;
	}
	if (saved == NULL)
		return (false);
	to_calc.push_back(saved->coord);
	vector<t_vect>	range;
	if (type == R_CIRCLE)
		range = AI_calc_diam(distance - delta, distance);
	if (type == R_LINE)
		range = AI_calc_cross(distance - delta, distance);
	t_vect result = t_vect(-1, -1);
	j = 999;
	i = 0;
	while (i < to_calc.size())
	{
		size_t count = 0;
		while (count < range.size())
		{
			if (get_cell(to_calc[i] + range[count]) && get_cell(to_calc[i] + range[count])->node->m_obs == false && get_cell(to_calc[i] + range[count])->m_dist != 999)
			{
				if (get_cell(to_calc[i] + range[count])->actor == NULL || get_cell(to_calc[i] + range[count])->actor == turn_order[turn_index % turn_order.size()] || get_cell(to_calc[i] + range[count])->actor->team != turn_order[turn_index % turn_order.size()]->team)
				{
					get_cell(to_calc[i] + range[count])->cursor = t_vect(2, 2);
					vector<t_vect> path = calc_path(to_calc[i] + range[count]);
					if (path.size() < j)
					{
						j = path.size();
						result = to_calc[i] + range[count];
					}
				}
			}
			count++;
		}
		i++;
	}

	vector<t_vect> path = calc_path(result);
	if (data.value[4] != -1 && (int)(path.size()) >= data.value[4] + 1)
		result = path[data.value[4]];
	if (get_cell(turn_order[turn_index % turn_order.size()]->coord)->cursor != t_vect(2, 2) && result != t_vect(-1, -1))
	{
		move_actor(turn_order[turn_index % turn_order.size()], result, 1);
		if (turn_order[turn_index % turn_order.size()]->stat.pm.value == old_pm)
			return (false);
		return (true);
	}
	else
		return (false);
}

bool				s_game_board::get_close_enemy_weak(t_ai_helper data)
{
	if (turn_order[turn_index % turn_order.size()]->stat.pm.value == 0)
		return (false);
	int old_pm = turn_order[turn_index % turn_order.size()]->stat.pm.value;
	int distance = data.value[1];
	int delta = data.value[2];
	int type = data.value[3];
	calculate_distance(turn_order[turn_index % turn_order.size()]->coord);
	vector<t_vect>	range;
	vector<t_vect> 	to_calc;
	size_t i = 0, j;
	t_actor *saved = NULL;
	while (i < turn_order.size())
	{
		if (turn_order[i]->team != turn_order[turn_index % turn_order.size()]->team && (saved == NULL || saved->stat.hp.value > turn_order[i]->stat.hp.value))
			saved = turn_order[i];
		i++;
	}
	if (saved == NULL)
		return (false);
	to_calc.push_back(saved->coord);
	if (type == R_CIRCLE)
		range = AI_calc_diam(distance - delta, distance);
	if (type == R_LINE)
		range = AI_calc_cross(distance - delta, distance);
	t_vect result = t_vect(-1, -1);
	j = 999;
	i = 0;
	while (i < to_calc.size())
	{
		size_t count = 0;
		while (count < range.size())
		{
			if (get_cell(to_calc[i] + range[count]) && get_cell(to_calc[i] + range[count])->node->m_obs == false && get_cell(to_calc[i] + range[count])->m_dist != 999)
			{
				if (get_cell(to_calc[i] + range[count])->actor == NULL || get_cell(to_calc[i] + range[count])->actor == turn_order[turn_index % turn_order.size()] || get_cell(to_calc[i] + range[count])->actor->team != turn_order[turn_index % turn_order.size()]->team)
				{
					get_cell(to_calc[i] + range[count])->cursor = t_vect(2, 2);
					vector<t_vect> path = calc_path(to_calc[i] + range[count]);
					if (path.size() < j)
					{
						j = path.size();
						result = to_calc[i] + range[count];
					}
				}
			}
			count++;
		}
		i++;
	}

	vector<t_vect> path = calc_path(result);
	if (data.value[4] != -1 && (int)(path.size()) >= data.value[4] + 1)
		result = path[data.value[4]];
	if (get_cell(turn_order[turn_index % turn_order.size()]->coord)->cursor != t_vect(2, 2) && result != t_vect(-1, -1))
	{
		move_actor(turn_order[turn_index % turn_order.size()], result, 1);
		if (turn_order[turn_index % turn_order.size()]->stat.pm.value == old_pm)
			return (false);
		return (true);
	}
	else
		return (false);
}

bool				s_game_board::get_close_enemy(t_ai_helper data)
{
	if (turn_order[turn_index % turn_order.size()]->stat.pm.value == 0)
		return (false);
	int old_pm = turn_order[turn_index % turn_order.size()]->stat.pm.value;
	int distance = data.value[1];
	int delta = data.value[2];
	int type = data.value[3];
	calculate_distance(turn_order[turn_index % turn_order.size()]->coord);
	vector<t_vect>	range;
	vector<t_vect> 	to_calc;
	size_t i = 0, j;
	while (i < turn_order.size())
	{
		if (turn_order[i]->team != turn_order[turn_index % turn_order.size()]->team)
			to_calc.push_back(turn_order[i]->coord);
		i++;
	}
	if (type == R_CIRCLE)
		range = AI_calc_diam(distance - delta, distance);
	if (type == R_LINE)
		range = AI_calc_cross(distance - delta, distance);
	t_vect result = t_vect(-1, -1);
	j = 999;
	i = 0;
	while (i < to_calc.size())
	{
		size_t count = 0;
		while (count < range.size())
		{
			if (get_cell(to_calc[i] + range[count]) && get_cell(to_calc[i] + range[count])->node->m_obs == false && get_cell(to_calc[i] + range[count])->m_dist != 999)
			{
				if (get_cell(to_calc[i] + range[count])->actor == NULL || get_cell(to_calc[i] + range[count])->actor == turn_order[turn_index % turn_order.size()] || get_cell(to_calc[i] + range[count])->actor->team != turn_order[turn_index % turn_order.size()]->team)
				{
					get_cell(to_calc[i] + range[count])->cursor = t_vect(2, 2);
					vector<t_vect> path = calc_path(to_calc[i] + range[count]);
					if (path.size() < j)
					{
						j = path.size();
						result = to_calc[i] + range[count];
					}
				}
			}
			count++;
		}
		i++;
	}

	vector<t_vect> path = calc_path(result);
	if (data.value[4] != -1 && (int)(path.size()) >= data.value[4] + 1)
		result = path[data.value[4]];
	if (get_cell(turn_order[turn_index % turn_order.size()]->coord)->cursor != t_vect(2, 2) && result != t_vect(-1, -1))
	{
		move_actor(turn_order[turn_index % turn_order.size()], result, 1);
		if (turn_order[turn_index % turn_order.size()]->stat.pm.value == old_pm)
			return (false);
		return (true);
	}
	else
		return (false);
}

bool				s_game_board::get_close_ally(t_ai_helper data)
{
	if (turn_order[turn_index % turn_order.size()]->stat.pm.value == 0)
		return (false);
	int old_pm = turn_order[turn_index % turn_order.size()]->stat.pm.value;
	int distance = data.value[1];
	int delta = data.value[2];
	int type = data.value[3];
	calculate_distance(turn_order[turn_index % turn_order.size()]->coord);
	vector<t_vect>	range;
	vector<t_vect> 	to_calc;
	size_t i = 0, j;
	while (i < turn_order.size())
	{
		if (turn_order[i] != turn_order[turn_index % turn_order.size()] &&
			turn_order[i]->team == turn_order[turn_index % turn_order.size()]->team)
			to_calc.push_back(turn_order[i]->coord);
		i++;
	}
	if (type == R_CIRCLE)
		range = AI_calc_diam(distance - 1, distance);
	if (type == R_LINE)
		range = AI_calc_cross(distance - 1, distance);
	t_vect result = t_vect(-1, -1);
	j = 999;
	i = 0;
	while (i < to_calc.size())
	{
		size_t count = 0;
		while (count < range.size())
		{
			if (get_cell(to_calc[i] + range[count]) && get_cell(to_calc[i] + range[count])->node->m_obs == false)
			{
				if (get_cell(to_calc[i] + range[count])->actor == NULL || get_cell(to_calc[i] + range[count])->actor == turn_order[turn_index % turn_order.size()] || get_cell(to_calc[i] + range[count])->actor->team == turn_order[turn_index % turn_order.size()]->team)
				{
					get_cell(to_calc[i] + range[count])->cursor = t_vect(2, 2);
					vector<t_vect> path = calc_path(to_calc[i] + range[count]);
					if (path.size() < j)
					{
						j = path.size();
						result = to_calc[i] + range[count];
					}
				}
			}
			count++;
		}
		i++;
	}

	vector<t_vect> path = calc_path(result);
	if (data.value[4] != -1 && (int)(path.size()) >= data.value[4] + 1)
		result = path[data.value[4]];
	if (get_cell(turn_order[turn_index % turn_order.size()]->coord)->cursor != t_vect(2, 2) && result != t_vect(-1, -1))
	{
		move_actor(turn_order[turn_index % turn_order.size()], result, 1);
		if (turn_order[turn_index % turn_order.size()]->stat.pm.value == old_pm)
			return (false);
		return (true);
	}
	else
		return (false);
}

bool				s_game_board::get_close_ally_percent(t_ai_helper data)
{
	if (turn_order[turn_index % turn_order.size()]->stat.pm.value == 0)
		return (false);
	int old_pm = turn_order[turn_index % turn_order.size()]->stat.pm.value;
	int distance = data.value[1];
	int delta = data.value[2];
	int type = data.value[3];
	calculate_distance(turn_order[turn_index % turn_order.size()]->coord);
	vector<t_vect>	range;
	vector<t_vect> 	to_calc;
	size_t i = 0, j;
	t_actor *saved = NULL;
	while (i < turn_order.size())
	{
		if (turn_order[i] != turn_order[turn_index % turn_order.size()] &&
			turn_order[i]->team == turn_order[turn_index % turn_order.size()]->team &&
			(saved == NULL || saved->stat.hp.percent() > turn_order[i]->stat.hp.percent()))
			saved = turn_order[i];
		i++;
	}
	if (saved == NULL)
		return (false);
	to_calc.push_back(saved->coord);
	if (type == R_CIRCLE)
		range = AI_calc_diam(distance - 1, distance);
	if (type == R_LINE)
		range = AI_calc_cross(distance - 1, distance);
	t_vect result = t_vect(-1, -1);
	j = 999;
	i = 0;
	while (i < to_calc.size())
	{
		size_t count = 0;
		while (count < range.size())
		{
			if (get_cell(to_calc[i] + range[count]) && get_cell(to_calc[i] + range[count])->node->m_obs == false)
			{
				if (get_cell(to_calc[i] + range[count])->actor == NULL || get_cell(to_calc[i] + range[count])->actor == turn_order[turn_index % turn_order.size()] || get_cell(to_calc[i] + range[count])->actor->team == turn_order[turn_index % turn_order.size()]->team)
				{
					get_cell(to_calc[i] + range[count])->cursor = t_vect(2, 2);
					vector<t_vect> path = calc_path(to_calc[i] + range[count]);
					if (path.size() < j)
					{
						j = path.size();
						result = to_calc[i] + range[count];
					}
				}
			}
			count++;
		}
		i++;
	}

	vector<t_vect> path = calc_path(result);
	if (data.value[4] != -1 && (int)(path.size()) >= data.value[4] + 1)
		result = path[data.value[4]];
	if (get_cell(turn_order[turn_index % turn_order.size()]->coord)->cursor != t_vect(2, 2) && result != t_vect(-1, -1))
	{
		move_actor(turn_order[turn_index % turn_order.size()], result, 1);
		if (turn_order[turn_index % turn_order.size()]->stat.pm.value == old_pm)
			return (false);
		return (true);
	}
	else
		return (false);
}

bool				s_game_board::get_close_ally_damaged(t_ai_helper data)
{
	if (turn_order[turn_index % turn_order.size()]->stat.pm.value == 0)
		return (false);
	int old_pm = turn_order[turn_index % turn_order.size()]->stat.pm.value;
	int distance = data.value[2];
	int delta = data.value[3];
	int type = data.value[4];
	calculate_distance(turn_order[turn_index % turn_order.size()]->coord);
	vector<t_vect>	range;
	vector<t_vect> 	to_calc;
	size_t i = 0, j;
	t_actor *saved = NULL;
	while (i < turn_order.size())
	{
		if (turn_order[i] != turn_order[turn_index % turn_order.size()] &&
			turn_order[i]->team == turn_order[turn_index % turn_order.size()]->team &&
			(saved == NULL || (saved->stat.hp.percent() > turn_order[i]->stat.hp.percent() && turn_order[i]->stat.hp.percent() < data.value[1])))
			saved = turn_order[i];
		i++;
	}
	if (saved == NULL)
		return (false);
	to_calc.push_back(saved->coord);
	if (type == R_CIRCLE)
		range = AI_calc_diam(distance - 1, distance);
	if (type == R_LINE)
		range = AI_calc_cross(distance - 1, distance);
	t_vect result = t_vect(-1, -1);
	j = 999;
	i = 0;
	while (i < to_calc.size())
	{
		size_t count = 0;
		while (count < range.size())
		{
			if (get_cell(to_calc[i] + range[count]) && get_cell(to_calc[i] + range[count])->node->m_obs == false)
			{
				if (get_cell(to_calc[i] + range[count])->actor == NULL || get_cell(to_calc[i] + range[count])->actor == turn_order[turn_index % turn_order.size()] || get_cell(to_calc[i] + range[count])->actor->team == turn_order[turn_index % turn_order.size()]->team)
				{
					get_cell(to_calc[i] + range[count])->cursor = t_vect(2, 2);
					vector<t_vect> path = calc_path(to_calc[i] + range[count]);
					if (path.size() < j)
					{
						j = path.size();
						result = to_calc[i] + range[count];
					}
				}
			}
			count++;
		}
		i++;
	}

	vector<t_vect> path = calc_path(result);
	if (data.value[5] != -1 && (int)(path.size()) >= data.value[5] + 1)
		result = path[data.value[5]];
	if (get_cell(turn_order[turn_index % turn_order.size()]->coord)->cursor != t_vect(2, 2) && result != t_vect(-1, -1))
	{
		move_actor(turn_order[turn_index % turn_order.size()], result, 1);
		if (turn_order[turn_index % turn_order.size()]->stat.pm.value == old_pm)
			return (false);
		return (true);
	}
	else
		return (false);
}

bool				s_game_board::flee_enemy(t_ai_helper data)
{
	if (turn_order[turn_index % turn_order.size()]->stat.pm.value == 0)
		return (false);
	if (turn_order[turn_index % turn_order.size()]->stat.hp.percent() > data.value[1])
		return (false);
	int old_pm = turn_order[turn_index % turn_order.size()]->stat.pm.value;
	vector<vector<int>> tab;
	vector<t_vect>	to_calc;
	tab.resize(board_size.x);
	int i = 0, j;
	while (i < board_size.x)
	{
		tab[i].resize(board_size.y);
		j = 0;
		while (j < board_size.y)
		{
			if (get_cell(i, j) && (get_cell(i, j)->actor == NULL || get_cell(i, j)->actor->team == turn_order[turn_index % turn_order.size()]->team))
				tab[i][j] = 0;
			else
				tab[i][j] = board_size.x + board_size.y + 1;
			j++;
		}
		i++;
	}
	int min = 999;
	i = 0;
	int find;
	while (i < board_size.x)
	{
		j = 0;
		while (j < board_size.y)
		{
			find = 0;
			if (tab[i][j] != 0)
			{
				if (get_cell(i + 1, j) && (tab[i + 1][j] == 0 || tab[i + 1][j] < tab[i][j] - 1) && get_cell(i + 1, j)->node->m_obs == false && (find = 1))
					tab[i + 1][j] = tab[i][j] - 1;
				if (get_cell(i - 1, j) && (tab[i - 1][j] == 0 || tab[i - 1][j] < tab[i][j] - 1) && get_cell(i - 1, j)->node->m_obs == false && (find = 1))
					tab[i - 1][j] = tab[i][j] - 1;
				if (get_cell(i, j + 1) && (tab[i][j + 1] == 0 || tab[i][j + 1] < tab[i][j] - 1) && get_cell(i, j + 1)->node->m_obs == false && (find = 1))
					tab[i][j + 1] = tab[i][j] - 1;
				if (get_cell(i, j - 1) && (tab[i][j - 1] == 0 || tab[i][j - 1] < tab[i][j] - 1) && get_cell(i, j - 1)->node->m_obs == false && (find = 1))
					tab[i][j - 1] = tab[i][j] - 1;
			}
			if (min > tab[i][j])
				min = tab[i][j];
			if (find == 1)
			{
				i = 0;
				j = 0;
			}
			else
				j++;
		}
		i++;
	}
	t_vect			actual = turn_order[turn_index % turn_order.size()]->coord;
	t_vect			to_look;
	find = 1;
	while (tab[actual.x][actual.y] != min && find == 1)
	{
		find = 0;
		to_look = actual;
		if (get_cell(actual + t_vect(1, 0)) && tab[to_look.x][to_look.y] > tab[actual.x + 1][actual.y] && tab[actual.x + 1][actual.y] != 0)
		{
			to_look = t_vect(actual.x + 1, actual.y);
			find = 1;
		}
		if (get_cell(actual + t_vect(-1, 0)) && tab[to_look.x][to_look.y] > tab[actual.x - 1][actual.y] && tab[actual.x - 1][actual.y] != 0)
		{
			to_look = t_vect(actual.x - 1, actual.y);
			find = 1;
		}
		if (get_cell(actual + t_vect(0, 1)) && tab[to_look.x][to_look.y] > tab[actual.x][actual.y + 1] && tab[actual.x][actual.y + 1] != 0)
		{
			to_look = t_vect(actual.x, actual.y + 1);
			find = 1;
		}
		if (get_cell(actual + t_vect(0, -1)) && tab[to_look.x][to_look.y] > tab[actual.x][actual.y - 1] && tab[actual.x][actual.y - 1] != 0)
		{
			to_look = t_vect(actual.x, actual.y - 1);
			find = 1;
		}
		actual = to_look;
	}
	calculate_distance(turn_order[turn_index % turn_order.size()]->coord);

	if (get_cell(turn_order[turn_index % turn_order.size()]->coord)->cursor != t_vect(2, 2) && actual != t_vect(-1, -1))
	{
		move_actor(turn_order[turn_index % turn_order.size()], actual, 1);
		if (turn_order[turn_index % turn_order.size()]->stat.pm.value == old_pm)
			return (false);
		return (true);
	}
	else
		return (false);
}

bool				s_game_board::attack(t_ai_helper data)
{
	s_spell = data.value[1];
	t_actor *actor = turn_order[turn_index % turn_order.size()];
	if (actor->stat.pa.value < actor->spell[s_spell]->cost_pa ||
		actor->stat.pm.value < actor->spell[s_spell]->cost_pm ||
		(data.value[2] != -1 && actor->stat.pa.value != data.value[2]) ||
		(data.value[3] != -1 && actor->stat.pm.value != data.value[3]))
		return (false);
	reset_board();
	calculated = false;
	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		calculate_vision_circle();
	else
		calculate_vision_line();
	size_t i = 0;

	vector<t_vect>	target_list; //every tile hited by the spell
	if (actor->spell[s_spell]->zone_type == Z_DIAM)
		target_list = calc_diam(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_CROSS)
		target_list = calc_cross(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_SQUARE)
		target_list = calc_square(actor->spell[s_spell]->zone_size);


	vector<t_vect> to_look; //every tile to look

	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		to_look = AI_calc_diam(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);
	if (actor->spell[s_spell]->range_type == R_LINE)
		to_look = AI_calc_cross(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);

	t_vect target = t_vect(-1, -1); //the saved coord of the spell
	int		utils_value = 999;

	for (size_t i = 0; i < to_look.size(); i++)
	{
		int nb_target_hit = 0;
		int percent_target_hit = 100;
		if (actor->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = actor->coord - to_look[i];
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
				((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			target_list = calc_line(actor->spell[s_spell]->zone_size, dir);
		}
		for (size_t j = 0; j < target_list.size(); j++)
		{
			t_vect tmp = actor->coord + to_look[i] + target_list[j];
			if (get_cell(tmp) && get_cell(actor->coord + to_look[i]) && get_cell(actor->coord + to_look[i])->cursor == t_vect(0, 2) && get_cell(tmp)->actor != NULL)
			{
				if (get_cell(tmp)->actor->team != actor->team)
				{
					if (percent_target_hit >= get_cell(tmp)->actor->stat.hp.percent())
						percent_target_hit = get_cell(tmp)->actor->stat.hp.percent();
					nb_target_hit++;
				}
			}
		}
		if (nb_target_hit != 0 && percent_target_hit / nb_target_hit < utils_value)
		{
			utils_value = percent_target_hit / nb_target_hit;
			target = actor->coord + to_look[i];
		}
	}
	if (target == t_vect(-1, -1))
		return (false);
	if (cast_spell(target) == false)
		return (false);
	reset_board();
	return (true);
}

bool				s_game_board::attack_weak(t_ai_helper data)
{
	s_spell = data.value[1];
	t_actor *actor = turn_order[turn_index % turn_order.size()];
	if (actor->stat.pa.value < actor->spell[s_spell]->cost_pa ||
		actor->stat.pm.value < actor->spell[s_spell]->cost_pm ||
		(data.value[2] != -1 && actor->stat.pa.value != data.value[2]) ||
		(data.value[3] != -1 && actor->stat.pm.value != data.value[3]))
		return (false);
	reset_board();
	calculated = false;
	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		calculate_vision_circle();
	else
		calculate_vision_line();
	size_t i = 0;
	t_actor *target_actor = NULL;
	while (i < turn_order.size())
	{
		if ((target_actor == NULL || (get_cell(turn_order[i]->coord)->v_dist < get_cell(target_actor->coord)->v_dist && turn_order[i]->stat.hp.value < target_actor->stat.hp.value)) &&
			turn_order[i]->team != turn_order[turn_index % turn_order.size()]->team && get_cell(turn_order[i]->coord)->cursor == t_vect(0, 2))
			target_actor = turn_order[i];
		i++;
	}
	if (target_actor == NULL)
		return (false);
	i = 0;

	vector<t_vect>	target_list; //every tile hited by the spell
	if (actor->spell[s_spell]->zone_type == Z_DIAM)
		target_list = calc_diam(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_CROSS)
		target_list = calc_cross(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_SQUARE)
		target_list = calc_square(actor->spell[s_spell]->zone_size);


	vector<t_vect> to_look; //every tile to look

	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		to_look = AI_calc_diam(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);
	if (actor->spell[s_spell]->range_type == R_LINE)
		to_look = AI_calc_cross(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);

	t_vect target = t_vect(-1, -1); //the saved coord of the spell
	int		utils_value = 999;

	for (size_t i = 0; i < to_look.size(); i++)
	{
		int find = 0;
		int nb_target_hit = 0;
		int percent_target_hit = 100;
		if (actor->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = actor->coord - to_look[i];
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
				((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			target_list = calc_line(actor->spell[s_spell]->zone_size, dir);
		}
		for (size_t j = 0; j < target_list.size(); j++)
		{
			t_vect tmp = actor->coord + to_look[i] + target_list[j];
			if (get_cell(tmp) && get_cell(actor->coord + to_look[i]) && get_cell(actor->coord + to_look[i])->cursor == t_vect(0, 2) && get_cell(tmp)->actor != NULL)
			{
				if (get_cell(tmp)->actor == target_actor)
					find = 1;
				if (get_cell(tmp)->actor->team != actor->team)
				{
					if (percent_target_hit >= get_cell(tmp)->actor->stat.hp.value)
						percent_target_hit = get_cell(tmp)->actor->stat.hp.value;
					nb_target_hit++;
				}
			}
		}
		if (nb_target_hit != 0 && percent_target_hit / nb_target_hit < utils_value && find == 1)
		{
			utils_value = percent_target_hit / nb_target_hit;
			target = actor->coord + to_look[i];
		}
	}
	if (target == t_vect(-1, -1))
		return (false);
	if (cast_spell(target) == false)
		return (false);
	reset_board();
	return (true);
}

bool				s_game_board::attack_percent(t_ai_helper data)
{
	s_spell = data.value[1];
	t_actor *actor = turn_order[turn_index % turn_order.size()];

	if (actor->stat.pa.value < actor->spell[s_spell]->cost_pa ||
		actor->stat.pm.value < actor->spell[s_spell]->cost_pm ||
		(data.value[2] != -1 && actor->stat.pa.value != data.value[2]) ||
		(data.value[3] != -1 && actor->stat.pm.value != data.value[3]))
		return (false);
	reset_board();
	calculated = false;
	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		calculate_vision_circle();
	else
		calculate_vision_line();
	size_t i = 0;
	t_actor *target_actor = NULL;
	while (i < turn_order.size())
	{
		if ((target_actor == NULL || (get_cell(turn_order[i]->coord)->v_dist < get_cell(target_actor->coord)->v_dist && turn_order[i]->stat.hp.percent() < target_actor->stat.hp.percent())) &&
			turn_order[i]->team != turn_order[turn_index % turn_order.size()]->team && get_cell(turn_order[i]->coord)->cursor == t_vect(0, 2))
			target_actor = turn_order[i];
		i++;
	}
	if (target_actor == NULL)
		return (false);
	i = 0;

	vector<t_vect>	target_list; //every tile hited by the spell
	if (actor->spell[s_spell]->zone_type == Z_DIAM)
		target_list = calc_diam(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_CROSS)
		target_list = calc_cross(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_SQUARE)
		target_list = calc_square(actor->spell[s_spell]->zone_size);


	vector<t_vect> to_look; //every tile to look

	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		to_look = AI_calc_diam(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);
	if (actor->spell[s_spell]->range_type == R_LINE)
		to_look = AI_calc_cross(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);

	t_vect target = t_vect(-1, -1); //the saved coord of the spell
	int		utils_value = 999;

	for (size_t i = 0; i < to_look.size(); i++)
	{
		int find = 0;
		int nb_target_hit = 0;
		int percent_target_hit = 100;
		if (actor->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = actor->coord - to_look[i];
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
				((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			target_list = calc_line(actor->spell[s_spell]->zone_size, dir);
		}
		for (size_t j = 0; j < target_list.size(); j++)
		{
			t_vect tmp = actor->coord + to_look[i] + target_list[j];
			if (get_cell(tmp) && get_cell(actor->coord + to_look[i]) && get_cell(actor->coord + to_look[i])->cursor == t_vect(0, 2) && get_cell(tmp)->actor != NULL)
			{
				if (get_cell(tmp)->actor == target_actor)
					find = 1;
				if (get_cell(tmp)->actor->team != actor->team)
				{
					if (percent_target_hit >= get_cell(tmp)->actor->stat.hp.percent())
						percent_target_hit = get_cell(tmp)->actor->stat.hp.percent();
					nb_target_hit++;
				}
			}
		}
		if (nb_target_hit != 0 && percent_target_hit / nb_target_hit < utils_value && find == 1)
		{
			utils_value = percent_target_hit / nb_target_hit;
			target = actor->coord + to_look[i];
		}
	}
	if (target == t_vect(-1, -1))
		return (false);
	if (cast_spell(target) == false)
		return (false);
	reset_board();
	return (true);
}

bool				s_game_board::help(t_ai_helper data)
{
	s_spell = data.value[1];
	t_actor *actor = turn_order[turn_index % turn_order.size()];
	if (actor->stat.pa.value < actor->spell[s_spell]->cost_pa ||
		actor->stat.pm.value < actor->spell[s_spell]->cost_pm ||
		(data.value[2] != -1 && actor->stat.pa.value != data.value[2]) ||
		(data.value[3] != -1 && actor->stat.pm.value != data.value[3]))
		return (false);
	reset_board();
	calculated = false;
	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		calculate_vision_circle();
	else
		calculate_vision_line();
	size_t i = 0;

	vector<t_vect>	target_list; //every tile hited by the spell
	if (actor->spell[s_spell]->zone_type == Z_DIAM)
		target_list = calc_diam(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_CROSS)
		target_list = calc_cross(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_SQUARE)
		target_list = calc_square(actor->spell[s_spell]->zone_size);


	vector<t_vect> to_look; //every tile to look

	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		to_look = AI_calc_diam(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);
	if (actor->spell[s_spell]->range_type == R_LINE)
		to_look = AI_calc_cross(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);

	t_vect target = t_vect(-1, -1); //the saved coord of the spell
	int		utils_value = 999;

	for (size_t i = 0; i < to_look.size(); i++)
	{
		int nb_target_hit = 0;
		int percent_target_hit = 100;
		if (actor->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = actor->coord - to_look[i];
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
				((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			target_list = calc_line(actor->spell[s_spell]->zone_size, dir);
		}
		for (size_t j = 0; j < target_list.size(); j++)
		{
			t_vect tmp = actor->coord + to_look[i] + target_list[j];
			if (get_cell(tmp) && get_cell(actor->coord + to_look[i]) && get_cell(actor->coord + to_look[i])->cursor == t_vect(0, 2) && get_cell(tmp)->actor != NULL)
			{
				if (get_cell(tmp)->actor->team == actor->team)
				{
					if (percent_target_hit > get_cell(tmp)->actor->stat.hp.percent())
						percent_target_hit = get_cell(tmp)->actor->stat.hp.percent();
					nb_target_hit++;
				}
			}
		}
		if (nb_target_hit != 0 && percent_target_hit / nb_target_hit < utils_value)
		{
			utils_value = percent_target_hit / nb_target_hit;
			target = actor->coord + to_look[i];
		}
	}
	if (target == t_vect(-1, -1))
		return (false);
	if (cast_spell(target) == false)
		return (false);
	reset_board();
	return (true);
}

bool				s_game_board::help_weak(t_ai_helper data)
{
	s_spell = data.value[1];
	t_actor *actor = turn_order[turn_index % turn_order.size()];
	if (actor->stat.pa.value < actor->spell[s_spell]->cost_pa ||
		actor->stat.pm.value < actor->spell[s_spell]->cost_pm ||
		(data.value[2] != -1 && actor->stat.pa.value != data.value[2]) ||
		(data.value[3] != -1 && actor->stat.pm.value != data.value[3]))
		return (false);
	reset_board();
	calculated = false;
	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		calculate_vision_circle();
	else
		calculate_vision_line();
	size_t i = 0;
	t_actor *target_actor = NULL;
	while (i < turn_order.size())
	{
		if ((target_actor == NULL || (get_cell(turn_order[i]->coord)->v_dist < get_cell(target_actor->coord)->v_dist && turn_order[i]->stat.hp.value < target_actor->stat.hp.value)) &&
			turn_order[i]->team != turn_order[turn_index % turn_order.size()]->team && get_cell(turn_order[i]->coord)->cursor == t_vect(0, 2))
			target_actor = turn_order[i];
		i++;
	}
	if (target_actor == NULL)
		return (false);
	i = 0;

	vector<t_vect>	target_list; //every tile hited by the spell
	if (actor->spell[s_spell]->zone_type == Z_DIAM)
		target_list = calc_diam(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_CROSS)
		target_list = calc_cross(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_SQUARE)
		target_list = calc_square(actor->spell[s_spell]->zone_size);


	vector<t_vect> to_look; //every tile to look

	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		to_look = AI_calc_diam(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);
	if (actor->spell[s_spell]->range_type == R_LINE)
		to_look = AI_calc_cross(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);

	t_vect target = t_vect(-1, -1); //the saved coord of the spell
	int		utils_value = 999;

	for (size_t i = 0; i < to_look.size(); i++)
	{
		int find = 0;
		int nb_target_hit = 0;
		int percent_target_hit = 100;
		if (actor->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = actor->coord - to_look[i];
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
				((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			target_list = calc_line(actor->spell[s_spell]->zone_size, dir);
		}
		for (size_t j = 0; j < target_list.size(); j++)
		{
			t_vect tmp = actor->coord + to_look[i] + target_list[j];
			if (get_cell(tmp) && get_cell(actor->coord + to_look[i]) && get_cell(actor->coord + to_look[i])->cursor == t_vect(0, 2) && get_cell(tmp)->actor != NULL)
			{
				if (get_cell(tmp)->actor == target_actor)
					find = 1;
				if (get_cell(tmp)->actor->team == actor->team)
				{
					if (percent_target_hit >= get_cell(tmp)->actor->stat.hp.percent())
						percent_target_hit = get_cell(tmp)->actor->stat.hp.percent();
					nb_target_hit++;
				}
			}
		}
		if (nb_target_hit != 0 && percent_target_hit / nb_target_hit < utils_value && find == 1)
		{
			utils_value = percent_target_hit / nb_target_hit;
			target = actor->coord + to_look[i];
		}
	}
	if (target == t_vect(-1, -1))
		return (false);
	if (cast_spell(target) == false)
		return (false);
	reset_board();
	return (true);
}

bool				s_game_board::help_percent(t_ai_helper data)
{
	s_spell = data.value[1];
	t_actor *actor = turn_order[turn_index % turn_order.size()];

	if (actor->stat.pa.value < actor->spell[s_spell]->cost_pa ||
		actor->stat.pm.value < actor->spell[s_spell]->cost_pm ||
		(data.value[3] != -1 && actor->stat.pa.value != data.value[3]) ||
		(data.value[4] != -1 && actor->stat.pm.value != data.value[4]))
		return (false);
	reset_board();
	calculated = false;
	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		calculate_vision_circle();
	else
		calculate_vision_line();
	size_t i = 0;
	t_actor *target_actor = NULL;
	while (i < turn_order.size())
	{
		t_actor *tmp = NULL;
		if (target_actor == NULL && get_cell(turn_order[i]->coord)->cursor == t_vect(0, 2))
			tmp = turn_order[i];
		else if (target_actor != NULL && get_cell(turn_order[i]->coord)->v_dist < get_cell(target_actor->coord)->v_dist && get_cell(turn_order[i]->coord)->cursor == t_vect(0, 2))
			tmp = turn_order[i];
		if (tmp != NULL)
		{
			if (tmp->stat.hp.percent() > data.value[2])
				tmp = NULL;
			if(tmp == NULL)
				break ;
			if (tmp->team != turn_order[turn_index % turn_order.size()]->team)
				tmp = NULL;
			if(tmp == NULL)
				break ;
			if (tmp != NULL)
				target_actor = tmp;
		}
		i++;
	}
	if (target_actor == NULL)
		return (false);
	i = 0;

	vector<t_vect>	target_list; //every tile hited by the spell
	if (actor->spell[s_spell]->zone_type == Z_DIAM)
		target_list = calc_diam(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_CROSS)
		target_list = calc_cross(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_SQUARE)
		target_list = calc_square(actor->spell[s_spell]->zone_size);


	vector<t_vect> to_look; //every tile to look

	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		to_look = AI_calc_diam(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);
	if (actor->spell[s_spell]->range_type == R_LINE)
		to_look = AI_calc_cross(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);

	t_vect target = t_vect(-1, -1); //the saved coord of the spell
	int		utils_value = 999;

	for (size_t i = 0; i < to_look.size(); i++)
	{
		int find = 0;
		int nb_target_hit = 0;
		int percent_target_hit = 100;
		if (actor->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = actor->coord - to_look[i];
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
				((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			target_list = calc_line(actor->spell[s_spell]->zone_size, dir);
		}
		for (size_t j = 0; j < target_list.size(); j++)
		{
			t_vect tmp = actor->coord + to_look[i] + target_list[j];
			if (get_cell(tmp) && get_cell(actor->coord + to_look[i]) && get_cell(actor->coord + to_look[i])->cursor == t_vect(0, 2) && get_cell(tmp)->actor != NULL)
			{
				if (get_cell(tmp)->actor == target_actor)
					find = 1;
				if (get_cell(tmp)->actor->team == actor->team)
				{
					if (percent_target_hit >= get_cell(tmp)->actor->stat.hp.percent())
						percent_target_hit = get_cell(tmp)->actor->stat.hp.percent();
					nb_target_hit++;
				}
			}
		}
		if (nb_target_hit != 0 && percent_target_hit / nb_target_hit < utils_value && find == 1)
		{
			utils_value = percent_target_hit / nb_target_hit;
			target = actor->coord + to_look[i];
		}
	}
	if (target == t_vect(-1, -1))
		return (false);
	if (cast_spell(target) == false)
		return (false);
	reset_board();
	return (true);
}

bool				s_game_board::help_caster_hp(t_ai_helper data)
{
	s_spell = data.value[1];
	t_actor *actor = turn_order[turn_index % turn_order.size()];

	if (actor->stat.pa.value < actor->spell[s_spell]->cost_pa ||
		actor->stat.pm.value < actor->spell[s_spell]->cost_pm ||
		(data.value[3] != -1 && actor->stat.pa.value != data.value[3]) ||
		(data.value[4] != -1 && actor->stat.pm.value != data.value[4]) ||
		actor->stat.hp.percent() > data.value[2])
		return (false);
	reset_board();
	calculated = false;
	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		calculate_vision_circle();
	else
		calculate_vision_line();
	size_t i = 0;
	t_actor *target_actor = NULL;
	while (i < turn_order.size())
	{
		t_actor *tmp = NULL;
		if (target_actor == NULL && get_cell(turn_order[i]->coord)->cursor == t_vect(0, 2))
			tmp = turn_order[i];
		else if (target_actor != NULL && get_cell(turn_order[i]->coord)->v_dist < get_cell(target_actor->coord)->v_dist && get_cell(turn_order[i]->coord)->cursor == t_vect(0, 2))
			tmp = turn_order[i];
		if (tmp != NULL)
		{
			if (target_actor != NULL && tmp->stat.hp.percent() > target_actor->stat.hp.percent())
				tmp = NULL;
			if (tmp == NULL)
				break;
			if (tmp->team != turn_order[turn_index % turn_order.size()]->team)
				tmp = NULL;
			if (tmp == NULL)
				break;
			if (tmp != NULL)
				target_actor = tmp;
		}
		i++;
	}
	if (target_actor == NULL)
		return (false);
	i = 0;

	vector<t_vect>	target_list; //every tile hited by the spell
	if (actor->spell[s_spell]->zone_type == Z_DIAM)
		target_list = calc_diam(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_CROSS)
		target_list = calc_cross(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_SQUARE)
		target_list = calc_square(actor->spell[s_spell]->zone_size);


	vector<t_vect> to_look; //every tile to look

	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		to_look = AI_calc_diam(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);
	if (actor->spell[s_spell]->range_type == R_LINE)
		to_look = AI_calc_cross(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);

	t_vect target = t_vect(-1, -1); //the saved coord of the spell
	int		utils_value = 999;

	for (size_t i = 0; i < to_look.size(); i++)
	{
		int find = 0;
		int nb_target_hit = 0;
		int percent_target_hit = 100;
		if (actor->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = actor->coord - to_look[i];
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
				((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			target_list = calc_line(actor->spell[s_spell]->zone_size, dir);
		}
		for (size_t j = 0; j < target_list.size(); j++)
		{
			t_vect tmp = actor->coord + to_look[i] + target_list[j];
			if (get_cell(tmp) && get_cell(actor->coord + to_look[i]) && get_cell(actor->coord + to_look[i])->cursor == t_vect(0, 2) && get_cell(tmp)->actor != NULL)
			{
				if (get_cell(tmp)->actor == target_actor)
					find = 1;
				if (get_cell(tmp)->actor->team == actor->team)
				{
					if (percent_target_hit >= get_cell(tmp)->actor->stat.hp.percent())
						percent_target_hit = get_cell(tmp)->actor->stat.hp.percent();
					nb_target_hit++;
				}
			}
		}
		if (nb_target_hit != 0 && percent_target_hit / nb_target_hit < utils_value && find == 1)
		{
			utils_value = percent_target_hit / nb_target_hit;
			target = actor->coord + to_look[i];
		}
	}
	if (target == t_vect(-1, -1))
		return (false);
	if (cast_spell(target) == false)
		return (false);
	reset_board();
	return (true);
}

bool				s_game_board::attack_caster_hp(t_ai_helper data)
{
	s_spell = data.value[1];
	t_actor *actor = turn_order[turn_index % turn_order.size()];
	if (actor->stat.pa.value < actor->spell[s_spell]->cost_pa ||
		actor->stat.pm.value < actor->spell[s_spell]->cost_pm ||
		(data.value[3] != -1 && actor->stat.pa.value != data.value[3]) ||
		(data.value[4] != -1 && actor->stat.pm.value != data.value[4]) ||
		actor->stat.hp.percent() > data.value[2])
		return (false);
	reset_board();
	calculated = false;
	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		calculate_vision_circle();
	else
		calculate_vision_line();
	size_t i = 0;

	vector<t_vect>	target_list; //every tile hited by the spell
	if (actor->spell[s_spell]->zone_type == Z_DIAM)
		target_list = calc_diam(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_CROSS)
		target_list = calc_cross(actor->spell[s_spell]->zone_size);
	else if (actor->spell[s_spell]->zone_type == Z_SQUARE)
		target_list = calc_square(actor->spell[s_spell]->zone_size);


	vector<t_vect> to_look; //every tile to look

	if (actor->spell[s_spell]->range_type == R_CIRCLE)
		to_look = AI_calc_diam(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);
	if (actor->spell[s_spell]->range_type == R_LINE)
		to_look = AI_calc_cross(actor->spell[s_spell]->range[0], actor->spell[s_spell]->range[1]);

	t_vect target = t_vect(-1, -1); //the saved coord of the spell
	int		utils_value = 999;

	for (size_t i = 0; i < to_look.size(); i++)
	{
		int nb_target_hit = 0;
		int percent_target_hit = 100;
		if (actor->spell[s_spell]->zone_type == Z_LINE)
		{
			t_vect diff = actor->coord - to_look[i];
			t_vect dir = t_vect(((diff).x > 0 ? -1 : (diff).x < 0 ? 1 : 0),
				((diff).y > 0 ? -1 : (diff).y < 0 ? 1 : 0));
			target_list = calc_line(actor->spell[s_spell]->zone_size, dir);
		}
		for (size_t j = 0; j < target_list.size(); j++)
		{
			t_vect tmp = actor->coord + to_look[i] + target_list[j];
			if (get_cell(tmp) && get_cell(actor->coord + to_look[i]) && get_cell(actor->coord + to_look[i])->cursor == t_vect(0, 2) && get_cell(tmp)->actor != NULL)
			{
				if (get_cell(tmp)->actor->team != actor->team)
				{
					if (percent_target_hit >= get_cell(tmp)->actor->stat.hp.percent())
						percent_target_hit = get_cell(tmp)->actor->stat.hp.percent();
					nb_target_hit++;
				}
			}
		}
		if (nb_target_hit != 0 && percent_target_hit / nb_target_hit < utils_value)
		{
			utils_value = percent_target_hit / nb_target_hit;
			target = actor->coord + to_look[i];
		}
	}
	if (target == t_vect(-1, -1))
		return (false);
	if (cast_spell(target) == false)
		return (false);
	reset_board();
	return (true);
}

bool				s_game_board::action_on_turn(t_ai_helper data)
{
	(void)data;
	return (false);

}
/*
#define CHARGE			0	//run to the closest enemy at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define CHARGE_WEAK		1	//run to the enemy with less hp at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define CHARGE_PERCENT	2	//run to the enemy with less % hp at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define RETREAT			3	//retreat to the safest tile if stat hp < value[1] %
#define SUPPORT			4	//run to closest ally at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define SUPPORT_PERCENT	5	//run to closest ally with less % hp at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define ATTACK			6	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on the closest enemy availible in range if possible
#define ATTACK_WEAK		7	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on the enemy with the less hp in range if possible
#define ATTACK_PERCENT	8	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on the enemy with the less % hp in range if possible
#define HELP			9	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on an ally
#define HELP_WEAK		10	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on the ally with less % HP
#define HELP_PERC		11	//cast the spell num value[2] (if value[2] PA and value[3] pm on caster) on the ally if HP % < value[1]
#define TURN			12	//if turn == value[1] --> execute command num value[2] with verification helped by value[3]
*/

gambit_command command[] = {
	&s_game_board::get_close_enemy, // 0
	&s_game_board::get_close_enemy_weak, //1
	&s_game_board::get_close_enemy_percent, // 2
	&s_game_board::flee_enemy, //3
	&s_game_board::get_close_ally, //4
	&s_game_board::get_close_ally_percent, // 5
	&s_game_board::get_close_ally_damaged, // 6
	&s_game_board::attack, // 7
	&s_game_board::attack_weak, // 8
	&s_game_board::attack_percent, // 9
	&s_game_board::attack_caster_hp, //10
	&s_game_board::help, // 11
	&s_game_board::help_weak, // 12
	&s_game_board::help_percent, // 13
	&s_game_board::help_caster_hp, //14
	&s_game_board::action_on_turn, // 15
};


bool					s_game_board::execute_gambit(t_actor *source)
{
	if (check_anim() == false || check_visual() == false ||
		source->destination.size() != 0 ||
		enemy_list.size() == 0 || ally_list.size() == 0)
		return (false);
	size_t i = 0;
	while (i < source->gambit.size())
	{
		int index = source->gambit[i].value[0];
		t_ai_helper data = source->gambit[i];
		if (index >= 0)
		{
			if ((this->*(command[index]))(data) == true)
				return (false);
		}
		i++;
	}
	return (true);
}

void				s_game_board::enemy_turn()
{
	reset_board();

	bool play = true;
	SDL_Event	event;

	while (execute_gambit(turn_order[turn_index % turn_order.size()]) == false && play == true)
	{
		prepare_screen();

		update_board();
		draw_board();
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
	if (turn_order.size())
	{
		turn_order[turn_index % turn_order.size()]->selected = false;
		turn_order[turn_index % turn_order.size()]->reset_value();
	}
	turn_index = (turn_index + 1) % turn_order.size();
	turn_order[turn_index % turn_order.size()]->handle_effect_duration();
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

void				s_game_board::draw_path()
{
	vector<t_vect>	path = calc_path(get_mouse_pos());
	t_actor *player = turn_order[turn_index % turn_order.size()];

	size_t i = 0;
	while (i < path.size())
	{
		if (get_cell(path[i]) && get_cell(path[i])->m_dist <= player->stat.pm.value)
			draw_centred_rectangle((path[i] + target) * sprite_unit * zoom + offset + sprite_unit * zoom / 2, sprite_unit * zoom / 2, t_color(0.5, 0.5, 1.0));
		else
			draw_centred_rectangle((path[i] + target) * sprite_unit * zoom + offset + sprite_unit * zoom / 2, sprite_unit * zoom / 2, t_color(1.0, 0.5, 0.5));
		i++;
	}
}

void				s_game_board::draw_board()
{
	draw_self();
	draw_cell_layer();
	draw_placement();
	if (turn_order.size())
		draw_path();
	draw_actor_list();
	draw_cursor_layer();
	draw_mouse_cursor();
	//draw_cell_border();
	draw_animation();
	draw_visual_info();
}

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
				interface_map["simple_cursor"].draw_self(gui.unit * t_vect((i < 3 ? 8.5 : 18.5) + ((i % 3) * 1.5), 18.1), gui.unit * 0.8, t_vect(0, 3));
				draw_centred_text(to_string(player->spell[i]->cost_pa), 25, gui.unit * t_vect((i < 3 ? 8.5 : 18.5) + ((i % 3) * 1.5), 18.1) + gui.unit * 0.4, BLACK);
			}
			if (player->spell[i]->cost_pm > 0)
			{
				interface_map["simple_cursor"].draw_self(gui.unit * t_vect((i < 3 ? 7.7 : 17.7) + ((i % 3) * 1.5), 18.1), gui.unit * 0.8, t_vect(1, 3));
				draw_centred_text(to_string(player->spell[i]->cost_pm), 25, gui.unit * t_vect((i < 3 ? 7.7 : 17.7) + ((i % 3) * 1.5), 18.1) + gui.unit * 0.4, BLACK);
			}
		}
	}
}

static string parse_effect_poison_desc(s_effect *effect)
{
	if (effect->effect_type == 0)
		return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " damage(s) when turn begin (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
	else if (effect->effect_type == 1)
		return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " damage(s) when attack (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
	else if (effect->effect_type == 2)
		return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " damage(s) when move (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
	return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " damage(s) never (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
}

static string parse_effect_regeneration_desc(s_effect *effect)
{
	if (effect->effect_type == 0)
		return ("Effect : +" + to_string(effect->action[0].stat.value[0]) + " hp(s) when turn begin (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
	else if (effect->effect_type == 1)
		return ("Effect : +" + to_string(effect->action[0].stat.value[0]) + " hp(s) when attack (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
	else if (effect->effect_type == 2)
		return ("Effect : +" + to_string(effect->action[0].stat.value[0]) + " hp(s) when move (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
	return ("Effect : +" + to_string(effect->action[0].stat.value[0]) + " hp(s) never (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
}

static string parse_effect_change_pm_desc(s_effect *effect)
{
	if (effect->action[0].stat.value[0] < 0)
	{
		if (effect->effect_type == 0)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PM(s) when turn begin (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		else if (effect->effect_type == 1)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PM(s) when attack (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		else if (effect->effect_type == 2)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PM(s) when move (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PM(s) never (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
	}
	else
	{
		if (effect->effect_type == 0)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PM(s) when turn begin (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		else if (effect->effect_type == 1)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PM(s) when attack (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		else if (effect->effect_type == 2)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PM(s) when move (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PM(s) never (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
	}
}

static string parse_effect_change_pa_desc(s_effect *effect)
{
	if (effect->action[0].stat.value[0] < 0)
	{
		if (effect->effect_type == 0)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PA(s) when turn begin (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		else if (effect->effect_type == 1)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PA(s) when attack (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		else if (effect->effect_type == 2)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PA(s) when move (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PA(s) never (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
	}
	else
	{
		if (effect->effect_type == 0)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PA(s) when turn begin (" + to_string(effect->action[0].stat.value[3]) + " turn(s) left)");
		else if (effect->effect_type == 1)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PA(s) when attack");
		else if (effect->effect_type == 2)
			return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PA(s) when move");
		return ("Effect : " + to_string(effect->action[0].stat.value[0]) + " PA(s) never");
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
			draw_lined_text(text, text_size, gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
			text = "Action points : " + to_string(player->stat.pa.value);
			draw_lined_text(text, text_size, gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
			text = "Mouvement points : " + to_string(player->stat.pm.value);
			draw_lined_text(text, text_size, gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
			for (size_t j = 0; j < player->effect_list.poison.size(); j++)
				draw_lined_text(parse_effect_poison_desc(&(player->effect_list.poison[j])), text_size, gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
			for (size_t j = 0; j < player->effect_list.regeneration.size(); j++)
				draw_lined_text(parse_effect_regeneration_desc(&(player->effect_list.regeneration[j])), text_size, gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
			for (size_t j = 0; j < player->effect_list.change_pa.size(); j++)
				draw_lined_text(parse_effect_change_pm_desc(&(player->effect_list.change_pa[j])), text_size, gui.unit * t_vect(22.2, 0.5 * i++ + 0.5), BLACK);
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

void				s_game_board::draw_select_wheel(int *index)
{
	if (actor_pool.size() == 0)
		return;
	t_vect coord = t_vect(1, 2);
	t_vect size[3];

	size[0] = t_vect(1.2, 1.2);
	size[1] = t_vect(1.8, 1.8);
	size[2] = t_vect(2.4, 2.4);

	draw_border_rectangle(gui.unit * coord, gui.unit * size[0], 12, t_color(0.5, 0.5, 0.5), t_color(0.4, 0.4, 0.4));
	actor_pool[(*index - 2) % actor_pool.size()]->tile->draw_self(gui.unit * coord + 12, gui.unit * size[0] - 24, actor_pool[(*index - 2) % actor_pool.size()]->sprite);

	coord = coord + t_vect(0, size[0].y + 0.5);
	draw_border_rectangle(gui.unit * coord, gui.unit * size[1], 12, t_color(0.5, 0.5, 0.5), t_color(0.4, 0.4, 0.4));
	actor_pool[(*index - 1) % actor_pool.size()]->tile->draw_self(gui.unit * coord + 12, gui.unit * size[1] - 24, actor_pool[(*index - 1) % actor_pool.size()]->sprite);

	coord = coord + t_vect(0, size[1].y + 0.5);
	draw_border_rectangle(gui.unit * coord, gui.unit * size[2], 12, t_color(0.5, 0.5, 0.5), t_color(0.4, 0.4, 0.4));
	actor_pool[(*index) % actor_pool.size()]->tile->draw_self(gui.unit * coord + 12, gui.unit * size[2] - 24, actor_pool[(*index) % actor_pool.size()]->sprite);

	coord = coord + t_vect(0, size[2].y + 0.5);
	draw_border_rectangle(gui.unit * coord, gui.unit * size[1], 12, t_color(0.5, 0.5, 0.5), t_color(0.4, 0.4, 0.4));
	actor_pool[(*index + 1) % actor_pool.size()]->tile->draw_self(gui.unit * coord + 12, gui.unit * size[1] - 24, actor_pool[(*index + 1) % actor_pool.size()]->sprite);

	coord = coord + t_vect(0, size[1].y + 0.5);
	draw_border_rectangle(gui.unit * coord, gui.unit * size[0], 12, t_color(0.5, 0.5, 0.5), t_color(0.4, 0.4, 0.4));
	actor_pool[(*index + 2) % actor_pool.size()]->tile->draw_self(gui.unit * coord + 12, gui.unit * size[0] - 24, actor_pool[(*index + 2) % actor_pool.size()]->sprite);

	coord = coord + t_vect(0, size[0].y + 0.5);
	size[0] = t_vect(4.5, 6);
	draw_border_rectangle(gui.unit * coord, gui.unit * size[0], 12, t_color(0.5, 0.5, 0.5), t_color(0.4, 0.4, 0.4));
	int i = 0;
	t_actor *player = actor_pool[(*index) % actor_pool.size()];
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
		text = "Spell [" + to_string(i - 4) + "] : " + player->spell[i - 4]->name;
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


void				s_game_board::m_calc_cell(vector<t_vect> *to_calc, int i, int x, int j, int y)
{
	if (get_cell(i + x, j + y) != NULL && get_cell(i + x, j + y)->node->m_obs == false &&
		get_cell(i + x, j + y)->m_dist > get_cell(i, j)->m_dist + (get_cell(i + x, j + y)->actor == NULL ? get_cell(i + x, j + y)->node->cost : 999))
	{
		get_cell(i + x, j + y)->m_dist = get_cell(i, j)->m_dist + (get_cell(i + x, j + y)->actor == NULL ? get_cell(i + x, j + y)->node->cost : 999);
		to_calc->push_back(t_vect(i + x, j + y));
	}
}

void				s_game_board::calculate_distance(t_vect start)
{
	reset_board();
	vector<t_vect>	to_calc;
	size_t i;

	to_calc.push_back(start);

	i = 0;
	get_cell(to_calc[i].x, to_calc[i].y)->m_dist = 0;
	while (i < to_calc.size())
	{
		m_calc_cell(&to_calc, to_calc[i].x, 1, to_calc[i].y, 0);
		m_calc_cell(&to_calc, to_calc[i].x, -1, to_calc[i].y, 0);
		m_calc_cell(&to_calc, to_calc[i].x, 0, to_calc[i].y, 1);
		m_calc_cell(&to_calc, to_calc[i].x, 0, to_calc[i].y, -1);
		i++;
	}
	i = 1;
	t_actor *player = turn_order[turn_index % turn_order.size()];
	while (i < to_calc.size() && player->team == 1)
	{
		if (get_cell(to_calc[i].x, to_calc[i].y)->m_dist <= turn_order[turn_index % turn_order.size()]->stat.pm.value)
			get_cell(to_calc[i].x, to_calc[i].y)->cursor = t_vect(1, 2);
		i++;
	}
	calculated = true;
}

vector<t_vect>		s_game_board::pathfinding(t_actor *player, t_vect dest, double speed)
{
	vector<t_vect>	path;
	t_vect			actual = dest;
	t_vect			to_look = actual;
	t_vect			source = player->coord;

	if (get_cell(dest) && get_cell(dest)->actor != NULL &&
		((abs(source.x - dest.x) == 1 && source.y - dest.y == 0) ||
		(abs(source.y - dest.y) == 1 && source.x - dest.x == 0)))
		actual = source;
	while (actual != source)
	{
		actual = to_look;
		if (get_cell((int)(actual.x) + 1, (int)(actual.y)) && get_cell((int)(actual.x) + 1, (int)(actual.y))->m_dist < get_cell(to_look.x, to_look.y)->m_dist)
			to_look = t_vect((int)(actual.x) + 1, (int)(actual.y));
		if (get_cell((int)(actual.x) - 1, (int)(actual.y)) && get_cell((int)(actual.x) - 1, (int)(actual.y))->m_dist < get_cell(to_look.x, to_look.y)->m_dist)
			to_look = t_vect((int)(actual.x) - 1, (int)(actual.y));
		if (get_cell((int)(actual.x), (int)(actual.y) + 1) && get_cell((int)(actual.x), (int)(actual.y) + 1)->m_dist < get_cell(to_look.x, to_look.y)->m_dist)
			to_look = t_vect((int)(actual.x), (int)(actual.y) + 1);
		if (get_cell((int)(actual.x), (int)(actual.y) - 1) && get_cell((int)(actual.x), (int)(actual.y) - 1)->m_dist < get_cell(to_look.x, to_look.y)->m_dist)
			to_look = t_vect((int)(actual.x), (int)(actual.y) - 1);
		if (actual == to_look)
			break;
		if ((get_cell(actual)->m_dist <= player->stat.pm.value && actual != source) || speed != 1)
		{
			int i = 0;
			t_vect delta = ((to_look - actual) / ((15.0 * speed) * (speed != 1 ? get_cell(actual)->node->cost : 1)));
			while (i < (15.0 * speed) * (speed != 1 ? get_cell(actual)->node->cost : 1))
			{
				path.insert(path.begin(), actual + delta * i);
				i++;
			}
		}
	}
	calculated = false;
	return (path);
}

vector<t_vect>		s_game_board::calc_path(t_vect dest)
{
	vector<t_vect>	path;
	t_vect			actual = dest;
	t_vect			to_look = actual;
	t_vect			source = turn_order[turn_index % turn_order.size()]->coord;

	if (get_cell(dest.x, dest.y) == NULL || (get_cell(dest.x, dest.y)->m_dist == 999 && get_cell(dest.x, dest.y)->actor == NULL) )
		return (path);
	while (actual != source)
	{
		actual = to_look;
		if (get_cell((int)(actual.x) + 1, (int)(actual.y)) && get_cell((int)(actual.x) + 1, (int)(actual.y))->m_dist < get_cell(to_look)->m_dist)
			to_look = t_vect((int)(actual.x) + 1, (int)(actual.y));
		if (get_cell((int)(actual.x) - 1, (int)(actual.y)) && get_cell((int)(actual.x) - 1, (int)(actual.y))->m_dist < get_cell(to_look)->m_dist)
			to_look = t_vect((int)(actual.x) - 1, (int)(actual.y));
		if (get_cell((int)(actual.x), (int)(actual.y) + 1) && get_cell((int)(actual.x), (int)(actual.y) + 1)->m_dist < get_cell(to_look)->m_dist)
			to_look = t_vect((int)(actual.x), (int)(actual.y) + 1);
		if (get_cell((int)(actual.x), (int)(actual.y) - 1) && get_cell((int)(actual.x), (int)(actual.y) - 1)->m_dist < get_cell(to_look)->m_dist)
			to_look = t_vect((int)(actual.x), (int)(actual.y) - 1);
		if (to_look == actual)
			break;
		if (to_look != source)
			path.insert(path.begin(), to_look);
	}
	if (path.size() != 0 || (path.size() == 0 && get_cell(dest)->actor != NULL))
		path.insert(path.begin(), dest);
	if (path.size() == 2 && path[0] == path[1])
		path.clear();
	return (path);
}#include "taag.h"

void			s_game_board::handle_actor_placement(SDL_Event *event, int *index)
{
	t_vect 			mouse = get_mouse_pos();

	if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)
	{
		if (actor_pool.size())
		{
			size_t count = 0;
			while (count < placement_list.size() && mouse != placement_list[count])
				count++;
			if (count < placement_list.size())
			{
				invoke_actor(new t_actor(*(actor_pool[*index % actor_pool.size()])), get_mouse_pos());
			}
		}
	}
	else if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_RIGHT)
	{

		if (get_cell(mouse) && get_cell(mouse)->actor != NULL)
		{
			size_t count = 0;
			while (count < placement_list.size() && mouse != placement_list[count])
				count++;
			if (count < placement_list.size())
				outvoke_actor(get_cell(mouse)->actor);
		}
	}
	else if (event->type == SDL_KEYDOWN)
	{
		if (event->key.keysym.sym == SDLK_UP)
			(*index)--;
		else if (event->key.keysym.sym == SDLK_DOWN)
			(*index)++;
	}
}

void			s_game_board::placement_phase()
{
	bool		continu = true;
	SDL_Event	event;
	bool		play = true;
	int			index = 0;

	vector<string>	actor_file = list_files(ACTOR_PATH, ACTOR_EXT);
	for (size_t i = 0; i < actor_file.size(); i++)
	{
		t_actor *new_actor = new t_actor(read_actor(ACTOR_PATH + actor_file[i] + ACTOR_EXT));
		new_actor->team = 1;
		actor_pool.push_back(new_actor);
	}

	while (play)
	{
		prepare_screen();

		draw_board();
		draw_cell_info_on_gui();
		draw_select_wheel(&index);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(3, &gui, &play, &continu));
			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE && actor_list.size() != enemy_list.size())
				play = false;
			handle_control_camera(&event);
			handle_actor_placement(&event, &index);
		}
		render_screen(true);
	}
	placement_list.clear();
	enemy_placement_list.clear();

	if (continu == true)
		game_loop();
}
#include "taag.h"

void				s_game_board::handle_control_game(SDL_Event *event)
{
	t_actor *player = turn_order[turn_index % turn_order.size()];
	if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_SPACE)
		next_turn();
	else if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)
	{
		if (gui.click(event) == false)
		{
			if (s_spell == -1)
				move_actor(player, get_mouse_pos(), 1.0);
			else
				cast_spell(get_mouse_pos());
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
		if (event->key.keysym.sym == SDLK_q)
		{
			if (player->stat.pa.value >= player->spell[0]->cost_pa && player->stat.pm.value >= player->spell[0]->cost_pm &&
				player->cooldown[0] == 0)
				s_spell = 0;
			else
				s_spell = -1;
		}
		else if (event->key.keysym.sym == SDLK_w)
		{
			if (player->stat.pa.value >= player->spell[1]->cost_pa && player->stat.pm.value >= player->spell[1]->cost_pm &&
				player->cooldown[1] == 0)
				s_spell = 1;
			else
				s_spell = -1;
		}
		else if (event->key.keysym.sym == SDLK_e)
		{
			if (player->stat.pa.value >= player->spell[2]->cost_pa && player->stat.pm.value >= player->spell[2]->cost_pm &&
				player->cooldown[2] == 0)
				s_spell = 2;
			else
				s_spell = -1;
		}
		else if (event->key.keysym.sym == SDLK_r)
		{
			if (player->stat.pa.value >= player->spell[3]->cost_pa && player->stat.pm.value >= player->spell[3]->cost_pm &&
				player->cooldown[3] == 0)
				s_spell = 3;
			else
				s_spell = -1;
		}
		else if (event->key.keysym.sym == SDLK_t)
		{
			if (player->stat.pa.value >= player->spell[4]->cost_pa && player->stat.pm.value >= player->spell[4]->cost_pm &&
				player->cooldown[4] == 0)
				s_spell = 4;
			else
				s_spell = -1;
		}
		else if (event->key.keysym.sym == SDLK_y)
		{
			if (player->stat.pa.value >= player->spell[5]->cost_pa && player->stat.pm.value >= player->spell[0]->cost_pm &&
				player->cooldown[5] == 0)
				s_spell = 5;
			else
				s_spell = -1;
		}
		calculated = false;
	}
}

void			s_game_board::ending_fight(bool *play)
{
	if (check_anim() == true && check_visual() == true)
		*play = false;
}

void			s_game_board::game_loop()
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

		if (enemy_list.size() == 0 || ally_list.size() == 0)
			ending_fight(&play);

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
#include "taag.h"

map<t_vect, map<t_vect, vector<t_vect>[4]>>	vision_map;

bool				s_game_board::v_calc_cell(t_vect source, t_vect target, int prev_dist)
{
	int				scale = 5;

	if (vision_map.find(source) == vision_map.end() || vision_map[source].find(target) == vision_map[source].end())
	{
		vision_map[source][target][0] = calc_line_2d(t_vect(source.x + 0.49, source.y + 0.49) * scale, t_vect(target.x + 0.49, target.y + 0.49) * scale);
		vision_map[source][target][1] = calc_line_2d(t_vect(source.x + 0.51, source.y + 0.49) * scale, t_vect(target.x + 0.51, target.y + 0.49) * scale);
		vision_map[source][target][2] = calc_line_2d(t_vect(source.x + 0.49, source.y + 0.51) * scale, t_vect(target.x + 0.49, target.y + 0.51) * scale);
		vision_map[source][target][3] = calc_line_2d(t_vect(source.x + 0.51, source.y + 0.51) * scale, t_vect(target.x + 0.51, target.y + 0.51) * scale);
	}
	vector<t_vect>	vision_tl = vision_map[source][target][0];
	vector<t_vect>	vision_tr = vision_map[source][target][1];
	vector<t_vect>	vision_dl = vision_map[source][target][2];
	vector<t_vect>	vision_dr = vision_map[source][target][3];

	if (get_cell(target) && (get_cell(target)->node->v_obs == false || turn_order[turn_index % turn_order.size()]->spell[s_spell]->block == INT_TRUE) && get_cell(target)->v_dist >= prev_dist)
	{
		size_t i = 0;
		bool visible = true;
		while (turn_order[turn_index % turn_order.size()]->spell[s_spell]->block == INT_FALSE && i < vision_tl.size() && visible == true && get_cell(vision_dr[i] / scale))
		{
			if (get_cell(vision_tl[i] / scale)->node->v_obs == true ||
				(get_cell(vision_tl[i] / scale)->actor != NULL &&
					get_cell(vision_tl[i] / scale)->actor != turn_order[turn_index % turn_order.size()] &&
					get_cell(vision_tl[i] / scale)->actor != get_cell(target)->actor))
				if (get_cell(vision_tr[i] / scale)->node->v_obs == true ||
					(get_cell(vision_tr[i] / scale)->actor != NULL &&
						get_cell(vision_tr[i] / scale)->actor != turn_order[turn_index % turn_order.size()] &&
						get_cell(vision_tr[i] / scale)->actor != get_cell(target)->actor))
					if (get_cell(vision_dl[i] / scale)->node->v_obs == true ||
						(get_cell(vision_dl[i] / scale)->actor != NULL &&
							get_cell(vision_dl[i] / scale)->actor != turn_order[turn_index % turn_order.size()] &&
							get_cell(vision_dl[i] / scale)->actor != get_cell(target)->actor))
						if (get_cell(vision_dr[i] / scale)->node->v_obs == true ||
							(get_cell(vision_dr[i] / scale)->actor != NULL &&
								get_cell(vision_dr[i] / scale)->actor != turn_order[turn_index % turn_order.size()] &&
								get_cell(vision_dr[i] / scale)->actor != get_cell(target)->actor))
							visible = false;
			i++;
		}
		if (visible == true)
		{
			get_cell(target)->v_dist = prev_dist + 1;
			if (get_cell(target)->v_dist <= turn_order[turn_index % turn_order.size()]->spell[s_spell]->range[1])
			{
				if (get_cell(target)->v_dist >= turn_order[turn_index % turn_order.size()]->spell[s_spell]->range[0] &&
					get_cell(target)->node->v_obs == false && get_cell(target)->node->tile != NULL)
					get_cell(target)->cursor = t_vect(0, 2);
				return (true);
			}
		}
	}
	return (false);
}

void				s_game_board::calculate_vision_circle()
{
	reset_board();
	t_actor 		*player = turn_order[turn_index % turn_order.size()];
	vector<t_vect>	to_calc;
	size_t i;

	to_calc.push_back(player->coord);
	get_cell(to_calc[0])->v_dist = 0;
	if (player->spell[s_spell]->range[0] <= 0)
		get_cell(player->coord)->cursor = t_vect(0, 2);
	i = 0;
	while (i < to_calc.size())
	{
		if(v_calc_cell(player->coord, t_vect(to_calc[i].x + 1, to_calc[i].y), get_cell(to_calc[i])->v_dist) == true)
			to_calc.push_back(t_vect(to_calc[i].x + 1, to_calc[i].y));
		if(v_calc_cell(player->coord, t_vect(to_calc[i].x - 1, to_calc[i].y), get_cell(to_calc[i])->v_dist) == true)
			to_calc.push_back(t_vect(to_calc[i].x - 1, to_calc[i].y));
		if(v_calc_cell(player->coord, t_vect(to_calc[i].x, to_calc[i].y + 1), get_cell(to_calc[i])->v_dist) == true)
			to_calc.push_back(t_vect(to_calc[i].x, to_calc[i].y + 1));
		if(v_calc_cell(player->coord, t_vect(to_calc[i].x, to_calc[i].y - 1), get_cell(to_calc[i])->v_dist) == true)
			to_calc.push_back(t_vect(to_calc[i].x, to_calc[i].y - 1));
		i++;
	}
	calculated = true;
}

void				s_game_board::calculate_vision_line()
{
	reset_board();
	t_actor 		*player = turn_order[turn_index % turn_order.size()];
	int i;

	get_cell(player->coord)->v_dist = 0;
	if (player->spell[s_spell]->range[0] <= 0)
		get_cell(player->coord)->cursor = t_vect(0, 2);
	i = 0;
	while (i <= player->spell[s_spell]->range[1])
	{
		v_calc_cell(player->coord, t_vect(player->coord.x + i, player->coord.y), i - 1);
		v_calc_cell(player->coord, t_vect(player->coord.x - i, player->coord.y), i - 1);
		v_calc_cell(player->coord, t_vect(player->coord.x, player->coord.y + i), i - 1);
		v_calc_cell(player->coord, t_vect(player->coord.x, player->coord.y - i), i - 1);
		i++;
	}
	calculated = true;
}
