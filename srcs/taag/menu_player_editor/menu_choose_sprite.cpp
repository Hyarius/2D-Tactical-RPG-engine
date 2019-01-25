#include "taag.h"

static void		quit_choose_sprite(t_data data)
{
	size_t			*index = *(size_t **)(data.data[0]);
	t_vect			*selected = *(t_vect **)(data.data[1]);
	int				*tmp_index = (int *)(data.data[2]);
	t_vect			*tmp_selected = (t_vect *)(data.data[3]);
	bool			*continu = (bool *)(data.data[4]);

	printf("index = %zu\n", *index);
	*index = *tmp_index;
	printf("select = %.2f / %.2f\n", (*selected).x, (*selected).y);
	printf("tmp = %.2f / %.2f\n", (*tmp_selected).x, (*tmp_selected).y);
	*selected = *tmp_selected;
	*continu = false;
}

static void		increment_index(t_data data)
{
	size_t *index = (size_t *)(data.data[0]);
	int delta = (int &)(data.data[1]);

	*index = (*index + delta) % sprite_name.size();
}

void			menu_choose_sprite(t_data data)
{
	bool			continu = true;
	SDL_Event		event;
	t_gui			gui;

	size_t *index = *((size_t **)(data.data[1]));
	t_vect *target = *((t_vect **)(data.data[2]));
	size_t tmp_index = *index;

	s_button *button = new s_button(new s_text_button(
					"", DARK_GREY,
					gui.unit * t_vect(2, 3.5), gui.unit * t_vect(26, 13), 8,
					t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
					NULL, NULL);
	gui.add(button);

	t_vect coord = t_vect(0, 0);
	t_vect size = t_vect(5, 5);
	double x = (22 - (size.x * 4)) / 3;
	double y = (11 - (size.y * 2)) / 1;
	t_vect space = t_vect(x, y);
	t_vect selected = t_vect(0, 0);
	t_vect increment = t_vect(4, 4);

	int i = 0;
	while (i < 8)
	{
		t_button *button = new t_button(new t_text_button("", BLACK, // text info
					(coord + t_vect(3, 4.5)) * gui.unit, size * gui.unit, 8, // coord / size info
					t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
					quit_choose_sprite, t_data(5, data.data[1], data.data[2], &tmp_index, new t_vect(selected), &continu));
		gui.add(button);
		selected.x += increment.x;
		if (selected.x >= sprite_map[sprite_name[tmp_index]].nb_sprite.x)
			selected = t_vect(0, selected.y + increment.y);
		coord.x += size.x + space.x;
		if (coord.x + size.x > 25)
			coord = t_vect(0, coord.y + size.y + space.y);
		i++;
	}

	button = new t_button(new t_text_button("prev", BLACK, // text info
				(t_vect(3 + size.x * 4 + space.x * 4, 4.5)) * gui.unit, t_vect(1, 5) * gui.unit, 8, // coord / size info
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
				increment_index, t_data(2, &tmp_index, -1));
	gui.add(button);

	button = new t_button(new t_text_button("next", BLACK, // text info
				(t_vect(3 + size.x * 4 + space.x * 4, 10.5)) * gui.unit, t_vect(1, 5) * gui.unit, 8, // coord / size info
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
				increment_index, t_data(2, &tmp_index, 1));
	gui.add(button);


	while (continu == true)
	{
		prepare_screen();

		if (data.data.size() != 0)
			(*((t_gui *)(data.data[0]))).draw_self();
		gui.draw_self();

		i = 0;
		coord = t_vect(0, 0);
		selected = t_vect(0, 0);
		while (i < 8)
		{
			sprite_map[sprite_name[tmp_index]].draw_self((coord + t_vect(3, 4.5)) * gui.unit, size * gui.unit, selected);
			selected.x += increment.x;
			if (selected.x >= sprite_map[sprite_name[tmp_index]].nb_sprite.x)
				selected = t_vect(0, selected.y + increment.y);
			coord.x += size.x + space.x;
			if (coord.x + size.x > 25)
				coord = t_vect(0, coord.y + size.y + space.y);
			i++;
		}


		render_screen(true);

		if (SDL_PollEvent(&(event)) == 1)
		{
			if (event.type == SDL_QUIT)
				menu_quit(t_data(2, &gui, &continu));
			else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
				continu = false;
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click();
			else if (event.type == SDL_MOUSEWHEEL && event.wheel.y < 0)
				tmp_index = ((int)(tmp_index) - 1 < 0 ? sprite_name.size() - 1 : tmp_index - 1);
			else if (event.type == SDL_MOUSEWHEEL && event.wheel.y > 0)
				tmp_index = ((int)(tmp_index) + 1 >= (int)(sprite_name.size()) ? 0 : tmp_index + 1);
		}
	}
}
