#include "taag.h"

/*
TO_DO :

erase the increment function into iterator to allow a recalculation of every text in the screen when + or -
*/
extern vector<size_t> nb_param_gambit;

vector<vector<string>> gambit_text_desc = {
	{"Approach enemy", "Range to reach :", "Range delta :", "Range type :", "Max mouvement :"},
	{"Approach weak enemy", "Range to reach :", "Range delta :", "Range type :", "Max mouvement :"},
	{"Approach weak % enemy", "Range to reach :", "Range delta :", "Range type :", "Max mouvement :"},
	{"Flee enemy", "Hp percent to start :", "Max mouvement"},
	{"Support ally", "Range to reach :", "Range delta :", "Range type :", "Max mouvement :"},
	{"Support weak % ally", "Range to reach :", "Range delta :", "Range type :", "Max mouvement :"},
	{"Support damaged ally", "Ally percent to trigger :", "Range to reach :", "Range delta :", "Range type :", "Max mouvement :"},
	{"Attack enemy", "Spell num to cast:", "value PA (-1 for disable) : ", "value PM (-1 for disable) : "},
	{"Attack weak enemy", "Spell num to cast:", "value PA (-1 for disable) : ", "value PM (-1 for disable) : "},
	{"Attack weak % enemy", "Spell num to cast:", "value PA (-1 for disable) : ", "value PM (-1 for disable) : "},
	{"Help ally", "Spell num to cast:", "value PA (-1 for disable) : ", "value PM (-1 for disable) : "},
	{"Help weak ally", "Spell num to cast:", "value PA (-1 for disable) : ", "value PM (-1 for disable) : "},
	{"Help weak % ally", "Spell num to cast:", "Ally percent to trigger :", "value PA (-1 for disable) : ", "value PM (-1 for disable) : "},
	{"XXXX", "", ""},
};

vector<vector<int>> gambit_starting_value = {
	{0, 1, 0, 1, -1},
	{0, 1, 0, 1, -1},
	{0, 1, 0, 1, -1},
	{0, 25, -1},
	{0, 1, 0, 1, -1},
	{0, 1, 0, 1, -1},
	{0, 50, 1, 0, 1, -1},
	{0, 0, -1, -1},
	{0, 0, -1, -1},
	{0, 0, -1, -1},
	{0, 0, -1, -1},
	{0, 0, -1, -1},
	{0, 0, 50, -1, -1},
	{0, 0, 0},
};

vector<vector<int>> gambit_increment_value = {
	{0, 1, 1, 1, 1},
	{0, 1, 1, 1, 1},
	{0, 1, 1, 1, 1},
	{0, 5, 1},
	{0, 1, 1, 1, 1},
	{0, 1, 1, 1, 1},
	{0, 5, 1, 1, 1, 1},
	{0, 1, 1, 1},
	{0, 1, 1, 1},
	{0, 1, 1, 1},
	{0, 1, 1, 1},
	{0, 1, 1, 1},
	{0, 1, 5, 1, 1},
	{0, 0, 0},
};

vector<vector<int>> gambit_min_value = {
	{0, 1, 0, 0, -1},
	{0, 1, 0, 0, -1},
	{0, 1, 0, 0, -1},
	{0, 5, -1},
	{0, 1, 0, 0, -1},
	{0, 1, 0, 0, -1},
	{0, 5, 0, 0, -1},
	{0, 0, -1, -1},
	{0, 0, -1, -1},
	{0, 0, -1, -1},
	{0, 0, -1, -1},
	{0, 0, -1, -1},
	{0, 0, 5, -1, -1},
	{0, 0, 0},
};

vector<vector<int>> gambit_max_value = {
	{0, 10, 10, 1, 10},
	{0, 10, 10, 1, 10},
	{0, 10, 10, 1, 10},
	{0, 100, 10},
	{0, 10, 10, 1, 10},
	{0, 10, 10, 1, 10},
	{0, 100, 10, 10, 1, 10},
	{0, 5, 10, 10},
	{0, 5, 10, 10},
	{0, 5, 10, 10},
	{0, 5, 10, 10},
	{0, 5, 10, 10},
	{0, 5, 100, 10, 10},
	{0, 0, 0},
};

t_button *button_num_list[16];
t_button *button_desc_list[16];
t_button *gambit_desc;
t_iterator *gambit_selector;
t_iterator *value_iterator[10];

