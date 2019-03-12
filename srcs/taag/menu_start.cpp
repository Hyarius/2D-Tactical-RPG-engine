#include "taag.h"

int type = 0;

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
	write_in_log("\n");
	write_in_log("\n");
	write_in_log(" --- Statement at menu_start begin : ");
	printOpenGLError();
	printSDLError();
	write_in_log("Everything is settle properly\n");

	write_in_log("\n");
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
	t_gui gui_editor;

	write_in_log("Account : ");
	account = new t_game_engine();
	write_in_log("Creation complete\n");

	write_in_log("\n");

	write_in_log("Gui button background : ");
	gui.add(new s_button(new s_image_button(
			t_image("ressources/assets/interface/Menu_start.png"),
			t_vect(0, 0), get_win_size()),
			NULL, NULL));
	write_in_log("Creation complete\n");

	write_in_log("Gui button play : ");
	gui.add(new t_button(new s_text_button(
			"Play", DARK_GREY,
			t_vect(21, 1) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_play, NULL));
	write_in_log("Creation complete\n");

	write_in_log("Gui button shop : ");
	gui.add(new t_button(new s_text_button(
			"Shop", DARK_GREY,
			t_vect(21, 4) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_shop, NULL));
	write_in_log("Creation complete\n");


	write_in_log("Character slot : ");
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
	write_in_log("Creation complete\n");

	write_in_log("Level info : ");
	t_button *level_text_button = new t_button(new s_text_button(
		"Level :", DARK_GREY,
		t_vect(0.5, 0.5) * gui.unit, t_vect(2, 1) * gui.unit, 6,
		t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
		NULL, NULL);

	t_button *level_value_button = new t_button(new s_text_button(
		"", DARK_GREY,
		t_vect(2.7, 0.5) * gui.unit, t_vect(1.5, 1) * gui.unit, 6,
		t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
		NULL, NULL);

	gui.add(level_text_button);
	gui.add(level_value_button);
	write_in_log("Creation complete\n");

	write_in_log("Exp info : ");
	t_button *exp_text_button = new t_button(new s_text_button(
		"Exp :", DARK_GREY,
		t_vect(4.4, 0.5) * gui.unit, t_vect(1.5, 1) * gui.unit, 6,
		t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
		NULL, NULL);

	t_button *exp_value_button = new t_button(new s_text_button(
		"", DARK_GREY,
		t_vect(6.1, 0.5) * gui.unit, t_vect(4, 1) * gui.unit, 6,
		t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
		NULL, NULL);

	gui.add(exp_value_button);
	gui.add(exp_text_button);
	write_in_log("Creation complete\n");

	write_in_log("Gold info : ");
	t_button *gold_text_button = new t_button(new s_text_button(
		"Gold :", DARK_GREY,
		t_vect(11.1, 0.5) * gui.unit, t_vect(2, 1) * gui.unit, 6,
		t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
		NULL, NULL);

	t_button *gold_value_button = new t_button(new s_text_button(
		"", DARK_GREY,
		t_vect(13.3, 0.5) * gui.unit, t_vect(3.5, 1) * gui.unit, 6,
		t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
		NULL, NULL);
	write_in_log("Creation complete\n");

	gui.add(gold_text_button);
	gui.add(gold_value_button);

	write_in_log("Gui button monster editor : ");
	gui_editor.add(new t_button(new s_text_button(
			"Monster editor", DARK_GREY,
			t_vect(21, 7) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_monster_editor, NULL));
	write_in_log("Creation complete\n");

	write_in_log("Gui button map editor : ");
	gui_editor.add(new t_button(new s_text_button(
			"Map editor", DARK_GREY,
			t_vect(21, 10) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_map_editor, NULL));
	write_in_log("Creation complete\n");

	write_in_log("Gui button spell editor : ");
	gui_editor.add(new t_button(new s_text_button(
			"Spell editor", DARK_GREY,
			t_vect(21, 13) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_spell_editor, NULL));
	write_in_log("Creation complete\n");

	write_in_log("Gui button quit : ");
	gui.add(new t_button(new s_text_button(
			"QUIT", DARK_GREY,
			t_vect(21, 17) * gui.unit, t_vect(8, 2) * gui.unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			menu_quit, t_data(2, &gui, &play)));
	write_in_log("Creation complete\n");

	write_in_log("\n");
	write_in_log("Tutorial buttons : ");
	if (gui_tutorial[1].object_list.size() == 0)
	{
		gui_tutorial[1].add(TUTORIAL_NUM, new s_tutorial_button(NULL, new t_button(new s_paragraph_button(
				"Welcome to TAAG. \n \
Here you're in the main menu. It's composed, as you can see, of 6 character slots just under this text box, and some menu at the right part of the screen. \n \
\n \
The first menu is the play menu, which brings you to a game of TAAG. \n \
The second one is the shop, where you can buy things for your character. \n \n Click here to continue", DARK_GREY, gui.unit.y / 2, //text info
			t_vect(1.5, 1.5) * gui_tutorial[0].unit, t_vect(16, 5) * gui_tutorial[0].unit, 5, //object info
			t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL)
			, true));
	}

	if (gui_tutorial[2].object_list.size() == 0)
	{
		gui_tutorial[2].add(TUTORIAL_NUM, new s_tutorial_button(NULL, new t_button(new s_paragraph_button(
			"Those are your six character slots. \n Each one can contain a fully personalisable character. \n Let's look at this more closely. \n \n Click on one of them to open it.", DARK_GREY, gui.unit.y / 2, //text info
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
				"Now that we have a character, let's try and play a game ! Click on the \"Play\" menu !", DARK_GREY, gui.unit.y / 2, //text info
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
				"Let's open the \"Shop\" menu ! \n \n Click on it.", DARK_GREY, gui.unit.y / 2, //text info
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
	write_in_log("Creation complete\n");

	write_in_log("\n");
	if (account->tuto_state == 0)
	{
		write_in_log("Asking tutorial : ");
		start_tutorial(&gui);
		setting_keyboard(&gui);
		write_in_log("Reception done \n");
	}
	write_in_log("Tutorial set at " + to_string(account->tuto_state) + "\n");
	write_in_log("Asking keyboard : ");
	if (account->tuto_state > TUTO_SIZE)
	{
		setting_keyboard(&gui);
		account->tuto_state = TUTO_SIZE;
	}
	write_in_log("Keyboard set\n");

	write_in_log("\n");
	static int a = 2;
	while (play)
	{
		if (a >= 2)
		{
			write_in_log("--- Starting menu_start loop ---\n");
			write_in_log("Setting button text : ");
		}
		level_value_button->button->text = to_string(account->level);
		exp_value_button->button->text = to_string(account->exp.value) + "/" + to_string(account->exp.max);
		gold_value_button->button->text = to_string(account->gold);
		if (a >= 2)
		{
			write_in_log("DONE\n");
			write_in_log("Prepare screen : ");
		}
		prepare_screen();

		if (a >= 2)
		{
			write_in_log("DONE\n");
			write_in_log("Drawing gui : ");
		}
		gui.draw_self();
		if (type == 1)
			gui_editor.draw_self();
		if ((size_t)(account->tuto_state) < gui_tutorial.size())
			gui_tutorial[account->tuto_state].draw_self();

		if (a >= 2)
		{
			write_in_log("DONE\n");
			write_in_log("Render screen : ");
		}


		render_screen(true);

		if (a >= 2)
		{
			write_in_log("DONE\n");
			write_in_log("Event loop : ");
			a--;
		}
		if (SDL_PollEvent(&event) == 1)
		{
			if (a >= 1)
			{
				write_in_log("DONE\n");
				write_in_log("Check SDL/OpenGL : ");
				printOpenGLError();
				printSDLError();
				write_in_log("No error\n");
				a--;
			}

			const Uint8 *state = SDL_GetKeyboardState(NULL);
			if (state[SDL_SCANCODE_F1] && state[SDL_SCANCODE_F10])
			{
				type = (type == 0 ? 1 : 0);
			}
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(2, &gui, &play));
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
			{
				if (account->tuto_state < TUTO_SIZE && gui_tutorial[account->tuto_state].object_list.size())
					gui_tutorial[account->tuto_state].click(&event);
				else
				{
					gui.click(&event);
					if (type == 1)
						gui_editor.click(&event);
				}
			}
			else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
				gui.key_press(&event);
			SDL_FlushEvents(SDL_MOUSEMOTION, SDL_MOUSEWHEEL);
		}
	}
}
