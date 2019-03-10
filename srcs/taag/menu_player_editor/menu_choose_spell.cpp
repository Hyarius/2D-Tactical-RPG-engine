#include "taag.h"

t_spell		*spell_list[18];

static void	modify_index(t_data data)
{
	int *index = (int *)(data.data[0]);
	int delta = (int &)(data.data[1]);
	t_gui *gui = (t_gui *)(data.data[2]);

	if (*index + delta >= -12 && *index + delta < (int)(((account->spell_unlock.size() / 6) * 6 + 6)))
		*index += delta;
	for (int i = 0; i < 18; i++)
	{
		if (*index + i >= (int)(account->spell_unlock.size()) || *index + i < 0)
			spell_list[i] = &(spell_map["NULL"]);
		else
			spell_list[i] = &(spell_map[account->spell_unlock[(*index + i) % account->spell_unlock.size()]]);
	}
}

static void	quit_select_spell(t_data data)// &actor | num spell | i | &play
{
	t_actor		*player = (t_actor *)(data.data[0]);
	int			spell_num = (int &)(data.data[1]);
	int			i = (int &)(data.data[2]);
	bool		*play = (bool *)(data.data[3]);

	player->spell[spell_num] = spell_list[i];
	*play = false;
}

void		menu_choose_spell(t_data data)
{
	t_gui		*old_gui = (t_gui *)(data.data[0]);
	int			spell_num = (int &)(data.data[1]);
	t_actor		*player = (t_actor *)(data.data[2]);
	t_gui		gui;
	bool		play = true;
	SDL_Event	event;


	gui.add(new t_button(new t_text_button(
						"", BLACK,
						gui.unit * t_vect(0.3, 0.3), gui.unit * t_vect(29.4, 19.4), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						NULL, NULL));
	int index = 0;

	for (int i = 0; i < 18; i++)
	{
		if (index + i >= (int)(account->spell_unlock.size()) || index + i < 0)
			spell_list[i] = &(spell_map["NULL"]);
		else
			spell_list[i] = &(spell_map[account->spell_unlock[(index + i) % account->spell_unlock.size()]]);
		gui.add(new t_spell_card(&(spell_list[i]), gui.unit * t_vect(0.5 + (i % 6) * 4 + (0.2 * (i % 6)), (0.5 + (i / 6) * 6 + (0.2 * (i / 6)))), gui.unit * t_vect(4, 6), quit_select_spell, t_data(4, player, spell_num, i, &play)));// &actor | num spell | i | &play
	}

	gui.add(new t_button(new t_text_button(
						"up", BLACK,
						gui.unit * t_vect(26, 1), gui.unit * t_vect(3, 5), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						modify_index, t_data(3, &index, -6, &gui)));

	gui.add(new t_button(new t_text_button(
						"down", BLACK,
						gui.unit * t_vect(26, 14), gui.unit * t_vect(3, 5), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						modify_index, t_data(3, &index, 6, &gui)));

	if (gui_tutorial[9].object_list.size() == 0)
	{
		gui_tutorial[9] = t_gui(30, 20);

		gui_tutorial[9].add(TUTORIAL_NUM, new s_tutorial_button(NULL,
			new t_button(new s_paragraph_button(
				"Welcome to the spell selector ! Here, you will find every spell that you have access to. \n Let's look at them more closely !", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(9, 0.5) * gui_tutorial[0].unit, t_vect(12.4, 3) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL), true));
	}
	if (gui_tutorial[10].object_list.size() == 0)
	{
		gui_tutorial[10].add(TUTORIAL_NUM, new s_tutorial_button(new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(0.5, 0.5) * gui.unit, t_vect(4, 6) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
			NULL, NULL),
			new t_button(new s_paragraph_button(
				"This spell, obviously named " + spell_list[0]->name  + ", has some basic informations. \n - It's Action Points cost : " + to_string(spell_list[0]->cost_pa) + " PA per attack. \n - It's Mouvement Points cost. In this case it cost 0 MP per attack, but sometimes it will be more ! \n - The range of the attack. It describes the range from which you can cast this spell. The first value describe the minimum distance where to hit an enemy, and the other one describe the maximun range allowed. \n - The description of the spell and it's effect. In this case, it will deal damages to the target and will pull it to you by one square. \n \n And, at the bottom of the spell, you can find 4 icons. \n - The first on the left represent what kind of target you can hit with this spell. Some spell must target an occupied area, others should focus an empty tile, and some can target both. \n - The second one tells you if obstacles prevent the casting of the spell or if you can ignore them. \n - The third one describe the type of range allowed for this spell : in circle around you, or in a straight line. \n - And the last one represent the type of zone this spell will hit : a solo-area, a circle, a square, or a line.", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(9, 0.5) * gui_tutorial[0].unit, t_vect(12.4, 13.2) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), NULL, NULL), true));
	}
	if (gui_tutorial[11].object_list.size() == 0)
	{
		gui_tutorial[11].add(TUTORIAL_NUM, new s_tutorial_button(new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(0.5, 0.5) * gui.unit, t_vect(4, 6) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
			quit_select_spell, t_data(4, player, spell_num, 0, &play)),
			new t_button(new s_paragraph_button(
				"Let's select this spell ! Click on it !", DARK_GREY, gui.unit.y / 2, //text info
				t_vect(9, 0.5) * gui_tutorial[0].unit, t_vect(6.5, 2) * gui_tutorial[0].unit, 5, //object info
				t_color(222, 184, 135), t_color(245, 222, 179)), tmp_function, NULL), true));
	}

	((s_tutorial_button *)(gui_tutorial[11].object_list[TUTORIAL_NUM][0]))->button->button->data_left = t_data(4, player, spell_num, 0, &play);

	while (play == true)
	{
		prepare_screen();

		gui.draw_self();
		if ((size_t)(account->tuto_state) < gui_tutorial.size())
			gui_tutorial[account->tuto_state].draw_self();

		render_screen(true);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT)
				menu_quit(t_data(2, &gui, &play));
			if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
			{
				play = false;
				if (account->tuto_state < TUTO_SIZE)
					account->tuto_state = 8;
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				if (account->tuto_state < TUTO_SIZE && gui_tutorial[account->tuto_state].object_list.size())
					gui_tutorial[account->tuto_state].click(&event);
				else
					gui.click(&event);
			}
			else if (account->tuto_state >= TUTO_SIZE && (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN))
				gui.key_press(&event);
			else if (account->tuto_state >= TUTO_SIZE && event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_UP)
			{
				modify_index(t_data(3, &index, -6, &gui));
			}
			else if (account->tuto_state >= TUTO_SIZE && event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_DOWN)
			{
				modify_index(t_data(3, &index, 6, &gui));
			}
			else if (account->tuto_state >= TUTO_SIZE && event.type == SDL_MOUSEWHEEL)
			{
				if (event.wheel.y > 0)
					modify_index(t_data(3, &index, -6, &gui));
				else if (event.wheel.y < 0)
					modify_index(t_data(3, &index, 6, &gui));
			}
		}
	}
	(void)data;
}
