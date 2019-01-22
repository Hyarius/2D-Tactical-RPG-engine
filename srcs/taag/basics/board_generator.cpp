#include "taag.h"

static int			generate_type()
{
	int type = generate_nbr(0, 100);

	if (type <= 91)
		return (0);
	if (type <= 94)
		return (4);
	if (type <= 97)
		return (8);
	return (7);
}

t_game_board		board_generator(int size_x, int size_y)
{
	t_game_board	board;
	int				x;
	int				y;

	board.target = t_vect(-size_x, -size_y) / 2;
	board.board_size = t_vect(size_x, size_y);
	x = 0;
	board.cell_layer.resize(size_x);
	while (x < size_x)
	{
		board.cell_layer[x].resize(size_y);
		x++;
	}
	x = 0;
	while (x < size_x)
	{
		y = 0;
		while (y < size_y)
		{
			int type = generate_type();
			board.cell_layer[x][y] = t_cell(t_vect(x, y), &(board.node_list[type]));
			y++;
		}
		x++;
	}
	int nb_enemy= generate_nbr(1, 5);
	for (int i = 0; i < nb_enemy; i++)
	{
		t_spell *p_spell[6];
		for (int i = 0; i < 6; i++)
			p_spell[i] = &(spell_map["NULL"]);
		t_actor *enemy = new t_actor("enemy_test", &(tileset_map["simple_actor"]), t_vect(0, 0), t_stat(t_value(15), t_value(6), t_value(3), 3), p_spell);
		enemy->team = 2;
		x = generate_nbr(0, size_x);
		y = generate_nbr(0, size_y);
		while (board.get_cell(x, y)->actor != NULL || board.get_cell(x, y)->node->m_obs == true)
		{
			x = generate_nbr(0, size_x);
			y = generate_nbr(0, size_y);
		}
		enemy->coord = t_vect(x, y);
		board.cell_layer[x][y].actor = enemy;
		board.actor_list.push_back(board.cell_layer[x][y].actor);
		board.enemy_list.push_back(board.cell_layer[x][y].actor);
	}
	int i = 0;
	while (i < 8)
	{
		x = generate_nbr(0, size_x);
		y = generate_nbr(0, size_y);
		if (board.get_cell(x, y)->actor == NULL)
		{
			size_t count = 0;
			while (count < board.placement_list.size())
			{
				if (board.placement_list[count] == t_vect(x, y))
				{
					x = generate_nbr(0, size_x);
					y = generate_nbr(0, size_y);
					count = 0;
				}
				else
					count++;
			}
			board.placement_list.push_back(t_vect(x, y));
		}
		i++;
	}
	return (board);
}
