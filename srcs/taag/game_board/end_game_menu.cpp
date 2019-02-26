#include "taag.h"

void				quit_end_menu(t_data data)
{
	bool *play = (bool *)(data.data[0]);
	int *exp_bonus = (int *)(data.data[1]);
	int *gold_bonus = (int *)(data.data[2]);

	account->add_exp(*exp_bonus);
	account->add_gold(*gold_bonus);
	*play = false;
	save_game_engine();
}

void				s_game_board::end_game_win()
{
	t_gui gui = t_gui();
	bool play = true;
	SDL_Event	event;

	int exp_bonus = account->board.exp_reward;
	int	gold_bonus = account->board.gold_reward;

	t_button	*background = new t_button(new s_text_button(
		"", DARK_GREY,
		t_vect(1, 1) * gui.unit, t_vect(28, 18) * gui.unit, 5,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		NULL, NULL);

	t_button	*victory_message = new t_button(new s_text_button(
		"VICTORY", DARK_YELLOW,
		t_vect(2, 2) * gui.unit, t_vect(10, 2) * gui.unit, 5,
		t_color(0.5, 0.5, 0.5), t_color(0.7, 0.7, 0.7)),
		NULL, NULL);

	t_iterator *level_iterator = new s_iterator(&(account->level), NULL, 0, 1, 0, 150,
		new t_button(new s_text_button(
				"Level : ", DARK_GREY,
				t_vect(12.5, 2.5) * gui.unit, t_vect(2, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL,
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(15, 2.5) * gui.unit, t_vect(2, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL);

	t_iterator *exp_iterator = new s_iterator(&(account->exp.value), NULL, 0, 1, 0, 150,
		new t_button(new s_text_button(
				"Exp. : ", DARK_GREY,
				t_vect(18, 2.5) * gui.unit, t_vect(2, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL,
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(20.5, 2.5) * gui.unit, t_vect(2, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL);

	t_iterator *gold_iterator = new s_iterator(&(account->gold), NULL, 0, 1, 0, 150,
		new t_button(new s_text_button(
				"Gold : ", DARK_GREY,
				t_vect(23.5, 2.5) * gui.unit, t_vect(2, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL,
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(26, 2.5) * gui.unit, t_vect(2, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL);

	t_button	*continue_button = new t_button(new s_text_button(
		"CONTINUE", DARK_GREY,
		t_vect(23, 16.5) * gui.unit, t_vect(5, 1.5) * gui.unit, 5,
		t_color(0.5, 0.5, 0.5), t_color(0.7, 0.7, 0.7)),
		quit_end_menu, t_data(3, &play, &exp_bonus, &gold_bonus));

	gui.add(background);
	gui.add(victory_message);
	gui.add(level_iterator);
	gui.add(exp_iterator);
	gui.add(gold_iterator);
	gui.add(continue_button);

	int frame = 0;

	while (play == true)
	{
		if (exp_bonus > 1 && frame == 0)
		{
			exp_bonus--;
			account->add_exp(1);
		}
		if (gold_bonus > 1 && frame == 0)
		{
			gold_bonus--;
			account->add_gold(1);
		}
		frame++;
		if (frame == 5)
		{
			frame = 0;
		}

		prepare_screen(t_color(0.2, 0.2, 0.2));

		gui.draw_self();

		render_screen(true);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT)
			{
				account->add_exp(exp_bonus);
				account->add_gold(gold_bonus);
				exp_bonus = 0;
				gold_bonus = 0;
				menu_quit(t_data(2, &gui, &play));
			}
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE && gold_bonus == 0 && exp_bonus == 0)
				quit_end_menu(t_data(3, &play, &exp_bonus, &gold_bonus));
			if (event.type == SDL_KEYUP && (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_RETURN) && (gold_bonus != 0 || exp_bonus != 0))
			{
				account->add_exp(exp_bonus);
				account->add_gold(gold_bonus);
				exp_bonus = 0;
				gold_bonus = 0;
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
				gui.click(&event);
		}
	}
}

void				s_game_board::end_game_lose()
{
	t_gui gui = t_gui();
	bool play = true;
	SDL_Event	event;

	t_button	*background = new t_button(new s_text_button(
		"", DARK_GREY,
		t_vect(1, 1) * gui.unit, t_vect(28, 18) * gui.unit, 5,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		NULL, NULL);

	t_button	*victory_message = new t_button(new s_text_button(
		"YOU LOSE", DARK_RED,
		t_vect(10, 2) * gui.unit, t_vect(10, 2) * gui.unit, 5,
		t_color(0.5, 0.5, 0.5), t_color(0.7, 0.7, 0.7)),
		NULL, NULL);

	t_button	*continue_button = new t_button(new s_text_button(
		"CONTINUE", DARK_GREY,
		t_vect(23, 16.5) * gui.unit, t_vect(5, 1.5) * gui.unit, 5,
		t_color(0.5, 0.5, 0.5), t_color(0.7, 0.7, 0.7)),
		NULL, NULL);

	gui.add(background);
	gui.add(victory_message);
	gui.add(continue_button);

	while (play == true)
	{
		prepare_screen(t_color(0.2, 0.2, 0.2));

		gui.draw_self();

		render_screen(true);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(2, &gui, &play));
		}
	}
}
