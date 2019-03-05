#include "taag.h"

t_gui		gui_part_shop[3];


typedef 			void(*gui_funct)(t_data);

gui_funct		funct_tab[3] = {
	modify_index_sprite_tab,
	modify_index_spell_tab,
	modify_index_map_tab,
};

int 			index_part[] = {
	0,
	0,
	0,
};

int				index_delta[3] = {
	6,
	6,
	6,
};

static void adding_map(t_data data)
{
	(void)data;
	account->map_unlock.push_back("tutorial-00");
	actualize_map_tab(&(index_part[2]));
}

void		set_index(t_data data)
{
	int *index = (int *)(data.data[0]);
	int value = (int &)(data.data[1]);

	*index = value;
	if (*index == 0)
		create_tab_sprite(t_data(2, &(gui_part_shop[0]), &(index_part[0])));
	else if (*index == 1)
		create_tab_spell(t_data(2, &(gui_part_shop[1]), &(index_part[1])));
	else if (*index == 2)
		create_tab_map(t_data(2, &(gui_part_shop[2]), &(index_part[2])));
}

void menu_shop(t_data data)
{
	int index = 1;
	(void)data;
	SDL_Event	event;
	bool		play = true;
	t_gui gui;

	for (int i = 0; i < 3; i++)
		index_part[i] = 0;

	t_button *back = new t_button(new t_image_button(t_image(t_color(0.2, 0.2, 0.2)), t_vect(0, 0), get_win_size()), NULL, NULL);

	t_button *sprite_tabs = new t_button(new s_text_button(
			"SPRITE", DARK_GREY,
			t_vect(1, 1) * gui.unit, t_vect(3, 1) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			set_index, t_data(2, &index, 0));
	t_button *spell_tabs = new t_button(new s_text_button(
			"SPELL", DARK_GREY,
			t_vect(4.1, 1) * gui.unit, t_vect(3, 1) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			set_index, t_data(2, &index, 1));
	t_button *map_tabs = new t_button(new s_text_button(
			"MAP", DARK_GREY,
			t_vect(7.2, 1) * gui.unit, t_vect(3, 1) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			set_index, t_data(2, &index, 2));
	t_button *back_ground = new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(1, 2.1) * gui.unit, t_vect(28, 16.9) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			NULL, NULL);

	t_iterator *level_iterator = new s_iterator(&(account->level), NULL, 0, 1, 0, 150,
		new t_button(new s_text_button(
				"Level : ", DARK_GREY,
				t_vect(13, 1) * gui.unit, t_vect(2, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL,
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(15.2, 1) * gui.unit, t_vect(2, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL);

	t_iterator *gold_iterator = new s_iterator(&(account->gold), NULL, 0, 1, 0, 150,
		new t_button(new s_text_button(
				"Gold : ", DARK_GREY,
				t_vect(19, 1) * gui.unit, t_vect(2, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL,
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(21.2, 1) * gui.unit, t_vect(2, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL);

	gui.add(back);
	gui.add(sprite_tabs);
	gui.add(spell_tabs);
	gui.add(map_tabs);
	gui.add(back_ground);
	gui.add(level_iterator);
	gui.add(gold_iterator);

	if (gui_tutorial[17].object_list.size() == 0)
	{
		gui_tutorial[17] = t_gui(30, 20);

		gui_tutorial[17].add(TUTORIAL_NUM, new s_tutorial_button(NULL,
			new t_button(new s_paragraph_button(
				"Welcome to the \"Shop\" menu !", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(9.5, 8.55) * gui_tutorial[0].unit, t_vect(10, 4) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL), true
		));
	}

	if (gui_tutorial[18].object_list.size() == 0)
	{
		gui_tutorial[18].add(TUTORIAL_NUM, new s_tutorial_button(new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(13, 1) * gui.unit, t_vect(10.2, 1) * gui.unit, 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			menu_shop, NULL),
			new t_button(new s_paragraph_button(
				"Here, you can see your account level and gold", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(13, 2.2) * gui_tutorial[0].unit, t_vect(10.2, 1) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL), true
		));
	}

	if (gui_tutorial[19].object_list.size() == 0)
	{
		gui_tutorial[19].add(TUTORIAL_NUM, new s_tutorial_button(new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(1, 1) * gui.unit, t_vect(9.2, 1) * gui.unit, 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			menu_shop, NULL),
			new t_button(new s_paragraph_button(
				"There, you have the three tabs of the shop menu \n - Sprite : Here, you can find new looks for your characters \n - Spell : Here, you will find every purchasable spell \n - Map : A list of every purchasable map", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(1, 2.2) * gui_tutorial[0].unit, t_vect(9.2, 4.7) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL), true
		));
	}

	if (gui_tutorial[20].object_list.size() == 0)
	{
		gui_tutorial[20].add(TUTORIAL_NUM, new s_tutorial_button(new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(7.2, 1) * gui.unit, t_vect(3, 1) * gui.unit, 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			set_index, t_data(2, &index, 2)),
			new t_button(new s_paragraph_button(
				"Let's go to the map tab to buy our first map !", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(1, 2.2) * gui_tutorial[0].unit, t_vect(9.2, 4.7) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), tmp_function, NULL), true
		));
	}

	if (gui_tutorial[21].object_list.size() == 0)
	{
		gui_tutorial[21].add(TUTORIAL_NUM, new s_tutorial_button(new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(1.25, 2.4) * gui.unit, t_vect(3.6, 5.2) * gui.unit, 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			adding_map, NULL),
			new t_button(new s_paragraph_button(
				"All right ! Here is the only map we can find here ... \n \"Tutorial-00\"... \n You can see some informations about it : \n - It's price in the gold button. \n - The amount of gold / exp you can get by finishing it. \n The adviced account level to win this map. \n \n Let's buy it ! Left click on this map to buy it !", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(9.5, 8.55) * gui_tutorial[0].unit, t_vect(10, 7) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), tmp_function, NULL), true
		));
	}

	if (gui_tutorial[22].object_list.size() == 0)
	{
		gui_tutorial[22].add(TUTORIAL_NUM, new s_tutorial_button(NULL,
			new t_button(new s_paragraph_button(
				"Let's try our new map ! \n \n Press echap to go back to the main menu and try it !", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(9.5, 8.55) * gui_tutorial[0].unit, t_vect(10, 4) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), tmp_function, NULL), true
		));
	}

	create_tab_spell(t_data(2, &(gui_part_shop[1]), &(index_part[1])));

	while (play)
	{

		prepare_screen();

		gui.draw_self();
		gui_part_shop[index].draw_self();
		if ((size_t)(account->tuto_state) < gui_tutorial.size())
			gui_tutorial[account->tuto_state].draw_self();

		render_screen(true);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT)
			{
				menu_quit(t_data(2, &gui, &play));
			}
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
			{
				play = false;
				if (account->tuto_state < 22)
					account->tuto_state = 15;
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				if ((size_t)(account->tuto_state) < gui_tutorial.size() && gui_tutorial[account->tuto_state].object_list.size())
					gui_tutorial[account->tuto_state].click(&event);
				else
				{
					gui.click(&event);
					gui_part_shop[index].click(&event);
				}
			}
			else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
				gui.key_press(&event);
			else if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_UP))
				funct_tab[index](t_data(2, &(index_part[index]), -index_delta[index]));
			else if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_DOWN))
				funct_tab[index](t_data(2, &(index_part[index]), index_delta[index]));
			else if (event.type == SDL_MOUSEWHEEL)
			{
				if (event.wheel.y > 0)
					funct_tab[index](t_data(2, &(index_part[index]), -index_delta[index]));
				else if (event.wheel.y < 0)
					funct_tab[index](t_data(2, &(index_part[index]), index_delta[index]));
			}
		}
	}
	if (account->tuto_state < TUTO_SIZE)
		account->tuto_state++;
}
