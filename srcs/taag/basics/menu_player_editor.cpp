#include "taag.h"

typedef struct	s_iterator : t_gui_obj
{
	int			value;
	int			*linked_value;
	t_button	*desc;
	t_button	*minus;
	t_button	*container;
	t_button	*plus;
				s_iterator();
				s_iterator(int base_value, int *pool, int delta, int pool_cost, int min, int max,
							t_button *p_desc, t_button *p_minus, t_button *p_contain, t_button *p_plus);
				s_iterator(int *p_linked_value, int *pool, int delta, int pool_cost, int min, int max,
							t_button *p_desc, t_button *p_minus, t_button *p_contain, t_button *p_plus);
	void		draw_self();
	bool		click(t_vect mouse);
	bool		key_press(SDL_Event *event);
}				t_iterator;

				s_iterator::s_iterator()
{}

static void		increment_iterator(t_data data) //0 - &value / 1 - &pool / 2 - increment / 3 - cost / 4 - min / 5 - max
{
	int *value = (int *)(data.data[0]);
	int *pool = (int *)(data.data[1]);
	int	delta = (int &)(data.data[2]);
	int	cost = (int &)(data.data[3]);
	int	min = (int &)(data.data[4]);
	int	max = (int &)(data.data[5]);

	if (value != NULL)
	{
		if (*value + delta <= max && *pool - cost >= 0)
		{
			*value += delta;
			*pool -= cost;
		}
	}
}

static void		decrement_iterator(t_data data) //0 - &value / 1 - &pool / 2 - increment / 3 - cost / 4 - min / 5 - max
{
	int *value = (int *)(data.data[0]);
	int *pool = (int *)(data.data[1]);
	int	delta = (int &)(data.data[2]);
	int	cost = (int &)(data.data[3]);
	int	min = (int &)(data.data[4]);
	int	max = (int &)(data.data[5]);

	if (value != NULL)
	{
		if (*value - delta >= min)
		{
			*value -= delta;
			*pool += cost;
		}
	}
}

				s_iterator::s_iterator(int base_value, int *pool, int delta, int pool_cost, int min, int max,
					t_button *p_desc, t_button *p_minus, t_button *p_contain, t_button *p_plus)
{
	value = base_value;
	linked_value = NULL;
	desc = p_desc;
	minus = p_minus;
	if (minus != NULL)
	{
		minus->button->funct = decrement_iterator;
		minus->button->data = t_data(6, &value, pool, delta, pool_cost, min, max); //0 - &value / 1 - &pool / 2 - increment / 3 - cost / 4 - min / 5 - max
	}
	container = p_contain;
	plus = p_plus;
	if (plus != NULL)
	{
		plus->button->funct = increment_iterator;
		plus->button->data = t_data(6, &value, pool, delta, pool_cost, min, max); //0 - &value / 1 - &pool / 2 - increment / 3 - cost
	}
}

				s_iterator::s_iterator(int *p_linked_value, int *pool, int delta, int pool_cost, int min, int max,
					t_button *p_desc, t_button *p_minus, t_button *p_contain, t_button *p_plus)
{
	linked_value = p_linked_value;
	value = *linked_value;
	desc = p_desc;
	minus = p_minus;
	if (minus != NULL)
	{
		minus->button->funct = decrement_iterator;
		minus->button->data = t_data(6, &value, pool, delta, pool_cost, min, max); //0 - &value / 1 - &pool / 2 - increment / 3 - cost / 4 - min / 5 - max
	}
	container = p_contain;
	plus = p_plus;
	if (plus != NULL)
	{
		plus->button->funct = increment_iterator;
		plus->button->data = t_data(6, &value, pool, delta, pool_cost, min, max); //0 - &value / 1 - &pool / 2 - increment / 3 - cost / 4 - min / 5 - max
	}
}

void			s_iterator::draw_self()
{
	if (desc != NULL)
		desc->draw_self();
	if (minus != NULL)
		minus->draw_self();
	if (container != NULL)
	{
		if (linked_value != NULL)
			value = *linked_value;
		container->button->text = to_string(value);
		container->draw_self();
	}
	if (plus != NULL)
		plus->draw_self();
}

bool			s_iterator::click(t_vect mouse)
{
	if (minus != NULL && minus->click(mouse) == true)
		return (true);
	if (plus != NULL && plus->click(mouse) == true)
		return (true);
	return (false);
}

bool			s_iterator::key_press(SDL_Event *event)
{
	(void)event;
	return (false);
}

void					menu_player_editor(t_data data)
{
	(void)data;
	SDL_Event	event;
	bool		play = true;
	t_gui 		gui;

	t_entry *entry_path = new s_entry(new s_text_entry(	"File name of your character", "", BLACK,
			t_vect(1, 1) * gui.unit, t_vect(8, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6), t_color(1.0, 0.95, 0)));

	t_entry *entry_name = new s_entry(new s_text_entry(	"Name of your character", "", BLACK,
			t_vect(1, 2.2) * gui.unit, t_vect(8, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6), t_color(1.0, 0.95, 0)));

	int		pool = 30;
	t_iterator *pool_iterator = new s_iterator(&pool, &pool, 0, 0, 0, 100,
		new t_button(new s_text_button(
				"Attrib points (AtbP) left : ", DARK_GREY,
				t_vect(1, 3.4) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL,
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(5.5, 3.4) * gui.unit, t_vect(3.5, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL);

	t_iterator *hp_iterator = new s_iterator(50, &pool, 5, 1, 30, 75,
		new t_button(new s_text_button(
				"Health points (5 hp / AtbP): ", DARK_GREY,
				t_vect(1, 4.6) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"-", DARK_GREY,
				t_vect(5.5, 4.6) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(6.75, 4.6) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"+", DARK_GREY,
				t_vect(8, 4.6) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));

	string *name = &(entry_name->entry->text);
	string *path = &(entry_path->entry->text);

	gui.add(ENTRY_NUM, entry_name);
	gui.add(ENTRY_NUM, entry_path);
	gui.add(pool_iterator);
	gui.add(hp_iterator);

	while (play)
	{
		prepare_screen(t_color(0.2, 0.2, 0.2));

		gui.draw_self();

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(2, &gui, &play));
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click();
			else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
				gui.key_press(&event);
		}
		render_screen(true);
	}
}
