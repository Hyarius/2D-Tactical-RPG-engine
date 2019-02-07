#include "taag.h"

static void		increment_iterator(t_data data)
{
	t_tileset *tile = (t_tileset *)(data.data[0]);
	t_vect *selected = (t_vect *)(data.data[1]);
	t_vect delta = *((t_vect *)(data.data[2]));

	selected->x += delta.x;
	if (tile->nb_sprite.x <= selected->x)
	{
		selected->x = 0;
		selected->y += delta.y;
	}
	if (tile->nb_sprite.y <= selected->y)
	{
		selected->x = 0;
		selected->y = 0;
	}
}

static void		decrement_iterator(t_data data)//0 - &tileset / 1 - &selected / 2 - &delta
{
	t_tileset *tile = (t_tileset *)(data.data[0]);
	t_vect *selected = (t_vect *)(data.data[1]);
	t_vect delta = *(t_vect *)(data.data[2]);

	selected->x -= delta.x;
	if (selected->x < 0)
	{
		selected->x = tile->nb_sprite.x - delta.x;
		selected->y -= delta.y;
	}
	if (selected->y < 0)
	{
		selected->x = tile->nb_sprite.x - delta.x;
		selected->y = tile->nb_sprite.y - delta.y;
	}
}

s_tileset_iterator::s_tileset_iterator()
{

}

static void		quit_choose_tileset(t_data data)
{
	t_vect			*selected = (t_vect *)(data.data[0]);
	t_vect			*tmp_selected = (t_vect *)(data.data[1]);
	bool			*play = (bool *)(data.data[2]);

	*selected = *tmp_selected;
	*play = false;
}

static void			menu_choose_tileset(t_data data)
{
	bool			play = true;
	SDL_Event		event;
	t_gui			gui;

	t_tileset *tile = ((t_tileset *)(data.data[0]));
	t_vect *target = ((t_vect *)(data.data[1]));

	s_button *button = new s_button(new s_text_button(
		"", DARK_GREY,
		gui.unit * t_vect(0.5, 0.5), gui.unit * t_vect(29, 19), 8,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		NULL, NULL);
	gui.add(button);

	t_vect coord = t_vect(1, 1);
	t_vect size = t_vect((28 - (0.1 * 14)) / 14, (18 - (0.1 * 9)) / 9);
	t_vect space = t_vect(0.1, 0.1);
	t_vect selected = t_vect(0, 0);
	t_vect increment = t_vect(1, 1);

	int i = 0;
	while (i < 126)
	{
		t_button *button = new t_button(new t_tileset_button(
						tile,
						selected,
						t_vect(1, 0),
						t_vect(coord.x + (size.x + space.x) * (i % 14), coord.y + (size.y + space.y) * (i / 14)) * gui.unit,
						size * gui.unit,
						5),
						quit_choose_tileset, NULL);
		button->button->data = t_data(3, target, &(((t_tileset_button *)(button->button))->selected), &play);
		gui.add(button);
		selected.x += increment.x;
		if (selected.x >= tile->nb_sprite.x)
			selected = t_vect(0, selected.y + increment.y);
		if (selected.y >= tile->nb_sprite.y)
			selected = t_vect(0, 0);
		i++;
	}

	while (play == true)
	{
		prepare_screen();

		gui.draw_self();


		render_screen(true);

		if (SDL_PollEvent(&(event)) == 1)
		{
			if (event.type == SDL_QUIT)
				menu_quit(t_data(1, &gui));
			if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				play = false;
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click();
		}
	}
}

s_tileset_iterator::s_tileset_iterator(t_vect p_delta, t_button *p_desc, t_button *p_minus, t_button *p_contain, t_button *p_plus)
{
	delta = p_delta;
	desc = p_desc;
	minus = p_minus;
	container = p_contain;
	if (container != NULL)
	{
		container->button->funct = menu_choose_tileset;
		container->button->data = t_data(2, ((t_tileset_button *)(container->button))->tile, &((t_tileset_button *)(container->button))->selected); //0 - &index / 1 - &selected / 2 - &delta
	}
	if (minus != NULL && container != NULL)
	{
		minus->button->funct = decrement_iterator;
		minus->button->data = t_data(3, ((t_tileset_button *)(container->button))->tile, &((t_tileset_button *)(p_contain->button))->selected, &delta); //0 - &index / 1 - &selected / 2 - &delta
	}
	plus = p_plus;
	if (plus != NULL && container != NULL)
	{
		plus->button->funct = increment_iterator;
		plus->button->data = t_data(3, ((t_tileset_button *)(container->button))->tile, &((t_tileset_button *)(p_contain->button))->selected, &delta); //0 - &index / 1 - &selected / 2 - &delta
	}
}

void			s_tileset_iterator::draw_self()
{
	if (desc != NULL)
		desc->draw_self();
	if (minus != NULL)
		minus->draw_self();
	if (container != NULL)
		container->draw_self();
	if (plus != NULL)
		plus->draw_self();
}

bool			s_tileset_iterator::click(t_vect mouse)
{
	if (minus != NULL && minus->click(mouse) == true)
		return (true);
	if (container != NULL && container->click(mouse) == true)
		return (true);
	if (plus != NULL && plus->click(mouse) == true)
		return (true);
	return (false);
}

bool			s_tileset_iterator::key_press(SDL_Event *event)
{
	(void)event;
	return (false);
}
