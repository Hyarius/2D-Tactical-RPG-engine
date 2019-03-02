#include "taag.h"

vector<t_cell *>		cell_list;
t_gui					gui_part[4];

static void				select_node(t_data data)
{
	t_node *node;
	t_game_board *board = (t_game_board *)(data.data[0]);
	int i = (int &)(data.data[1]);
	if (i != -1)
		node = &(board->node_list[i]);
	else
		node = &empty_node;
	size_t j = 0;
	while (j < cell_list.size())
	{
		cell_list[j]->node = node;
		cell_list[j]->cursor = t_vect(0, 0);
		j++;
	}
	cell_list.clear();
}

static void				select_placement(t_data data)
{
	t_game_board *board = (t_game_board *)(data.data[0]);
	int i = (int &)(data.data[1]);
	size_t j = 0;
	while (j < cell_list.size())
	{
		size_t h = 0;
		while (h < board->placement_list.size())
		{
			if (board->placement_list[h] == cell_list[j]->coord)
				board->placement_list.erase(board->placement_list.begin() + h);
			h++;
		}
		h = 0;
		while (h < board->enemy_placement_list.size())
		{
			if (board->enemy_placement_list[h] == cell_list[j]->coord)
				board->enemy_placement_list.erase(board->enemy_placement_list.begin() + h);
			h++;
		}
		if (i == 1)
			board->placement_list.push_back(cell_list[j]->coord);
		else if (i == 2)
			board->enemy_placement_list.push_back(cell_list[j]->coord);
		cell_list[j]->cursor = t_vect(0, 0);
		j++;
	}
	cell_list.clear();
}

bool moved = false;

static void				control_mouse_editor(t_game_board *board)
{
	t_vect mouse = board->get_mouse_pos();
	if (mouse != t_vect(-1, -1))
	{
		if (moved == true && board->get_cell(mouse)->cursor == t_vect(0, 0))
		{
			board->get_cell(mouse)->cursor = t_vect(2, 0);
			cell_list.push_back(board->get_cell(mouse));
		}
		if (moved == false && board->get_cell(mouse)->cursor == t_vect(0, 0))
		{
			board->get_cell(mouse)->cursor = t_vect(2, 0);
			cell_list.push_back(board->get_cell(mouse));
		}
		else if (moved == false && board->get_cell(mouse)->cursor == t_vect(2, 0))
		{
			board->get_cell(mouse)->cursor = t_vect(0, 0);
			size_t h = 0;
			while (h < cell_list.size())
			{
				if (cell_list[h]->coord == mouse)
					cell_list.erase(cell_list.begin() + h);
				h++;
			}
		}
	}
}

static void				reset_selection()
{
	size_t i = 0;
	while (i < cell_list.size())
	{
		cell_list[i]->cursor = t_vect(0, 0);
		i++;
	}
	cell_list.clear();
}

vector<string> text_type = {
	"nothing",
	"node",
	"placement",
	"information",
};

static void		increment_iterator_text(t_data data) //0 - &value / 1 - &pool / 2 - increment / 3 - cost / 4 - min / 5 - max
{
	int *value = (int *)(data.data[0]);
	int	delta = (int &)(data.data[1]);
	int	min = (int &)(data.data[2]);
	int	max = (int &)(data.data[3]);
	string *text = (string *)(data.data[4]);

	if (value != NULL)
	{
		if (*value + delta <= max && *value + delta >= min)
			*value += delta;
	}
	*text = text_type[*value];
}

