#include "taag.h"

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

void				s_game_board::draw_cursor(t_vect coord, t_vect target, t_vect size, t_vect offset, t_vect sprite)
{
	cursor_tile->prepare_print((coord + target) * size + offset, size, sprite);
}

void				s_game_board::draw_board()
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

void				s_game_board::draw_mouse_cursor()
{
	t_vect mouse = get_mouse_pos();

	t_vect size = sprite_unit * zoom;
	if (mouse != t_vect(-1, -1))
		draw_cursor(mouse, target, size, offset, t_vect(1, 0));
	render_triangle_texture(cursor_tile->texture_id);

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
	render_triangle_texture(get_interface_tile("simple_cursor")->texture_id);
}

void				s_game_board::draw_placement()
{
	t_vect size = sprite_unit * zoom;

	for (size_t i = 0; i < placement_list.size(); i++)
		cursor_tile->draw_self((placement_list[i] + target) * size + offset, size, t_vect(3, 0));
	for (size_t i = 0; i < enemy_placement_list.size(); i++)
		cursor_tile->draw_self((enemy_placement_list[i] + target) * size + offset, size, t_vect(4, 0));
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
