#include "taag.h"

/*
#define CHARGE			0	//run to the closest enemy at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define CHARGE_WEAK		1	//run to the enemy with less hp at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define CHARGE_PERCENT	2	//run to the enemy with less % hp at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define RETREAT			3	//retreat to the safest tile if stat hp < value[1] % max mouvement value[2]
#define SUPPORT			4	//run to closest ally at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define SUPPORT_PERCENT	5	//run to closest ally with less % hp at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define ATTACK			6	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on the closest enemy availible in range if possible
#define ATTACK_WEAK		7	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on the enemy with the less hp in range if possible
#define ATTACK_PERCENT	8	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on the enemy with the less % hp in range if possible
#define HELP			9	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on an ally
#define HELP_WEAK		10	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on the ally with less % HP
#define HELP_PERC		11	//cast the spell num value[2] (if value[2] PA and value[3] pm on caster) on the ally if HP % < value[1]
#define TURN			12	//if turn == value[1] --> execute command num value[2] with value stocked after
*/

extern vector<size_t> nb_param_gambit;

t_button *button_num_list[16];
t_button *button_desc_list[16];

static void		increment_index(t_data data)//actor, &index, delta
{
	t_actor			*actor = (t_actor *)(data.data[0]);
	size_t			*index = (size_t *)(data.data[1]);
	int				delta = (int &)(data.data[2]);

	if ((int)(*index) + delta >= 0 && ((int)(*index) + delta) < (int)(actor->gambit.size()))
		*index += delta;
	for (size_t j = 0; j < 16; j++)
	{
		button_num_list[j]->button->text = "[" + to_string(j + *index) + "]";
		button_desc_list[j]->button->text = ((j + *index) < actor->gambit.size() ? parse_gambit(actor->gambit[j + *index]) : "");
	}
}

static void select_gambit_editable(t_data data)
{
	t_gui		*gui_right = (t_actor *)(data.data[0]);
	t_actor		*actor = (t_actor *)(data.data[1]);
	size_t		*index = (size_t *)(data.data[2]);
	int			selected = (int &)(data.data[3]);

	*gui_right = t_gui();
}

void menu_gambit_editor(t_data data)
{
	t_actor		*actor = (t_actor *)(data.data[1]);
	t_gui		gui_left = t_gui();
	t_gui		gui_right = t_gui();
	SDL_Event	event;

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

	size_t index = 0;

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
				"[" + to_string(j + index) + "]", DARK_GREY,
				t_vect(1.5, 1.5 + (1.1 * i)) * gui_left.unit, t_vect(1, 0.95) * gui_left.unit, 3,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
				NULL, NULL);
		button_desc_list[j] = new t_button(new s_text_button(
				((j + index) < actor->gambit.size() ? parse_gambit(actor->gambit[j + index]) : ""), DARK_GREY,
				t_vect(2.75, 1.5 + (1.1 * i)) * gui_left.unit, t_vect(11.5, 0.95) * gui_left.unit, 3,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
				NULL, NULL);
		i += 0.95;
		gui_left.add(button_num_list[j]);
		gui_left.add(button_desc_list[j]);
	}

	bool		play = true;

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
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
			{
				gui_left.click();
			}
			else if (event.type == SDL_MOUSEWHEEL && event.wheel.y < 0)
				increment_index(t_data(3, actor, &index, 1));
			else if (event.type == SDL_MOUSEWHEEL && event.wheel.y > 0)
				increment_index(t_data(3, actor, &index, -1));
		}
	}
}
