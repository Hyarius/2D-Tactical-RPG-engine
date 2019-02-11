#include "taag.h"

string 				*text_list_effect_name[30];

static void	modify_index(t_data data)
{
	int *index = (int *)(data.data[0]);
	int delta = (int &)(data.data[1]);

	if (*index + delta >= -27 && *index + delta < (int)(((list_effect_name.size() / 3) * 3 + 3)))
		*index += delta;
	for (int i = 0; i < 30; i++)
	{
		if (*index + i >= (int)(list_effect_name.size()) || *index + i < 0)
			*(text_list_effect_name[i]) = "";
		else
			*(text_list_effect_name[i]) = list_effect_name[i + *index];
	}
}

static void		quit_choose(t_data data)// string * / bool * / num
{
	string		*entry_name = (string *)(data.data[0]);
	bool 		*play = (bool *)(data.data[1]);
	int			i = (int &)(data.data[2]);

	if (*(text_list_effect_name[i]) != "")
	{
		*entry_name = *(text_list_effect_name[i]);
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
			(i < (int)(list_effect_name.size()) ? list_effect_name[i] : ""), BLACK,
			gui.unit * t_vect(1.5 + ((i % 3) * 8.3 + (i % 3)), 4 + ((i / 3) * 1.3)),
			gui.unit * t_vect(8.3, 1.1),
			8,
			t_color(0.4, 0.4, 0.4),
			t_color(0.6, 0.6, 0.6)),
			quit_choose, t_data(3, str, &play, i));
		text_list_effect_name[i] = &(button->button->text);
		gui.add(button);
		i++;
	}

	while (play == true)
	{
		prepare_screen();

		if (old_gui != NULL)
			old_gui->draw_self();
		if (back_gui != NULL)
			back_gui->draw_self();

		gui.draw_self();

		render_screen(true);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT)
				menu_quit(t_data(1, &gui));
			if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				play = false;
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click();
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

static void quit_effect_select(t_data data)
{
	bool *play = (bool *)(data.data[0]);

	*play = false;
}

//save_effect, t_data(4, spell, entry, &(value[0][0]), &play));

int *value[6][4];
string *entry[6];

string front_text[] = {
	"deal ",
	"heal ",
	"give ",
	"give ",
	"push the target by ",
	"pull the target by ",
	"heal the caster by ",
	"deal ",
	"move the caster to the targeted location\0",
	"change position between caster and target\0",
	"give ",
	"give "
};

string back_text[] = {
	" damage(s) to the target",
	" HP(s) to the target",
	" PM(s) to the target",
	" PA(s) to the target",
	" square(s)",
	" square(s)",
	" HP(s)",
	" damage(s) to the caster",
	"",
	"",
	" PM(s) to the caster",
	" PA(s) to the caster",
};

static string compute_text(size_t size, int count, t_effect *effect, int nb_effect)
{
	if (count == -1)
		return "";
	string pre_part = (size != 0 ? (nb_effect == 1 ? " and " : ", ") : "");
	string front_part = front_text[count];
	if (size == 0)
		front_part[0] = front_part[0] + ('A' - 'a');
	string back_part = back_text[count];
	string value = (effect->stat.value[0] != 0 ? to_string(effect->stat.value[0]) : "");
	return (pre_part + front_part + value + back_part);

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
		if (*(entry[i]) != "")
			nb_effect++;
	}

	for (int i = 0; i < 6; i++)
	{
		int count = 0;

		while (count < (int)(list_effect_name.size()) && list_effect_name[count] != *(entry[i]))
			count++;
		if (count == (int)(list_effect_name.size()))
			count = -1;
		else
			count--;
		if (count != -1)
			spell->effect[i].effect = g_effects[count];
		*(text[i]) = list_effect_name[count + 1];
		for (int j = 0; j < 4; j++)
		{
			spell->effect[i].stat.value[j] = *(value[i][j]);
		}
		desc.append(compute_text(desc.size(), count, &(spell->effect[i]), nb_effect - i));
	}

	spell->desc = desc;
	*play = false;
}

void menu_select_effect(t_data data)
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
		button->button->data = t_data(3, old_gui, &gui, &(button->button->text));
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
		quit_effect_select, &play);

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
				gui.click();
			else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
				gui.key_press(&event);
		}
	}

}
