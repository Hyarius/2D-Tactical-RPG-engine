#include "taag.h"

string 				*text_list_action_name[30];

static void	modify_index(t_data data)
{
	int *index = (int *)(data.data[0]);
	int delta = (int &)(data.data[1]);

	if (*index + delta >= -27 && *index + delta < (int)(((list_action_name.size() / 3) * 3 + 3)))
		*index += delta;
	for (int i = 0; i < 30; i++)
	{
		if (*index + i >= (int)(list_action_name.size()) || *index + i < 0)
			*(text_list_action_name[i]) = "";
		else
			*(text_list_action_name[i]) = list_action_name[i + *index];
	}
}

static void		quit_choose(t_data data)// string * / bool * / num
{
	string		*entry_name = (string *)(data.data[0]);
	bool 		*play = (bool *)(data.data[1]);
	int			i = (int &)(data.data[2]);

	if (*(text_list_action_name[i]) != "")
	{
		*entry_name = *(text_list_action_name[i]);
		*play = false;
	}
}

void			menu_choose_effect(t_data data)
{
	t_gui		*old_gui = (t_gui *)(data.data[0]);
	t_gui		*back_gui = (t_gui *)(data.data[1]);
	string		*str = (string *)(data.data[2]);
	t_gui		gui;
	bool		play = true;
	SDL_Event	event;
	int 		index = 0;

	gui.add(new t_button(new t_text_button(
		"", BLACK,
		gui.unit * t_vect(1, 1), gui.unit * t_vect(28, 18), 8,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
	),
		NULL, NULL));

	gui.add(new t_button(new t_text_button(
		"Which effect did you want", BLACK,
		gui.unit * t_vect(1.5, 1.5), gui.unit * t_vect(27, 2), 8,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
	),
		NULL, NULL));
	int i = 0;
	while (i < 30)
	{
		t_button *button = new t_button(new t_text_button(
			(i < (int)(list_action_name.size()) ? list_action_name[i] : ""), BLACK,
			gui.unit * t_vect(1.5 + ((i % 3) * 8.3 + (i % 3)), 4 + ((i / 3) * 1.3)),
			gui.unit * t_vect(8.3, 1.1),
			8,
			t_color(0.4, 0.4, 0.4),
			t_color(0.6, 0.6, 0.6)),
			quit_choose, t_data(3, str, &play, i));
		text_list_action_name[i] = &(button->button->text);
		gui.add(button);
		i++;
	}

	while (play == true)
	{
		prepare_screen();

		//if (old_gui != NULL)
		//	old_gui->draw_self();
		//if (back_gui != NULL)
		//	back_gui->draw_self();

		gui.draw_self();

		render_screen(true);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT)
				menu_quit(t_data(1, &gui));
			if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				play = false;
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click(&event);
			else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
				gui.key_press(&event);
			else if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_UP))
			{
				modify_index(t_data(2, &index, -3));
			}
			else if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_DOWN))
			{
				modify_index(t_data(2, &index, 3));
			}
			else if (event.type == SDL_MOUSEWHEEL)
			{
				if (event.wheel.y > 0)
					modify_index(t_data(2, &index, -3));
				else if (event.wheel.y < 0)
					modify_index(t_data(2, &index, 3));
			}
		}
	}
}

static void quit_action_select(t_data data)
{
	bool *play = (bool *)(data.data[0]);

	*play = false;
}

//save_effect, t_data(4, spell, entry, &(value[0][0]), &play));

int *value[6][4];
string *entry[6];

