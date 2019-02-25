#include "taag.h"

static void				change_s_spell(t_data data)
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
		enemy_turn();
	else
	{
		reset_board();
		if (turn_order.size())
			calculate_distance(turn_order[turn_index % turn_order.size()]->coord);
	}
}
