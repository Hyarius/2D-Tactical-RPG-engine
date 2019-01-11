#include "taag.h"

#define ACTOR_PATH "ressources/game_object/actor/"
#define ACTOR_EXT ".act"
#define NODE_PATH "ressources/game_object/node/"
#define NODE_EXT ".node"

s_game_board::s_game_board() {}

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
		if (tab.size() == 0)
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
			cell_layer[(size_t)(coord.x)][(size_t)(coord.y)].actor = new t_actor(read_actor(ACTOR_PATH + tab[3] + ACTOR_EXT));
			cell_layer[(size_t)(coord.x)][(size_t)(coord.y)].actor->coord = coord;
			cell_layer[(size_t)(coord.x)][(size_t)(coord.y)].actor->team = atoi(tab[4].c_str());
			actor_list.push_back(cell_layer[(size_t)(coord.x)][(size_t)(coord.y)].actor);
		}
	}
	myfile.close();

	zoom = 3.0;
	sprite_unit = t_vect(32, 32);
	offset = get_win_size() / 2;
	target = t_vect(-board_size.x / 2, -board_size.y / 2);
	cursor_tile = &tileset_map["simple_cursor"];
}

t_cell				*s_game_board::get_cell(int x, int y)
{
	if (x < 0 || x >= board_size.x || y < 0 || y >= board_size.y)
		return (NULL);
	if (cell_layer[x][y].node == NULL)
		return (NULL);
	return (&(cell_layer[x][y]));
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
	cursor_tile->draw_self((coord + target) * size + offset, size, sprite);
}

void				s_game_board::draw_self()
{
	draw_cell_layer();
	draw_cursor_layer();
	draw_actor_list();
	draw_mouse_cursor();
}

void				s_game_board::draw_mouse_cursor()
{
	t_vect mouse = get_mouse_pos();

	t_vect size = sprite_unit * zoom;
	if (mouse != t_vect(-1, -1))
		draw_cursor(mouse, target, size, offset, t_vect(1, 0));
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
	if (event->type == SDL_MOUSEMOTION && event->button.button == SDL_BUTTON_LEFT)
	{
		target.x += (double)(event->motion.xrel) / (sprite_unit.x * zoom);
		target.y += (double)(event->motion.yrel) / (sprite_unit.y * zoom);
	}
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