static string parse_effect_dmg(t_action *effect)
{
	string text = "deal " + to_string(effect->stat.value[0]) + " damage(s) to the target";
	return (text);
}
static string parse_effect_heal(t_action *effect)
{
	string text = "heal the target by " + to_string(effect->stat.value[0]) + " HP(s)";
	return (text);
}
static string parse_effect_change_pm(t_action *effect)
{
	string text;
	if (effect->stat.value[0] < 0)
		text = "reduce target's PM(s) by " + to_string(effect->stat.value[0]);
	else
		text = "give " + to_string(effect->stat.value[0]) + " PM(s) to the target" ;
	return (text);
}
static string parse_effect_change_pa(t_action *effect)
{
	string text;
	if (effect->stat.value[0] < 0)
		text = "reduce target's PA(s) by " + to_string(effect->stat.value[0]);
	else
		text = "give " + to_string(effect->stat.value[0]) + " PA(s) to the target";
	return (text);
}
static string parse_effect_heal_caster(t_action *effect)
{
	string text = "heal the caster by " + to_string(effect->stat.value[0]) + " HP(s)";
	if (effect->stat.value[3] != 1)
	{
		text.append(" per target hit");
		if (effect->stat.value[3] != 0)
			text.append(" up to " + to_string(effect->stat.value[3]) + " times max");
	}
	return (text);
}
static string parse_effect_dmg_caster(t_action *effect)
{
	string text = "deal " + to_string(effect->stat.value[0]) + " damage(s) to the caster";
	if (effect->stat.value[3] != 1)
	{
		text.append(" per target hit");
		if (effect->stat.value[3] != 0)
			text.append(" up to " + to_string(effect->stat.value[3]) + " times max");
	}
	return (text);
}
static string parse_effect_push_actor(t_action *effect)
{
	string text = "push the target by " + to_string(effect->stat.value[0]) + " square(s)";
	return (text);
}
static string parse_effect_pull_actor(t_action *effect)
{
	string text = "pull the target by " + to_string(effect->stat.value[0]) + " square(s)";
	return (text);
}
static string parse_effect_move_caster(t_action *effect)
{
	(void)effect;
	string text = "move the caster to the targeted location";
	return (text);
}
static string parse_effect_swap_actor(t_action *effect)
{
	(void)effect;
	string text = "swap caster and target location";
	return (text);
}
static string parse_effect_change_caster_pa(t_action *effect)
{
	string text;
	if (effect->stat.value[0] < 0)
		text = "reduce caster's PA(s) by " + to_string(effect->stat.value[0]);
	else
		text = "give " + to_string(effect->stat.value[0]) + " PA(s) to the caster";
	if (effect->stat.value[3] != 1)
	{
		text.append(" per target hit");
		if (effect->stat.value[3] != 0)
			text.append(" up to " + to_string(effect->stat.value[3]) + " times max");
	}
	return (text);
}
static string parse_effect_change_caster_pm(t_action *effect)
{
	string text;
	if (effect->stat.value[0] < 0)
		text = "reduce caster's PM(s) by " + to_string(effect->stat.value[0]);
	else
		text = "give " + to_string(effect->stat.value[0]) + " PM(s) to the caster";
	if (effect->stat.value[3] != 1)
	{
		text.append(" per target hit");
		if (effect->stat.value[3] != 0)
			text.append(" up to " + to_string(effect->stat.value[3]) + " times max");
	}
	return (text);
}
static string parse_effect_push_caster(t_action *effect)
{
	string text = "push the caster toward the target by " + to_string(effect->stat.value[0]) + " square(s)";
	return (text);
}
static string parse_effect_pull_caster(t_action *effect)
{
	string text = "push the caster away from the target by " + to_string(effect->stat.value[0]) + " square(s)";
	return (text);
}
static string parse_effect_apply_poison(t_action *effect)
{
	string text;
	if (effect->stat.value[0] == 0)
		text = "apply a effect that deal " + to_string(effect->stat.value[1]) + " damage(s) per turn for the next " + to_string(effect->stat.value[2]) + " turn(s)";
	else if (effect->stat.value[0] == 1)
		text = "apply a effect that deal " + to_string(effect->stat.value[1]) + " damage(s) on every action for the next " + to_string(effect->stat.value[2]) + " turn(s)";
	else if (effect->stat.value[0] == 2)
		text = "apply a effect that deal " + to_string(effect->stat.value[1]) + " damage(s) on every mouvement for the next " + to_string(effect->stat.value[2]) + " turn(s)";
	else if (effect->stat.value[0] == -1)
		text = "apply a effect that had no effect";
	return (text);
}
static string parse_effect_apply_regeneration(t_action *effect)
{
	string text;
	if (effect->stat.value[0] == 0)
		text = "apply a regeneration effect that heal " + to_string(effect->stat.value[1]) + " HP(s) per turn for the next " + to_string(effect->stat.value[2]) + " turn(s)";
	else if (effect->stat.value[0] == 1)
		text = "apply a regeneration effect that heal " + to_string(effect->stat.value[1]) + " HP(s) on every action for the next " + to_string(effect->stat.value[2]) + " turn(s)";
	else if (effect->stat.value[0] == 2)
		text = "apply a regeneration effect that heal " + to_string(effect->stat.value[1]) + " HP(s) on every mouvement for the next " + to_string(effect->stat.value[2]) + " turn(s)";
	else if (effect->stat.value[0] == -1)
		text = "apply a regeneration effect that had no effect";
	return (text);
}
static string parse_effect_apply_pa_change(t_action *effect)
{
	string type = effect->stat.value[1] < 0 ? "malus PA " : "bonus PA ";
	string text;
	if (effect->stat.value[0] == 0)
		text = "apply a " + type + "off " + to_string(effect->stat.value[1]) + " PA(s) per turn for the next " + to_string(effect->stat.value[2]) + " turn(s)";
	else if (effect->stat.value[0] == 1)
		text = "apply a " + type + "off " + to_string(effect->stat.value[1]) + " PA(s) on every action for the next " + to_string(effect->stat.value[2]) + " turn(s)";
	else if (effect->stat.value[0] == 2)
		text = "apply a " + type + "off " + to_string(effect->stat.value[1]) + " PA(s) on every mouvement for the next " + to_string(effect->stat.value[2]) + " turn(s)";
	else if (effect->stat.value[0] == -1)
		text = "apply a " + type + "that had no effect";
	return (text);
}
static string parse_effect_apply_pm_change(t_action *effect)
{
	string type = effect->stat.value[1] < 0 ? "malus PM " : "bonus PM ";
	string text;
	if (effect->stat.value[0] == 0)
		text = "apply a " + type + "off " + to_string(effect->stat.value[1]) + " PM(s) per turn for the next " + to_string(effect->stat.value[2]) + " turn(s)";
	else if (effect->stat.value[0] == 1)
		text = "apply a " + type + "off " + to_string(effect->stat.value[1]) + " PM(s) on every action for the next " + to_string(effect->stat.value[2]) + " turn(s)";
	else if (effect->stat.value[0] == 2)
		text = "apply a " + type + "off " + to_string(effect->stat.value[1]) + " PM(s) on every mouvement for the next " + to_string(effect->stat.value[2]) + " turn(s)";
	else if (effect->stat.value[0] == -1)
		text = "apply a " + type + "that had no effect";
	return (text);
}