static void		calc_left_panel(t_actor *actor, size_t index)
{
	for (size_t j = 0; j < 16; j++)
	{
		button_num_list[j]->button->text = ((j + index) < actor->gambit.size() ? "[" + to_string(j + index) + "]" : "");
		button_desc_list[j]->button->text = ((j + index) < actor->gambit.size() ? parse_gambit((actor->gambit)[j + index]) : "");
	}
}

static void		increment_index(t_data data)//actor, &index, delta
{
	t_actor			*actor = (t_actor *)(data.data[0]);
	size_t			*index = (size_t *)(data.data[1]);
	int				delta = (int &)(data.data[2]);

	if ((int)(*index) + delta >= 0 && ((int)(*index) + delta) < (int)(actor->gambit.size()))
		*index += delta;
	calc_left_panel(actor, *index);
}

static void		increment_value_iterator(t_data data)
{
	t_actor *actor = (t_actor *)(data.data[6]);
	size_t	p_value = (size_t &)(data.data[7]);
	size_t		selected = (size_t &)(data.data[8]);
	increment_iterator(t_data(6, data.data[0], data.data[1], data.data[2], data.data[3], data.data[4], data.data[5]));
	button_desc_list[selected]->button->text = parse_gambit((actor->gambit)[p_value]);
}

