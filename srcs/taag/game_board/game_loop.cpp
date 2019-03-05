#include "taag.h"

static void			tutorial_move_function(t_data data)
{
	(void)data;
	account->board.move_actor(account->board.turn_order[account->board.turn_index], account->board.get_mouse_pos(), 1.0);
}

static void			tutorial_cast_spell(t_data data)
{
	(void)data;
	bool result = account->board.cast_spell(account->board.get_mouse_pos());
}

void				s_game_board::game_loop()
{
	SDL_Event	event;
	bool		play = true;

	printf("here\n");
	initiate_turn_order();

	gui = t_gui(30, 20);
	gui.add(new s_button(new t_image_button(t_image("ressources/assets/interface/GUI_Shortcut.png"), gui.unit * t_vect(7.5, 18), gui.unit * t_vect(15, 2)), NULL, NULL));
	for (int i = 0; i < 6; i++)
		gui.add(SPELL_BUTTON, new s_button(new t_text_button("", BLACK, gui.unit * t_vect((i < 3 ? 8 : 18) + ((i % 3) * 1.5), 18.5), gui.unit, 0, t_color(0.3, 0.3, 0.3, 0.0), t_color(0.6, 0.6, 0.6, 0.0)), change_s_spell, t_data(3, &s_spell, &calculated, i)));

	while (play)
	{
		prepare_screen();

		update_board();
		draw_self();
		draw_gui();

		if (enemy_list.size() == 0 || ally_list.size() == 0)
			ending_fight(&play);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(2, &gui, &play));
			handle_control_camera(&event);
			handle_control_game(&event);
		}
		render_screen(true);
	}
}

