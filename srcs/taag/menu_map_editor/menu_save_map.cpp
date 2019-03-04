#include "taag.h"

static void			stand(t_data data)
{
	*((bool *)data.data[0]) = false;
}

static void			save_map(t_data data) // 0 - t_board * / 1 - file name
{
	t_game_board *board = (t_game_board *)(data.data[0]);
	string path = MAP_PATH + *(string *)(data.data[1]) + MAP_EXT;
	string shop_path = SHOP_MAP_PATH + *(string *)(data.data[1]) + SHOP_EXT;

	ofstream myfile;
	myfile.open (shop_path);

	myfile << "string to add:" + *(string *)(data.data[1]) + '\n';
	myfile << "tile:map_icons\n";
	myfile << "sprite:" + to_string(board->shop_icon) + '\n';
	myfile << "price:" + to_string(board->price) + '\n';

	myfile.close();
	myfile.open (path);

	myfile << "price:" + to_string(board->price) + '\n';
	myfile << "shop_icon:" + to_string(board->shop_icon) + '\n';
	myfile << "level:" + to_string(board->map_difficulty) + '\n';
	myfile << "exp reward:" + to_string(board->exp_reward) + '\n';
	myfile << "gold reward:" + to_string(board->gold_reward) + '\n';

	int j = 0;
	while (j < board->board_size.y)
	{
		int i = 0;
		while (i < board->board_size.x)
		{
			if (board->cell_layer[i][j].node != &empty_node)
			{
				size_t node_type = 0;
				while (node_type < board->node_list.size() && &(board->node_list[node_type]) != board->cell_layer[i][j].node)
					node_type++;
				myfile << to_string(i) + ":" + to_string(j) + ":" + to_string(node_type);
				if (board->cell_layer[i][j].actor != NULL && board->cell_layer[i][j].actor->team != 1)
					myfile << ":" + board->cell_layer[i][j].actor->path + ":" + to_string(board->cell_layer[i][j].actor->team);
				myfile << "\n";
			}
			i++;
		}
		j++;
	}
	myfile << "placement :\n";
	size_t i = 0 ;
	while (i < board->placement_list.size())
	{
		myfile << to_string((int)(board->placement_list[i].x)) + ":" + to_string((int)(board->placement_list[i].y)) + ":0\n";
		i++;
	}
	i = 0 ;
	while (i < board->enemy_placement_list.size())
	{
		myfile << to_string((int)(board->enemy_placement_list[i].x)) + ":" + to_string((int)(board->enemy_placement_list[i].y)) + ":2\n";
		i++;
	}
	myfile.close();
}

static void			quit_save(t_data data)
{
	save_map(t_data(2, data.data[0], data.data[1]));
	bool *play = (bool *)(data.data[2]);
	*play = false;
}

void 				menu_save_map(t_data data) //0 - gui / 1 - t_game_board * / 2 - & file name
{
	string name = (*((string *)(data.data[2])) == "" ? "default" : *((string *)(data.data[2])) );
	string full_path = MAP_PATH + name + MAP_EXT;

	t_gui		gui = t_gui(15, 10);
	SDL_Event	event;

	bool		play = true;

	s_button *button = new s_button(new s_text_button(//button did you wanna quit
						"Did you want to save this file ?", DARK_GREY, //text info
						gui.unit * t_vect(4, 2), gui.unit * t_vect(7, 5), 8, //object info
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
						NULL, NULL);
	button->button->image_coord = button->button->image_coord - (gui.unit * t_vect(0, 1));

	gui.add(button);

	s_button *button2 = new s_button(new s_text_button(//button did you wanna quit
						full_path, DARK_GREY, //text info
						gui.unit * t_vect(4, 2), gui.unit * t_vect(7, 5), 8, //object info
						t_color(0, 0, 0, 0), t_color(0, 0, 0, 0)),
						NULL, NULL);

	gui.add(button2);

	gui.add(new s_button(new s_text_button(//button yes
						"YES", DARK_GREY, //text info
						gui.unit * t_vect(4.25, 5.25), gui.unit * t_vect(3, 1.5), 8, //object info
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
						quit_save, t_data(3, data.data[1], &name, &play)));

	gui.add(new s_button(new s_text_button(//button no
						"NO", DARK_GREY, //text info
						gui.unit * t_vect(7.75, 5.25), gui.unit * t_vect(3, 1.5), 8, //object info
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
						stand, &play));

	while (play == true)
	{
		prepare_screen();

		if (data.data.size() != 0)
			(*((t_gui *)(data.data[0]))).draw_self();
		gui.draw_self();

		render_screen(true);

		if (SDL_PollEvent(&(event)) == 1)
		{
			if (event.type == SDL_QUIT)
				menu_quit(t_data(1, &gui));
			else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
				play = false;
			else if (event.type == SDL_MOUSEBUTTONUP)
				gui.click(&event);
		}
	}

}