static void		calc_right_panel(t_actor *actor, size_t p_value, t_gui *gui_right, size_t selected)
{
	*gui_right = t_gui();
	gui_right->add(gambit_desc);
	gui_right->add(gambit_selector);
	if ((actor->gambit)[p_value].value[0] == -1)
		return ;
	int i = 1;
	for (size_t j = 1; j < gambit_text_desc[(actor->gambit)[p_value].value[0]].size(); j++)
	{
		value_iterator[j] = new s_iterator(&((actor->gambit)[p_value].value[j]), NULL, gambit_increment_value[(actor->gambit)[p_value].value[0]][j], 1, gambit_min_value[(actor->gambit)[p_value].value[0]][j], gambit_max_value[(actor->gambit)[p_value].value[0]][j],
			new t_button(new s_text_button(
					gambit_text_desc[(actor->gambit)[p_value].value[0]][j], DARK_GREY,
					t_vect(15.75, 1.5 + (1.2 * (i))) * gui_right->unit, t_vect(9, 1) * gui_right->unit, 5,
					t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
			new t_button(new s_text_button(
					"-", DARK_GREY,
					t_vect(25, 1.5 + (1.2 * (i))) * gui_right->unit, t_vect(1, 1) * gui_right->unit, 5,
					t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
			new t_button(new s_text_button(
					"", DARK_GREY,
					t_vect(26.25, 1.5 + (1.2 * (i))) * gui_right->unit, t_vect(1, 1) * gui_right->unit, 5,
					t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
			new t_button(new s_text_button(
					"+", DARK_GREY,
					t_vect(27.5, 1.5 + (1.2 * (i))) * gui_right->unit, t_vect(1, 1) * gui_right->unit, 5,
					t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));

		value_iterator[j]->plus->button->funct_left = increment_value_iterator;
		value_iterator[j]->minus->button->funct_left = increment_value_iterator;
		value_iterator[j]->plus->button->data_left = t_data(9, &((actor->gambit)[p_value].value[j]), NULL, gambit_increment_value[(actor->gambit)[p_value].value[0]][j], 1, gambit_min_value[(actor->gambit)[p_value].value[0]][j], gambit_max_value[(actor->gambit)[p_value].value[0]][j], actor, p_value, selected);
		value_iterator[j]->minus->button->data_left = t_data(9, &((actor->gambit)[p_value].value[j]), NULL, -gambit_increment_value[(actor->gambit)[p_value].value[0]][j], -1, gambit_min_value[(actor->gambit)[p_value].value[0]][j], gambit_max_value[(actor->gambit)[p_value].value[0]][j], actor, p_value, selected);
		i++;
		gui_right->add(value_iterator[j]);
	}
}

static void		increment_type_iterator(t_data data)
{

	increment_iterator(t_data(6, data.data[0], data.data[1], data.data[2], data.data[3], data.data[4], data.data[5]));

	t_actor *actor = (t_actor *)(data.data[6]);
	size_t	p_value = (size_t &)(data.data[7]);
	t_gui	*gui_right = (t_gui *)(data.data[8]);
	size_t		selected = (size_t &)(data.data[9]);

	if (actor->gambit[p_value].value[0] != -1)
	{
		actor->gambit[p_value] = gambit_starting_value[actor->gambit[p_value].value[0]];
		button_desc_list[selected]->button->text = parse_gambit((actor->gambit)[p_value]);
		gambit_desc->button->text = gambit_text_desc[(actor->gambit)[p_value].value[0]][0];
		calc_right_panel(actor, p_value, gui_right, selected);
	}
	else
	{
		button_desc_list[selected]->button->text = parse_gambit((actor->gambit)[p_value]);
		gambit_desc->button->text = "Empty";
		*gui_right = t_gui();
		gui_right->add(gambit_desc);
		gui_right->add(gambit_selector);
	}
}

static void select_gambit_editable(t_data data) //&gui, &actor, &index, selected
{
	t_gui		*gui_right = (t_gui *)(data.data[0]);
	t_actor		*actor = (t_actor *)(data.data[1]);
	size_t		*index = (size_t *)(data.data[2]);
	size_t		selected = (size_t &)(data.data[3]);
	size_t		value = *index + selected;

	*gui_right = t_gui();

	if (value >= (actor->gambit).size())
		return ;

	/*if ((actor->gambit)[value].value.size() == 0 || (actor->gambit)[value].value.size() != gambit_text_desc[(actor->gambit)[value].value[0]].size())
		(actor->gambit)[value] = t_ai_helper({0, 1, 0, 0, -1});*/
	int			*action_type = &((actor->gambit)[value].value[0]);

	if (value < actor->gambit.size())
	{
		int i = 0;
		gambit_desc = new s_button(new s_text_button(
			((actor->gambit)[value].value[0] != -1 ? gambit_text_desc[(actor->gambit)[value].value[0]][0] : "Empty"), DARK_GREY,
			gui_right->unit * t_vect(17, 1.5), gui_right->unit * t_vect(10.25, 1), 4,
			t_color(0.5, 0.5, 0.5), t_color(0.7, 0.7, 0.7)),
			NULL, NULL);
		gambit_selector = new s_iterator(&((actor->gambit)[value].value[0]), NULL, 1, 0, -1, gambit_text_desc.size() - 1,
			NULL,
			new t_button(new s_text_button(
					"<", DARK_GREY,
					t_vect(15.75, 1.5 + (1.2 * (i))) * gui_right->unit, t_vect(1, 1) * gui_right->unit, 5,
					t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
			NULL,
			new t_button(new s_text_button(
					">", DARK_GREY,
					t_vect(27.5, 1.5 + (1.2 * (i))) * gui_right->unit, t_vect(1, 1) * gui_right->unit, 5,
					t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
			i++;
		gambit_selector->plus->button->funct_left = increment_type_iterator;
		gambit_selector->minus->button->funct_left = increment_type_iterator;
		gambit_selector->plus->button->data_left = t_data(10, &((actor->gambit)[value].value[0]), NULL, 1, 0, -1, gambit_text_desc.size() - 1, actor, value, gui_right, selected);
		gambit_selector->minus->button->data_left = t_data(10, &((actor->gambit)[value].value[0]), NULL, -1, 0, -1, gambit_text_desc.size() - 1, actor, value, gui_right, selected);
		calc_right_panel(actor, value, gui_right, selected);
	}
}

static void create_choice_menu(t_data data)
{
	t_gui		*gui_left = (t_gui *)(data.data[0]);
	t_gui		*gui_right = (t_gui *)(data.data[1]);
	t_actor		*actor = (t_actor *)(data.data[2]);
	bool		*back_play = (bool *)(data.data[3]);
	size_t		*index = (size_t *)(data.data[4]);
	size_t		selected = (size_t &)(data.data[5]);
	size_t		value = *index + selected;
	t_gui		gui = t_gui();
	SDL_Event	event;
	bool		play = true;

	t_vect		mouse = get_mouse_coord();

	t_button	*back_button = new s_button(new s_text_button(
		"", DARK_GREY,
		mouse + (gui.unit * t_vect(0, -0.75)), gui.unit * t_vect(4, 1.45), 4,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		NULL, NULL);
	gui.add(back_button);

	t_button	*add_button = new s_button(new s_text_button(
		"Insert a new gambit", DARK_GREY,
		mouse + (gui.unit * t_vect(0.1, -0.65)), gui.unit * t_vect(3.8, 0.6), 3,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		menu_add_gambit, t_data(2, actor, value));
	gui.add(add_button);

	t_button	*remove_button = new s_button(new s_text_button(
		"Remove this gambit", DARK_GREY,
		mouse + (gui.unit * t_vect(0.1, 0)), gui.unit * t_vect(3.8, 0.6), 3,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		menu_remove_gambit, t_data(3, actor, value, gui_right));
	gui.add(remove_button);

	while (play == true)
	{
		prepare_screen();

		gui_left->draw_self();
		gui_right->draw_self();
		gui.draw_self();

		render_screen(true);

		if (SDL_PollEvent(&(event)) == 1)
		{
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
			{
				*back_play = false;
				play = false;
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				gui.click(&event);
				play = false;
			}
		}
	}
	calc_left_panel(actor, *index);
	calc_left_panel(actor, *index);
}

void menu_gambit_editor(t_data data)
{
	t_actor		*actor = (t_actor *)(data.data[1]);
	t_gui		gui_left = t_gui();
	t_gui		gui_right = t_gui();
	SDL_Event	event;
	bool		play = true;
	size_t 		index = 0;

	s_button *button = new s_button(new s_text_button(
		"", DARK_GREY,
		gui_left.unit * t_vect(0.2, 0.2), gui_left.unit * t_vect(29.6, 19.6), 8,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		NULL, NULL);
	gui_left.add(button);

	s_button *left_part = new s_button(new s_text_button(
		"", DARK_GREY,
		gui_left.unit * t_vect(1, 1), gui_left.unit * t_vect(13.75, 18), 8,
		t_color(0.5, 0.5, 0.5), t_color(0.7, 0.7, 0.7)),
		NULL, NULL);
	gui_left.add(left_part);

	s_button *right_part = new s_button(new s_text_button(
		"", DARK_GREY,
		gui_left.unit * t_vect(15.25, 1), gui_left.unit * t_vect(13.75, 18), 8,
		t_color(0.5, 0.5, 0.5), t_color(0.7, 0.7, 0.7)),
		NULL, NULL);
	gui_left.add(right_part);

	s_button *button_up = new s_button(new s_text_button(
		"UP", DARK_GREY,
		gui_left.unit * t_vect(6.375, 0.5), gui_left.unit * t_vect(3, 1), 4,
		t_color(0.5, 0.5, 0.5), t_color(0.7, 0.7, 0.7)),
		increment_index, t_data(3, actor, &index, -1));//actor, &index, delta
	gui_left.add(button_up);

	s_button *button_down = new s_button(new s_text_button(
		"DOWN", DARK_GREY,
		gui_left.unit * t_vect(6.375, 18.5), gui_left.unit * t_vect(3, 1), 4,
		t_color(0.5, 0.5, 0.5), t_color(0.7, 0.7, 0.7)),
		increment_index, t_data(3, actor, &index, 1));//actor, &index, delta
	gui_left.add(button_down);

	double i = 0.15;
	for (size_t j = 0; j < 16; j++)
	{
		button_num_list[j] = new t_button(new s_text_button(
				((j + index) < actor->gambit.size() ? "[" + to_string(j + index) + "]" : ""), DARK_GREY,
				t_vect(1.5, 1.5 + (1.1 * i)) * gui_left.unit, t_vect(1, 0.95) * gui_left.unit, 3,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
				NULL, NULL);
		button_desc_list[j] = new t_button(new s_text_button(
				((j + index) < actor->gambit.size() ? parse_gambit((actor->gambit)[j + index]) : ""), DARK_GREY,
				t_vect(2.75, 1.5 + (1.1 * i)) * gui_left.unit, t_vect(11.5, 0.95) * gui_left.unit, 3,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
				select_gambit_editable, t_data(4, &gui_right, actor, &index, j)); //&gui, &actor, &index, selected
		button_desc_list[j]->button->funct_right = create_choice_menu;
		button_desc_list[j]->button->data_right = t_data(6, &gui_left, &gui_right, actor, &play, &index, j);
		i += 0.95;
		gui_left.add(button_num_list[j]);
		gui_left.add(button_desc_list[j]);
	}

	while (play == true)
	{
		prepare_screen();

		if (data.data.size() != 0)
			(*((t_gui *)(data.data[0]))).draw_self();
		gui_left.draw_self();
		gui_right.draw_self();

		render_screen(true);

		if (SDL_PollEvent(&(event)) == 1)
		{
			if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
				play = false;
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				gui_left.click(&event);
				gui_right.click(&event);
			}
			else if (event.type == SDL_MOUSEWHEEL && event.wheel.y < 0)
				increment_index(t_data(3, actor, &index, 1));
			else if (event.type == SDL_MOUSEWHEEL && event.wheel.y > 0)
				increment_index(t_data(3, actor, &index, -1));
		}
	}
}
