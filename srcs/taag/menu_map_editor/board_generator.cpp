#include "taag.h"

static void			change_bool(t_data data)
{
	*((bool *)data.data[0]) = false;
}

static void			quit_generate_menu(t_data data) //0 - board | 1 - x | 2 - y | 3 - bool
{
	t_game_board *board = (t_game_board *)(data.data[0]);
	int size_x = *((int *)(data.data[1]));
	int size_y = *((int *)(data.data[2]));
	int node = *((int *)(data.data[4]));
	string			*path = (string *)(data.data[5]);
	t_vect target = board->target;
	double zoom = board->zoom;
	if (board->board_size != t_vect(0, 0))
	{
		*board = board_generator(size_x, size_y, node);
		board->target = target;
		board->zoom = zoom;
	}
	else
		*board = board_generator(size_x, size_y, node);
	*((bool *)data.data[3]) = false;
	*path = "";
}

t_game_board		board_generator(int size_x, int size_y, int node)
{
	t_game_board	board = t_game_board();
	int				x;
	int				y;

	board.target = t_vect(-size_x, -size_y) / 2;
	board.board_size = t_vect(size_x, size_y);
	board.offset = get_win_size() / 2;
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
			int type = generate_nbr(0, 100);
			if (type < 70)
				board.cell_layer[x][y] = t_cell(t_vect(x, y), &(board.node_list[node]));
			else if (type < 85)
				board.cell_layer[x][y] = t_cell(t_vect(x, y), &(board.node_list[node + 1]));
			else
				board.cell_layer[x][y] = t_cell(t_vect(x, y), &(board.node_list[node + 2]));
			y++;
		}
		x++;
	}
	return (board);
}

void	menu_generate_board(t_data data)
{
	t_game_board *board = (t_game_board *)(data.data[1]);
	string			*path = (string *)(data.data[2]);
	SDL_Event	event;
	bool		play = true;
	t_gui 		gui;

	double i = 0;
	t_button *back_ground = new t_button(new s_text_button(
		"", DARK_GREY,
		t_vect(0.5, 0.5) * gui.unit, t_vect(9, 8) * gui.unit, 5,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL);

	s_button *button = new s_button(new s_text_button(//button did you wanna quit
						"--- Generating menu ---", DARK_GREY, //text info
						t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, gui.unit * t_vect(8, 1), 8, //object info
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
						NULL, NULL);

	i++;

	int size_x = (board->board_size.x != 0 ? board->board_size.x : 5);

	t_iterator *size_x_iterator = new s_iterator(&size_x, NULL, 1, 1, 1, 30,
		new t_button(new s_text_button(
			"Size X : ", DARK_GREY,
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

	int size_y = (board->board_size.y != 0 ? board->board_size.y : 5);

	t_iterator *size_y_iterator = new s_iterator(&size_y, NULL, 1, 1, 1, 30,
		new t_button(new s_text_button(
			"Size Y : ", DARK_GREY,
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

	int node_type = 0;

	t_vect coord = t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit;
	t_vect size = t_vect(1, 1) * gui.unit;

	t_iterator *node_iterator = new s_iterator(&node_type, NULL, 10, 1, 0, board->node_list.size() - 1,
		new t_button(new s_text_button(
			"Node : ", DARK_GREY,
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

	t_button	*generate_button = new t_button(new s_text_button(
		"Generate", DARK_GREY,
		t_vect(1, 1 + (1.2 * i)) * gui.unit, t_vect(8, 1) * gui.unit, 5,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		quit_generate_menu, t_data(6, &(board), &size_x, &size_y, &play, &node_type, path)); //0 - board | 1 - x | 2 - y | 3 - bool | 4 - node type
	i++;

	t_button	*quit_button = new t_button(new s_text_button(
		"Quit", DARK_GREY,
		t_vect(1, 1 + (1.2 * i)) * gui.unit, t_vect(8, 1) * gui.unit, 5,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		change_bool, t_data(1, &play));
	i++;

	gui.add(back_ground);
	gui.add(button);
	gui.add(size_x_iterator);
	gui.add(size_y_iterator);
	gui.add(node_iterator);
	gui.add(generate_button);
	gui.add(quit_button);

	while (play)
	{
		prepare_screen();

		if (data.data.size() != 0)
		{
			(*((t_gui *)(data.data[0]))).draw_self();
			board->draw_board();
		}
		gui.draw_self();

		board->node_list[node_type].tile->draw_self(coord + 5, size - 10, board->node_list[node_type].sprite);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(2, &gui, &play));
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click(&event);
			else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
				gui.key_press(&event);
		}
		render_screen(true);
	}
}