void				s_game_board::game_loop_tutorial()
{
	SDL_Event	event;
	bool		play = true;

	initiate_turn_order();

	gui = t_gui(30, 20);
	gui.add(new s_button(new t_image_button(t_image("ressources/assets/interface/GUI_Shortcut.png"), gui.unit * t_vect(7.5, 18), gui.unit * t_vect(15, 2)), NULL, NULL));
	for (int i = 0; i < 6; i++)
		gui.add(SPELL_BUTTON, new s_button(new t_text_button("", BLACK, gui.unit * t_vect((i < 3 ? 8 : 18) + ((i % 3) * 1.5), 18.5), gui.unit, 0, t_color(0.3, 0.3, 0.3, 0.0), t_color(0.6, 0.6, 0.6, 0.0)), change_s_spell, t_data(3, &s_spell, &calculated, i)));


	string text[6] = { SDL_GetKeyName(account->keyboard[0]), SDL_GetKeyName(account->keyboard[1]) , SDL_GetKeyName(account->keyboard[2]) , SDL_GetKeyName(account->keyboard[3]) , SDL_GetKeyName(account->keyboard[4]) , SDL_GetKeyName(account->keyboard[5]) };

	if (gui_tutorial[26].object_list.size() == 0)
	{
		t_button *button = new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(0, 0), get_win_size(), 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			NULL, NULL);

		t_button *button2 = new t_button(new s_paragraph_button(
			"Here we go ! \n Oh, look, there is a wild rat. It looks aggressive ... Better get ready, we don't know what this monster is able of... \n \n To Fight him, we have our awesome Aqua-sword ! It's on the 1 spell slot (Shortcut : " + text[0] + "). \n But first, we need to get close to him. \n As you can see, there is some green squares around your fabulous " + ally_list[0]->name + ". It describe the range where he can run to, in one turn. Every square cost an amount of MP to reach (You can find more informations about a certain square on the top-right corner of the screen by hovering your mouse on it).", DARK_GREY, gui.unit.y / 2, //text info
			t_vect(0.5, 0.5) * gui_tutorial[0].unit, t_vect(10, 10) * gui_tutorial[0].unit, 5, //object info
			t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL);

		gui_tutorial[26].add(TUTORIAL_NUM, new s_tutorial_button(button, button2, true));
	}
	if (gui_tutorial[27].object_list.size() == 0)
	{
		gui_tutorial[27] = t_gui(30, 20);

		t_button *button = new t_button(new s_text_button(
			"", DARK_GREY,
			get_tile_pos(3, 5), get_tile_size(), 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			tutorial_move_function, NULL);

		t_button *button2 = new t_button(new s_paragraph_button(
			"Now it's time ! Go kill this rat ! Move to the targeted square !", DARK_GREY, gui.unit.y / 2, //text info
			t_vect(0.5, 0.5) * gui_tutorial[0].unit, t_vect(10, 10) * gui_tutorial[0].unit, 5, //object info
			t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL);

		gui_tutorial[27].add(TUTORIAL_NUM, new s_tutorial_button(button, button2, true));
	}

	if (gui_tutorial[28].object_list.size() == 0)
	{
		t_button *button = new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(0, 0), get_win_size(), 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			NULL, NULL);

		t_button *button2 = new t_button(new s_paragraph_button(
			"Alright ! Now that we moved, let's try to cast our powerful Aqua-sword. \n \n Oh, this rat is too far away... I guess we have to pass our turn for now. For that, press the \"space\" key.", DARK_GREY, gui.unit.y / 2, //text info
			t_vect(0.5, 0.5) * gui_tutorial[0].unit, t_vect(10, 10) * gui_tutorial[0].unit, 5, //object info
			t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL);

		gui_tutorial[28].add(TUTORIAL_NUM, new s_tutorial_button(button, button2, false));
	}

	if (gui_tutorial[30].object_list.size() == 0)
	{
		gui_tutorial[30] = t_gui(30, 20);

		t_button *button = new t_button(new s_text_button(
			"", DARK_GREY,
			get_tile_pos(3, 3), get_tile_size(), 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			tutorial_move_function, NULL);

		t_button *button2 = new t_button(new s_paragraph_button(
			"Oh, the rat is aggressive ! Let's try to kill it ! First of all, move too and get closer to him !", DARK_GREY, gui.unit.y / 2, //text info
			t_vect(0.5, 0.5) * gui_tutorial[0].unit, t_vect(10, 10) * gui_tutorial[0].unit, 5, //object info
			t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL);

		gui_tutorial[30].add(TUTORIAL_NUM, new s_tutorial_button(button, button2, true));
	}

	if (gui_tutorial[31].object_list.size() == 0)
	{
		t_button *button = new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(0, 0), get_win_size(), 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			NULL, NULL);

		t_button *button2 = new t_button(new s_paragraph_button(
			"Perfect ! Cast your amazing Aqua-sword and destroy this little rat ! \n (Press " + text[0] + " to select your Aqua-sword spell)", DARK_GREY, gui.unit.y / 2, //text info
			t_vect(0.5, 0.5) * gui_tutorial[0].unit, t_vect(10, 10) * gui_tutorial[0].unit, 5, //object info
			t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL);

		gui_tutorial[31].add(TUTORIAL_NUM, new s_tutorial_button(button, button2, true));
	}

	if (gui_tutorial[32].object_list.size() == 0)
	{
		gui_tutorial[32] = t_gui(30, 20);

		t_button *button = new t_button(new s_text_button(
			"", DARK_GREY,
			get_tile_pos(4, 3), get_tile_size(), 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			tutorial_cast_spell, NULL);

		t_button *button2 = new t_button(new s_paragraph_button(
			"Okay ! Time to cast this incredible attack on his face ! Click on it !", DARK_GREY, gui.unit.y / 2, //text info
			t_vect(0.5, 0.5) * gui_tutorial[0].unit, t_vect(10, 10) * gui_tutorial[0].unit, 5, //object info
			t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL);

		gui_tutorial[32].add(TUTORIAL_NUM, new s_tutorial_button(button, button2, true));
	}

	if (gui_tutorial[33].object_list.size() == 0)
	{
		t_button *button = new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(0, 0), get_win_size(), 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			NULL, NULL);

		t_button *button2 = new t_button(new s_paragraph_button(
			"Perfect ! But ... Looks like you didn't kill him... And you dont't have enough AP to cast again your Aqua-sword. \n \n You should pass your turn and finaly kill him.", DARK_GREY, gui.unit.y / 2, //text info
			t_vect(0.5, 0.5) * gui_tutorial[0].unit, t_vect(10, 10) * gui_tutorial[0].unit, 5, //object info
			t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL);

		gui_tutorial[33].add(TUTORIAL_NUM, new s_tutorial_button(button, button2, false));
	}

	if (gui_tutorial[35].object_list.size() == 0)
	{
		t_button *button = new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(0, 0), get_win_size(), 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			NULL, NULL);

		t_button *button2 = new t_button(new s_paragraph_button(
			"Okay, now it's your turn ! Impress us, and purge this rat ! \n \n - Spell 1 : " + text[0] + " - Aqua-sword \n - Spell 2 : " + text[1] + " \n - Spell 3 : " + text[2] + " \n - Spell 4 : " + text[3] + " \n - Spell 5 : " + text[4] + " \n - Spell 6 : " + text[5] + " \n - Pass your turn : Space", DARK_GREY, gui.unit.y / 2, //text info
			t_vect(0.5, 0.5) * gui_tutorial[0].unit, t_vect(10, 10) * gui_tutorial[0].unit, 5, //object info
			t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL);

		gui_tutorial[35].add(TUTORIAL_NUM, new s_tutorial_button(button, button2, false));
	}

	while (play)
	{
		prepare_screen();

		update_board();
		draw_self();
		draw_gui();

		if (account->tuto_state < TUTO_SIZE)
		{
			((t_tutorial_button *)(gui_tutorial[27].object_list[TUTORIAL_NUM][0]))->button->button->coord[0] = get_tile_pos(3, 5);
			((t_tutorial_button *)(gui_tutorial[27].object_list[TUTORIAL_NUM][0]))->button->button->size[0] = get_tile_size();
			((t_tutorial_button *)(gui_tutorial[30].object_list[TUTORIAL_NUM][0]))->button->button->coord[0] = get_tile_pos(4, 4);
			((t_tutorial_button *)(gui_tutorial[30].object_list[TUTORIAL_NUM][0]))->button->button->size[0] = get_tile_size();
			((t_tutorial_button *)(gui_tutorial[32].object_list[TUTORIAL_NUM][0]))->button->button->coord[0] = get_tile_pos(5, 4);
			((t_tutorial_button *)(gui_tutorial[32].object_list[TUTORIAL_NUM][0]))->button->button->size[0] = get_tile_size();
			gui_tutorial[account->tuto_state].draw_self();
		}

		if (enemy_list.size() == 0 || ally_list.size() == 0)
			ending_fight(&play);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
			{
				menu_quit(t_data(2, &gui, &play));
			}
			if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT && account->tuto_state < TUTO_SIZE && (account->tuto_state != 31 || (account->tuto_state == 31 && s_spell != -1)))
				gui_tutorial[account->tuto_state].click(&event);
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE && (account->tuto_state == 28 || account->tuto_state == 33))
			{
				account->tuto_state++;
				next_turn();
				account->tuto_state++;
			}
			if (event.type == SDL_KEYUP && event.key.keysym.sym == account->keyboard[0] && account->tuto_state == 31)
			{
				account->tuto_state++;
				s_spell = 0;
				calculated = false;
			}
			if (account->tuto_state >= 35)
			{
				handle_control_camera(&event);
				handle_control_game(&event);
			}
		}
		render_screen(true);
	}
}