static string parse_effect_cure_poison(t_action *effect)
{
	(void)effect;
	string text = "cure every poison effect of the target";
	return (text);
}

static string parse_effect_cure_regeneration(t_action *effect)
{
	(void)effect;
	string text = "cure every regeneration effect of the target";
	return (text);
}

static string parse_effect_cure_pa_malus(t_action *effect)
{
	(void)effect;
	string text = "cure every malus PA effect of the target";
	return (text);
}

static string parse_effect_cure_pm_malus(t_action *effect)
{
	(void)effect;
	string text = "cure every malus PM effect of the target";
	return (text);
}

static string parse_effect_cure_pa_bonus(t_action *effect)
{
	(void)effect;
	string text = "cure every bonus PA effect of the target";
	return (text);
}

static string parse_effect_cure_pm_bonus(t_action *effect)
{
	(void)effect;
	string text = "cure every bonus PM effect of the target";
	return (text);
}
static string parse_effect_add_armor(t_action *effect)
{
	string text = "add " + to_string(effect->stat.value[0]) + " armor point(s) to the target";
	return (text);
}
static string parse_effect_remove_armor(t_action *effect)
{
	string text = "remove " + to_string(effect->stat.value[0]) + " armor point(s) to the target";
	return (text);
}
static string parse_effect_true_dmg(t_action *effect)
{
	string text = "deal " + to_string(effect->stat.value[0]) + " true damage(s) to the target";
	return (text);
}
static string parse_effect_add_armor_caster(t_action *effect)
{
	string text = "add " + to_string(effect->stat.value[0]) + " armor point(s) to the caster";
	if (effect->stat.value[3] != 1)
	{
		text.append(" per target hit");
		if (effect->stat.value[3] != 0)
			text.append(" up to " + to_string(effect->stat.value[3]) + " times max");
	}
	return (text);
}
static string parse_effect_remove_armor_caster(t_action *effect)
{
	string text = "remove " + to_string(effect->stat.value[0]) + " armor point(s) to the caster";
	if (effect->stat.value[3] != 1)
	{
		text.append(" per target hit");
		if (effect->stat.value[3] != 0)
			text.append(" up to " + to_string(effect->stat.value[3]) + " times max");
	}
	return (text);
}

typedef string(*f_effect_parsor)(t_action *effect);

f_effect_parsor f_effect_list[] = {
	&parse_effect_dmg,
	&parse_effect_heal,
	&parse_effect_change_pm,
	&parse_effect_change_pa,
	&parse_effect_push_actor,
	&parse_effect_pull_actor,
	&parse_effect_heal_caster,
	&parse_effect_dmg_caster,
	&parse_effect_move_caster,
	&parse_effect_swap_actor,
	&parse_effect_change_caster_pm,
	&parse_effect_change_caster_pa,
	&parse_effect_push_caster,
	&parse_effect_pull_caster,
	&parse_effect_apply_poison,
	&parse_effect_apply_regeneration,
	&parse_effect_apply_pa_change,
	&parse_effect_apply_pm_change,
	&parse_effect_cure_poison,
	&parse_effect_cure_regeneration,
	&parse_effect_cure_pa_malus,
	&parse_effect_cure_pm_malus,
	&parse_effect_cure_pa_bonus,
	&parse_effect_cure_pm_bonus,
	&parse_effect_add_armor,
	&parse_effect_remove_armor,
	&parse_effect_true_dmg,
	&parse_effect_add_armor_caster,
	&parse_effect_remove_armor_caster,
};

