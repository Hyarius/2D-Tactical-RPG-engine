#include "taag.h"

int type = 1;

bool			menu_status[30];

t_game_engine	 *account;
vector<t_gui>	gui_tutorial;

void		tmp_function(t_data data)
{
	(void)data;
}

void		increment_tutorial(t_data data)
{
	(void)data;
	account->tuto_state++;
}

void		menu_start()
{
	if (gui_tutorial.size() == 0)
		gui_tutorial.resize(TUTO_SIZE);
	for (size_t i = 0; i < TUTO_SIZE; i++)
	{
		gui_tutorial[i] = t_gui();
		if (i != 1)
			gui_tutorial[i].tutorial_value = 0.7;
	}

	SDL_Event	event;
	bool		play = true;
	t_gui gui;

	account = new t_game_engine();

	gui.add(new s_button(new s_image_button(
			t_image("ressources/assets/interface/Menu_start.png"),
			t_vect(0, 0), get_win_size()),
			NULL, NULL));

	gui.add(new t_button(new s_text_button(
			"Play", DARK_GREY,
			t_vect(21, 1) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_play, NULL));

	gui.add(new t_button(new s_text_button(
			"Shop", DARK_GREY,
			t_vect(21, 4) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_shop, NULL));

	t_vect size = t_vect(4, 5);
	for (size_t i = 0; i < 6; i++)
	{
		t_vect coord = t_vect(1 + (size.x + 0.5) * (i % 3), 8 + (size.y + 0.5) * (i / 3));
		t_button *player_button = new t_button(new s_text_button(
				"", DARK_GREY,
				coord * gui.unit, size * gui.unit, 5,
				t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
				menu_actor_editor, t_data(1, &(account->actor[i])));
		t_button *name_button = new t_button(new s_text_button(
				"", DARK_GREY,
				(coord + t_vect(0.25, 0.25)) * gui.unit, t_vect(3.5, 0.5) * gui.unit, 3,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
				NULL, NULL);
		t_button *frame_button = new t_button(new s_text_button(
				"", DARK_GREY,
				(coord + t_vect(0.25, 1.1)) * gui.unit, t_vect(3.5, 3.5) * gui.unit, 3,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
				NULL, NULL);
		t_actor_card *actor_card = new s_actor_card(player_button, &(account->actor_array[i]), name_button, frame_button);
		gui.add(actor_card);
	}

	if (type == 1)
	{
		gui.add(new t_button(new s_text_button(
			"Monster editor", DARK_GREY,
			t_vect(21, 7) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_monster_editor, NULL));

		gui.add(new t_button(new s_text_button(
			"Map editor", DARK_GREY,
			t_vect(21, 10) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_map_editor, NULL));

		gui.add(new t_button(new s_text_button(
			"Spell editor", DARK_GREY,
			t_vect(21, 13) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_spell_editor, NULL));
	}

	gui.add(new t_button(new s_text_button(
			"QUIT", DARK_GREY,
			t_vect(21, 17) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_quit, t_data(2, &gui, &play)));

	if (gui_tutorial[1].object_list.size() == 0)
	{
		gui_tutorial[1].add(TUTORIAL_NUM, new s_tutorial_button(NULL, new t_button(new s_paragraph_button(
				"Welcome to TAAG. \n \
You're here into the main menu. He's compose, as you can see, of 6 characters slots, just under this text box, and some menu, at the right part of the screen. \n \
\n \
The first menu is play play menu, witch can bring you to a game of TAAG. \n \
The second one is the shop, where you can buy things for your character. \n ", DARK_GREY, gui.unit.y / 2, //text info
			t_vect(1.5, 1.5) * gui_tutorial[0].unit, t_vect(16, 4) * gui_tutorial[0].unit, 5, //object info
			t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL)
			, true));
	}

	if (gui_tutorial[2].object_list.size() == 0)
	{
		gui_tutorial[2].add(TUTORIAL_NUM, new s_tutorial_button(NULL, new t_button(new s_paragraph_button(
			"Those are your six character slots. \n Witch one of them can contain one fully personalisable character. \n Let's look at this more closely. \n \n Click on one of them.", DARK_GREY, gui.unit.y / 2, //text info
			t_vect(15, 8) * gui_tutorial[0].unit, t_vect(8, 4) * gui_tutorial[0].unit, 5, //object info
			t_color(222, 184, 135), t_color(245, 222, 179)), tmp_function, NULL), true
		));

		size = t_vect(4, 5);
		for (size_t i = 0; i < 6; i++)
		{
			t_vect coord = t_vect(1 + (size.x + 0.5) * (i % 3), 8 + (size.y + 0.5) * (i / 3));
			t_button *slot_button = new t_button(new s_text_button(
				"", DARK_GREY,
				coord * gui.unit, size * gui.unit, 5,
				t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
				menu_actor_editor, t_data(1, &(account->actor[i])));
			gui_tutorial[2].add(TUTORIAL_NUM, new s_tutorial_button(slot_button, NULL, true));
		}
	}
	if (gui_tutorial[14].object_list.size() == 0)
	{
		gui_tutorial[14] = t_gui(30, 20);

		gui_tutorial[14].add(TUTORIAL_NUM, new s_tutorial_button(new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(21, 1) * gui.unit, t_vect(8, 2) * gui.unit, 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			menu_play, NULL),
			new t_button(new s_paragraph_button(
				"Now than we got a character, let's try and play one game ! Click on the \"Play\" menu !", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(1.5, 1.5) * gui_tutorial[0].unit, t_vect(16, 4) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), tmp_function, NULL), true
		));
	}
	if (gui_tutorial[16].object_list.size() == 0)
	{
		gui_tutorial[16] = t_gui(30, 20);

		gui_tutorial[16].add(TUTORIAL_NUM, new s_tutorial_button(new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(21, 4) * gui.unit, t_vect(8, 2) * gui.unit, 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			menu_shop, NULL),
			new t_button(new s_paragraph_button(
				"Let's open the \"Shop\" menu !", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(1.5, 1.5) * gui_tutorial[0].unit, t_vect(16, 4) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), tmp_function, NULL), true
		));
	}

	if (gui_tutorial[23].object_list.size() == 0)
	{
		gui_tutorial[23] = t_gui(30, 20);

		gui_tutorial[23].add(TUTORIAL_NUM, new s_tutorial_button(new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(21, 1) * gui.unit, t_vect(8, 2) * gui.unit, 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			menu_play, NULL),
			new t_button(new s_paragraph_button(
				"Click on the \"Play\" menu !", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(1.5, 1.5) * gui_tutorial[0].unit, t_vect(16, 4) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), tmp_function, NULL), true
		));
	}


	if (account->tuto_state == 0)
	{
		start_tutorial(&gui);
		setting_keyboard(&gui);
	}

	while (play)
	{
		prepare_screen();

		gui.draw_self();
		if ((size_t)(account->tuto_state) < gui_tutorial.size())
			gui_tutorial[account->tuto_state].draw_self();

		render_screen(true);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(2, &gui, &play));
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
			{
				if (account->tuto_state < TUTO_SIZE && gui_tutorial[account->tuto_state].object_list.size())
					gui_tutorial[account->tuto_state].click(&event);
				else
					gui.click(&event);
			}
			else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
				gui.key_press(&event);
			SDL_FlushEvents(SDL_MOUSEMOTION, SDL_MOUSEWHEEL);
		}
		printOpenGLError();
		printSDLError();
	}
}