void					menu_map_editor(t_data data)
{
	(void)data;
	t_game_board board;
	SDL_Event	event;
	bool		play = true;
	t_gui 		gui;

	int index = 1;
	gui_part[0] = t_gui();
	gui_part[1] = t_gui();
	gui_part[2] = t_gui();
	gui_part[3] = t_gui();

	double i = 0;

	t_entry *entry_path = new s_entry(new s_text_entry("File name of your map", "", BLACK,
	t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(8, 1) * gui.unit, 5,
	t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6), t_color(1.0, 0.95, 0.0)));
	entry_path->entry->back = MAP_EXT;
	entry_path->entry->max_len = 32;

	i++;

	t_iterator *gui_iterator = new s_iterator(&index, NULL, 1, 0, 0, 3,
		new t_button(new s_text_button(
			text_type[index], DARK_GREY,
			t_vect(2.2, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"-", DARK_GREY,
			t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL,
		new t_button(new s_text_button(
			"+", DARK_GREY,
			t_vect(6.65, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	gui_iterator->minus->button->funct_left = increment_iterator_text;
	gui_iterator->minus->button->data_left = t_data(5, &index, -1, 0, 3, &(gui_iterator->desc->button->text)); //0 - &value / 1 - &pool / 2 - increment / 3 - cost / 4 - min / 5 - max
	gui_iterator->plus->button->funct_left = increment_iterator_text;
	gui_iterator->plus->button->data_left = t_data(5, &index, 1, 0, 3, &(gui_iterator->desc->button->text)); //0 - &value / 1 - &pool / 2 - increment / 3 - cost / 4 - min / 5 - max

	i++;

	double saved_pos = i;

	size_t j = 0;
	t_vect size = t_vect(gui.unit.y / gui.unit.x, 1);
	t_button	*node_button = new t_button(new s_tileset_button(
				"", DARK_GREY,
				NULL,
				t_vect(-1, -1),
				t_vect(1 + ((size.x + 0.2) * (j % 8)), 1 + ((size.y + 0.2) * (i + j / 8))) * gui.unit,
				size * gui.unit,
				5),
				select_node, t_data(2, &board, ((int)j) - 1));

	gui_part[1].add(node_button);
	while (j < board.node_list.size())
	{
		node_button = new t_button(new s_tileset_button(
					"", DARK_GREY,
					board.node_list[j].tile,
					board.node_list[j].sprite,
					t_vect(1 + ((size.x + 0.2) * (j % 10)), 2.2 + ((size.y + 0.2) * (i + j / 10))) * gui.unit,
					size * gui.unit,
					5),
					select_node, t_data(2, &board, j));

		gui_part[1].add(node_button);
		j++;
	}
	i = saved_pos;

	j = 0;
	while (j < 3)
	{
	t_button	*placement_button = new t_button(new s_tileset_button(
				"", DARK_GREY,
				get_interface_tile("simple_cursor"),
				(j == 0 ? t_vect(0, 0) : (j == 1 ? t_vect(3, 0) : t_vect(4, 0))),
				t_vect(1 + (1.2 * j), 1 + (1.2 * i)) * gui.unit,
				t_vect(1, 1) * gui.unit,
				5),
				select_placement, t_data(2, &board, ((int)j)));

			gui_part[2].add(placement_button);
			j++;
	}
	i = saved_pos;

	t_iterator *exp_iterator = new s_iterator(&(board.exp_reward), NULL, 1, 1, 0, 150000,
		new t_button(new s_text_button(
				"Exp reward: ", DARK_GREY,
				t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"-", DARK_GREY,
				t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"+", DARK_GREY,
				t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
		i++;
	gui_part[3].add(exp_iterator);

	t_iterator *gold_iterator = new s_iterator(&(board.gold_reward), NULL, 1, 1, 0, 150000,
		new t_button(new s_text_button(
				"Gold reward: ", DARK_GREY,
				t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"-", DARK_GREY,
				t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"+", DARK_GREY,
				t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
		i++;
	gui_part[3].add(gold_iterator);

	t_iterator *level_iterator = new s_iterator(&(board.map_difficulty), NULL, 1, 1, 0, 150000,
		new t_button(new s_text_button(
				"Map difficulty : ", DARK_GREY,
				t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"-", DARK_GREY,
				t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"+", DARK_GREY,
				t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
		i++;
	gui_part[3].add(level_iterator);

	t_iterator *price_iterator = new s_iterator(&(board.price), NULL, 1, 1, 0, 150000,
		new t_button(new s_text_button(
				"Price : ", DARK_GREY,
				t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"-", DARK_GREY,
				t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"+", DARK_GREY,
				t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
		i++;
	gui_part[3].add(price_iterator);

	t_iterator *icon_iterator = new s_iterator(&(board.shop_icon), NULL, 1, 1, 0, get_interface_tile("map_icons")->nb_sprite.x * get_interface_tile("map_icons")->nb_sprite.y  - 1,
		new t_button(new s_text_button(
			"Shop icon : ", DARK_GREY,
			t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"<", DARK_GREY,
			t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			">", DARK_GREY,
			t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i++;
	gui_part[3].add(icon_iterator);

	i = 13;
	t_button	*save_button = new t_button(new s_text_button(
		"Save map", DARK_GREY,
		t_vect(1, 1 + (1.2 * i)) * gui.unit, t_vect(8, 1) * gui.unit, 5,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		menu_save_map, t_data(3, &gui, &(board), &(entry_path->entry->text)));//0 - gui / 1 - t_game_board * / 2 - & file name
	t_button	*generate_button = new t_button(new s_text_button(
		"Generate map", DARK_GREY,
		t_vect(10, 1 + (1.2 * i)) * gui.unit,
		t_vect(8, 1) * gui.unit,
		5,
		t_color(0.4, 0.4, 0.4),
		t_color(0.6, 0.6, 0.6)),
		menu_generate_board, t_data(3, &gui, &(board), &(entry_path->entry->text)));
	i++;
	t_button	*load_button = new t_button(new s_text_button(
		"Load map", DARK_GREY,
		t_vect(1, 1 + (1.2 * i)) * gui.unit, t_vect(8, 1) * gui.unit, 5,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		menu_load_map, t_data(3, &gui, &(board), &(entry_path->entry->text))); // 0 - t_gui * / 1 - game_board * / 2 - &path
	t_button	*delete_button = new t_button(new s_text_button(
		"Delete map", DARK_GREY,
		t_vect(10, 1 + (1.2 * i)) * gui.unit,
		t_vect(8, 1) * gui.unit,
		5,
		t_color(0.4, 0.4, 0.4),
		t_color(0.6, 0.6, 0.6)),
		menu_delete_map, t_data(3, &gui, &(board), &(entry_path->entry->text)));
	t_button	*quit_button = new t_button(new s_text_button(
		"Quit", DARK_GREY,
		t_vect(19, 1 + (1.2 * i)) * gui.unit, t_vect(8, 1) * gui.unit, 5,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		menu_quit, t_data(2, &gui, &play));
	i++;

	gui.add(save_button);
	gui.add(gui_iterator);
	gui.add(ENTRY_NUM, entry_path);
	gui.add(generate_button);
	gui.add(load_button);
	gui.add(delete_button);
	gui.add(quit_button);

	while (play)
	{
		prepare_screen(t_color(0.2, 0.2, 0.2));


		board.draw_board();
		board.draw_cell_layer();
		if (index == 2)
			board.draw_placement();
		board.draw_actor_list();
		board.draw_cursor_layer();
		board.draw_mouse_cursor();

		gui.draw_self();
		gui_part[index].draw_self();

		if (index == 3)
			get_interface_tile("map_icons")->draw_self(icon_iterator->container->button->coord[1], icon_iterator->container->button->size[1], board.shop_icon);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE && cell_list.size() == 0))
				menu_quit(t_data(2, &gui, &play));
			else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE && cell_list.size() != 0)
			{
				board.reset_board();
				cell_list.clear();
			}
			else if (event.type == SDL_MOUSEMOTION && event.button.button == SDL_BUTTON_LEFT)
			{
				moved = true;
				control_mouse_editor(&board);
			}
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
			{
				if (gui.click(&event) == false && gui_part[index].click(&event) == false)
					control_mouse_editor(&board);
				moved = false;
			}
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT)
			{
				if (cell_list.size() != 0)
					reset_selection();
				else
				{
					t_cell *cell =  board.get_cell(board.get_mouse_pos());
					if (cell != NULL)
					{
						if (cell->actor == NULL)
							menu_place_monster(t_data(3, &(cell->coord), &board, &gui));
						else if (cell->actor != NULL)
							board.outvoke_actor(cell->actor);
					}
				}
			}
			else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
			{
				gui.key_press(&event);
				gui_part[index].key_press(&event);
			}
			board.handle_control_camera(&event);
		}
		render_screen(true);
	}
}