static string compute_text(size_t size, int count, t_action *effect, int nb_effect)
{
	if (count == -1)
		return "";
	string text = (size != 0 ? (nb_effect == 1 ? " and " : ", ") : "");
	text += f_effect_list[count](effect);
	if (size == 0)
		text[0] = text[0] + ('A' - 'a');
	return (text);

}

static void save_effect(t_data data)
{
	t_spell *spell = (t_spell *)(data.data[0]);
	bool *play = (bool *)(data.data[1]);
	string **text = (string **)(data.data[2]);
	string desc;
	int nb_effect = 0;

	for (int i = 0; i < 6; i++)
	{
		if (*(entry[i]) != "Empty effect")
			nb_effect++;
	}

	for (int i = 0; i < 6; i++)
	{
		int count = 0;

		while (count < (int)(list_action_name.size()) && list_action_name[count] != *(entry[i]))
			count++;
		if (count == (int)(list_action_name.size()))
			count = -1;
		else
			count--;
		if (count != -1)
			spell->effect[i].effect = g_effects[count];
		*(text[i]) = list_action_name[count + 1];
		for (int j = 0; j < 4; j++)
		{
			spell->effect[i].stat.value[j] = *(value[i][j]);
		}
		desc.append(compute_text(desc.size(), count, &(spell->effect[i]), nb_effect - i));
	}

	spell->desc = desc;
	*play = false;
}

void menu_select_action(t_data data)
{
	t_gui		*old_gui = (t_gui *)(data.data[0]);
	t_spell		*spell = (t_spell *)(data.data[1]);
	string		**text = (string **)(data.data[2]);
	t_gui		gui;
	bool		play = true;
	SDL_Event	event;
	int 		index = 0;

	gui.add(new t_button(new t_text_button(
		"", BLACK,
		gui.unit * t_vect(0.5, 0.5), gui.unit * t_vect(28, 18), 8,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
	),
		NULL, NULL));

	t_vect coord = t_vect(0, 0);
	int num = 0;
	int i = 0;
	while (num < 6)
	{
		gui.add(new t_button(new t_text_button(
			"Effect " + to_string(num + 1) + " :", BLACK,
			gui.unit * (t_vect(1, 1.0 + (1.2 * (i))) + coord), gui.unit * t_vect(4, 1), 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
			NULL, NULL));

		t_button *button = new t_button(new t_text_button(
			*(text[num]), BLACK,
			gui.unit * (t_vect(5.5, 1.0 + (1.2 * (i))) + coord), gui.unit * t_vect(4, 1), 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
			menu_choose_effect, NULL);
		entry[num] = &(button->button->text);
		button->button->data_left = t_data(3, old_gui, &gui, &(button->button->text));
		i++;

		gui.add(button);

		int max_range = 30;

		for (int k = 0; k < 4; k++)
		{
			t_iterator *value_iterator = new s_iterator(spell->effect[num].stat.value[k], NULL, 1, 1, -max_range, max_range,
				new t_button(new s_text_button(
					"Effect value [" + to_string(k) + "] :", DARK_GREY,
					(coord + t_vect(1, 1.0 + (1.2 * (i)))) * gui.unit, t_vect(4, 1) * gui.unit, 5,
					t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
				new t_button(new s_text_button(
					"-", DARK_GREY,
					(coord + t_vect(5.5, 1.0 + (1.2 * (i)))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
					t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
				new t_button(new s_text_button(
					"", DARK_GREY,
					(coord + t_vect(6.75, 1.0 + (1.2 * (i)))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
					t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
				new t_button(new s_text_button(
					"+", DARK_GREY,
					(coord + t_vect(8, 1.0 + (1.2 * (i)))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
					t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
			value[num][k] = &(value_iterator->value);
			i++;
			gui.add(value_iterator);
		}
		num++;
		coord.x = 9 * (num % 3);
		i = (num >= 3 ? 6 : 0);
	}

	i += 6;

	t_button *button = new t_button(new t_text_button(
		"Saving effect", BLACK,
		gui.unit * (t_vect(1, 1.0 + (1.2 * (i))) + coord), gui.unit * t_vect(10, 1), 5,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		save_effect, t_data(3, spell, &play, text));

	gui.add(button);

	button = new t_button(new t_text_button(
		"Quit", BLACK,
		gui.unit * (t_vect(18, 1.0 + (1.2 * (i))) + coord), gui.unit * t_vect(10, 1), 5,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		quit_action_select, &play);

	gui.add(button);

	while (play == true)
	{
		prepare_screen();

		if (old_gui != NULL)
			old_gui->draw_self();

		gui.draw_self();

		render_screen(true);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT)
				menu_quit(t_data(1, &gui));
			if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				play = false;
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click(&event);
			else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
				gui.key_press(&event);
		}
	}

}
