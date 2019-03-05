#include "taag.h"

t_gui gui_part_end;

vector<t_vect>		coord_button = {
	t_vect(9, 		5.75),
	t_vect(13.5, 	5.75),
	t_vect(18, 		5.75),
	t_vect(22.5,	5.75),
	t_vect(9, 		7),
	t_vect(13.5, 	7),
	t_vect(18, 		7),
	t_vect(22.5, 	7),
	t_vect(9, 		8.25),
	t_vect(13.5, 	8.25),
	t_vect(18, 		8.25),
	t_vect(22.5, 	8.25),
	t_vect(9, 		9.5),
	t_vect(13.5, 	9.5),
	t_vect(9, 		10.75),
	t_vect(13.5, 	10.75),
	t_vect(18, 		10.75),
	t_vect(22.5,	10.75),
	t_vect(9, 		12),
	t_vect(13.5, 	12),
	t_vect(9, 		13.25),
	t_vect(13.5, 	13.25),
	t_vect(18, 		13.25),
	t_vect(22.5,	13.25),
};

vector<string>		desc_text = {
	"Total damage dealt :",
	"Total heal dealth :",
	"Total damage taken :",
	"Total armor given :",
	"Total PA taken :",
	"Total PM taken :",
	"Total PA given :",
	"Total PM given :",
	"Total actor pushed :",
	"Total actor pulled :",
	"Total damage to himself :",
	"Total move/placement :",
	"Total poison damage dealt :",
	"Total regeneration dealt :",
	"Total PA taken by effect :",
	"Total PM taken by effect :",
	"Total PA given by effect :",
	"Total PM given by effect :",
	"Total poison cured :",
	"Total regeneration cured :",
	"Total malus PA cured :",
	"Total malus PM cured :",
	"Total bonus PA cured :",
	"Total bonus PM cured :",
};

static void			select_player_info(t_data data)
{
	t_vect			desc_part = t_vect(2.5, 1);
	t_vect			value_part = t_vect(1, 1);
	t_actor *actor = (t_actor *)(data.data[0]);
	gui_part_end = t_gui(30, 20);
	if (actor != NULL)
	{
		t_button	*name_button = new t_button(new s_text_button(
			"Actor : " + actor->name, DARK_GREY,
			t_vect(9, 4.5) * gui_part_end.unit, t_vect(6, 1) * gui_part_end.unit, 5,
			t_color(0.5, 0.5, 0.5), t_color(0.7, 0.7, 0.7)),
			NULL, NULL);
		gui_part_end.add(name_button);
		for (size_t i = 0; i < coord_button.size(); i++)
		{
			t_button *new_desc = new t_button(new s_text_button(
				desc_text[i], DARK_GREY,
				coord_button[i] * gui_part_end.unit, desc_part * gui_part_end.unit, 5,
				t_color(0.5, 0.5, 0.5), t_color(0.7, 0.7, 0.7)),
				NULL, NULL);
			t_button *new_value = new t_button(new s_text_button(
				to_string(actor->total_effect[i]), DARK_GREY,
				(coord_button[i] + t_vect(desc_part.x + 0.2, 0)) * gui_part_end.unit, value_part * gui_part_end.unit, 5,
				t_color(0.5, 0.5, 0.5), t_color(0.7, 0.7, 0.7)),
				NULL, NULL);
			gui_part_end.add(new_desc);
			gui_part_end.add(new_value);
		}
		t_button *new_desc = new t_button(new s_text_button(
				"Spell used :", DARK_GREY,
				t_vect(9, 14.5) * gui_part_end.unit, t_vect(4, 1) * gui_part_end.unit, 5,
				t_color(0.5, 0.5, 0.5), t_color(0.7, 0.7, 0.7)),
				NULL, NULL);
		gui_part_end.add(new_desc);
		for (size_t i = 0; i < 6; i++)
		{
			if (actor->spell[i]->name != "NULL")
			{
				t_button *new_icon = new t_button(new s_tileset_button(actor->spell[i]->tile, actor->spell[i]->icon,
									t_vect(9 + (3.25 * (i % 3)), 15.75 + (1.1 * (i / 3))) * gui_part_end.unit, t_vect(1, 1) * gui_part_end.unit, 4),
					NULL, NULL);
				t_button *new_text = new t_button(new s_text_button(
					"x " + to_string(actor->spell_used[i]), DARK_GREY,
					t_vect(9 + 1.2 + (3.25 * (i % 3)), 15.75 + (1.1 * (i / 3))) * gui_part_end.unit, t_vect(1, 1) * gui_part_end.unit, 5,
					t_color(0.5, 0.5, 0.5), t_color(0.7, 0.7, 0.7)),
					NULL, NULL);
				gui_part_end.add(new_icon);
				gui_part_end.add(new_text);
			}
		}
	}
}

void				quit_end_menu(t_data data)
{
	bool *play = (bool *)(data.data[0]);
	int *exp_bonus = (int *)(data.data[1]);
	int *gold_bonus = (int *)(data.data[2]);

	account->add_exp(*exp_bonus);
	account->add_gold(*gold_bonus);
	*play = false;
	save_game_engine();
	account->tuto_state++;
}

static void			give_away_end_tutorial(t_data data)
{
	account->map_unlock.clear();
	account->map_unlock.push_back("road-00");
	account->add_gold(100);
	quit_end_menu(data);
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

	t_vect size = t_vect(3, 4);
	for (size_t i = 0; i < 6; i++)
	{
		t_vect coord = t_vect(2 + (size.x + 0.5) * (i % 2), 4.5 + (size.y + 0.5) * (i / 2));
		t_button *player_button = new t_button(new s_text_button(
				"", DARK_GREY,
				coord * gui.unit, size * gui.unit, 5,
				t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
				select_player_info, t_data(1, account->actor_array[i]));
		t_button *name_button = new t_button(new s_text_button(
				"", DARK_GREY,
				(coord + t_vect(0.2, 0.2)) * gui.unit, t_vect(size.x - 0.4, 0.8) * gui.unit, 3,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
				NULL, NULL);
		t_button *frame_button = new t_button(new s_text_button(
				"", DARK_GREY,
				(coord + t_vect(0.2, 1.2)) * gui.unit, t_vect(size.x - 0.4, size.x - 0.4) * gui.unit, 3,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
				NULL, NULL);
		t_actor_card *actor_card = new s_actor_card(player_button, &(account->actor_array[i]), name_button, frame_button);
		gui.add(actor_card);
	}

	if (gui_tutorial[36].object_list.size() == 0)
	{
		gui_tutorial[36] = t_gui(30, 20);

		t_button *button = new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(0, 0), get_win_size(), 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			NULL, NULL);

		t_button *button2 = new t_button(new s_paragraph_button(
			"Here is the end game menu ! \n You will find in this menu some useful informations about the game you just played, your character and the way they have handle this.", DARK_GREY, gui_tutorial[0].unit.y / 2, //text info
			t_vect(12.5, 4) * gui_tutorial[0].unit, t_vect(15.5, 6) * gui_tutorial[0].unit, 5, //object info
			t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL);

		gui_tutorial[36].add(TUTORIAL_NUM, new s_tutorial_button(button, button2, true));
	}

	if (gui_tutorial[37].object_list.size() == 0)
	{
		gui_tutorial[37] = t_gui(30, 20);

		t_button *button = new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(12.5, 2.5) * gui.unit, t_vect(15.5, 1) * gui.unit, 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			NULL, NULL);

		t_button *button2 = new t_button(new s_paragraph_button(
			"This part of the screen show you what you have earn in this map : Your new level/exp, and the amount of gold you have now. \n You can pass the animation of exp/gold giveaway by pressing Space or Echap once.", DARK_GREY, gui.unit.y / 2, //text info
			t_vect(12.5, 4) * gui_tutorial[0].unit, t_vect(15.5, 3) * gui_tutorial[0].unit, 5, //object info
			t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL);

		gui_tutorial[37].add(TUTORIAL_NUM, new s_tutorial_button(button, button2, true));
	}

	if (gui_tutorial[38].object_list.size() == 0)
	{
		gui_tutorial[38] = t_gui(30, 20);

		t_button *button = new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(2, 4.5) * gui.unit, t_vect(6.5, 13) * gui.unit, 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			NULL, NULL);

		t_button *button2 = new t_button(new s_paragraph_button(
			"There you can find all of your possible 6 characters. If you click on one of them, you will see a bunch of information about him.", DARK_GREY, gui.unit.y / 2, //text info
			t_vect(12.5, 4) * gui_tutorial[0].unit, t_vect(15.5, 3) * gui_tutorial[0].unit, 5, //object info
			t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL);

		gui_tutorial[38].add(TUTORIAL_NUM, new s_tutorial_button(button, button2, true));
	}

	if (gui_tutorial[39].object_list.size() == 0)
	{
		gui_tutorial[39] = t_gui(30, 20);

		t_button *button = new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(12.5, 4) * gui_tutorial[0].unit, t_vect(15.5, 3) * gui.unit, 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			NULL, NULL);

		t_button *button2 = new t_button(new s_paragraph_button(
			"And this is the end of our tutorial ! \n \n I hope that you will enjoy our game. \n As a gift, we offer you 100 gold, and one map : road-00 ! \n \n Click here to get back to main menu", DARK_GREY, gui.unit.y / 2, //text info
			t_vect(12.5, 4) * gui_tutorial[0].unit, t_vect(15.5, 4) * gui_tutorial[0].unit, 5, //object info
			t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL);

		gui_tutorial[39].add(TUTORIAL_NUM, new s_tutorial_button(button, button2, true));
	}

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
		if (frame == 2)
		{
			frame = 0;
		}

		prepare_screen(t_color(0.2, 0.2, 0.2));

		gui.draw_self();
		gui_part_end.draw_self();

		if (account->tuto_state < TUTO_SIZE)
			gui_tutorial[account->tuto_state].draw_self();

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
			else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE && gold_bonus == 0 && exp_bonus == 0 && account->tuto_state >= TUTO_SIZE)
				quit_end_menu(t_data(3, &play, &exp_bonus, &gold_bonus));
			else if (event.type == SDL_KEYUP && (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_RETURN) && (gold_bonus != 0 || exp_bonus != 0))
			{
				account->add_exp(exp_bonus);
				account->add_gold(gold_bonus);
				exp_bonus = 0;
				gold_bonus = 0;
			}
			else if (event.type == SDL_MOUSEBUTTONUP && account->tuto_state < TUTO_SIZE)
				gui_tutorial[account->tuto_state].click(&event);
			else if (event.type == SDL_MOUSEBUTTONUP)
				gui.click(&event);
		}
		if (account->tuto_state == 40)
			give_away_end_tutorial(t_data(3, &play, &exp_bonus, &gold_bonus));
	}
}

void				s_game_board::end_game_lose()
{
	int exp_bonus = 0;
	int	gold_bonus = 0;
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
		quit_end_menu, t_data(3, &play, &exp_bonus, &gold_bonus));

	gui.add(background);
	gui.add(victory_message);
	gui.add(continue_button);

	t_vect size = t_vect(3, 4);
	for (size_t i = 0; i < 6; i++)
	{
		t_vect coord = t_vect(2 + (size.x + 0.5) * (i % 2), 4.5 + (size.y + 0.5) * (i / 2));
		t_button *player_button = new t_button(new s_text_button(
				"", DARK_GREY,
				coord * gui.unit, size * gui.unit, 5,
				t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
				select_player_info, t_data(1, account->actor_array[i]));
		t_button *name_button = new t_button(new s_text_button(
				"", DARK_GREY,
				(coord + t_vect(0.2, 0.2)) * gui.unit, t_vect(size.x - 0.4, 0.8) * gui.unit, 3,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
				NULL, NULL);
		t_button *frame_button = new t_button(new s_text_button(
				"", DARK_GREY,
				(coord + t_vect(0.2, 1.2)) * gui.unit, t_vect(size.x - 0.4, size.x - 0.4) * gui.unit, 3,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
				NULL, NULL);
		t_actor_card *actor_card = new s_actor_card(player_button, &(account->actor_array[i]), name_button, frame_button);
		gui.add(actor_card);
	}

	if (gui_tutorial[36].object_list.size() == 0)
	{
		gui_tutorial[36] = t_gui(30, 20);

		t_button *button = new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(0, 0), get_win_size(), 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			NULL, NULL);

		t_button *button2 = new t_button(new s_paragraph_button(
			"Here is the end game menu ! \n You will find in this menu some useful informations about the game you just played, your character and the way they have handle this.", DARK_GREY, gui_tutorial[0].unit.y / 2, //text info
			t_vect(12.5, 4) * gui_tutorial[0].unit, t_vect(15.5, 6) * gui_tutorial[0].unit, 5, //object info
			t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL);

		gui_tutorial[36].add(TUTORIAL_NUM, new s_tutorial_button(button, button2, true));
	}

	if (gui_tutorial[37].object_list.size() == 0)
	{
		gui_tutorial[37] = t_gui(30, 20);

		t_button *button = new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(12.5, 2.5) * gui.unit, t_vect(15.5, 1) * gui.unit, 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			NULL, NULL);

		t_button *button2 = new t_button(new s_paragraph_button(
			"This part of the screen show you what you have earn in this map : Your new level/exp, and the amount of gold you have now. \n You can pass the animation of exp/gold giveaway by pressing Space or Echap once.", DARK_GREY, gui.unit.y / 2, //text info
			t_vect(12.5, 4) * gui_tutorial[0].unit, t_vect(15.5, 3) * gui_tutorial[0].unit, 5, //object info
			t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL);

		gui_tutorial[37].add(TUTORIAL_NUM, new s_tutorial_button(button, button2, true));
	}

	if (gui_tutorial[38].object_list.size() == 0)
	{
		gui_tutorial[38] = t_gui(30, 20);

		t_button *button = new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(2, 4.5) * gui.unit, t_vect(6.5, 13) * gui.unit, 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			NULL, NULL);

		t_button *button2 = new t_button(new s_paragraph_button(
			"There you can find all of your possible 6 characters. If you click on one of them, you will see a bunch of information about him.", DARK_GREY, gui.unit.y / 2, //text info
			t_vect(12.5, 4) * gui_tutorial[0].unit, t_vect(15.5, 3) * gui_tutorial[0].unit, 5, //object info
			t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL);

		gui_tutorial[38].add(TUTORIAL_NUM, new s_tutorial_button(button, button2, true));
	}

	if (gui_tutorial[39].object_list.size() == 0)
	{
		gui_tutorial[39] = t_gui(30, 20);

		t_button *button = new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(12.5, 4) * gui_tutorial[0].unit, t_vect(15.5, 3) * gui.unit, 5,
			t_color(0.0, 0.0, 0.0, 0.0), t_color(0.5, 0.5, 0.5)),
			give_away_end_tutorial, t_data(3, &play, &exp_bonus, &gold_bonus));

		t_button *button2 = new t_button(new s_paragraph_button(
			"And this is the end of our tutorial ! \n \n I hope that you will enjoy our game. \n As a gift, we offer you 100 gold, and one map : road-00 ! \n \n Click here to get back to main menu", DARK_GREY, gui.unit.y / 2, //text info
			t_vect(12.5, 4) * gui_tutorial[0].unit, t_vect(15.5, 4) * gui_tutorial[0].unit, 5, //object info
			t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL);

		gui_tutorial[39].add(TUTORIAL_NUM, new s_tutorial_button(button, button2, true));
	}

	while (play == true)
	{
		prepare_screen(t_color(0.2, 0.2, 0.2));

		gui.draw_self();
		gui_part_end.draw_self();

		if (account->tuto_state < TUTO_SIZE)
			gui_tutorial[account->tuto_state].draw_self();

		render_screen(true);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE && account->tuto_state >= TUTO_SIZE))
				menu_quit(t_data(2, &gui, &play));
			else if (event.type == SDL_MOUSEBUTTONUP && account->tuto_state < TUTO_SIZE)
				gui_tutorial[account->tuto_state].click(&event);
			else if (event.type == SDL_MOUSEBUTTONUP)
				gui.click(&event);
		}
	}
}
