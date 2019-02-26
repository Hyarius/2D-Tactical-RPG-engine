#include "taag.h"

void menu_edit_animation(t_data data)
{
	t_gui		*old_gui = (t_gui *)(data.data[0]);
	t_spell		*spell = (t_spell *)(data.data[1]);

	t_gui		gui;
	bool		play = true;
	SDL_Event	event;

	gui.add(new t_button(new t_text_button(
						"", BLACK,
						gui.unit * t_vect(1, 1), gui.unit * t_vect(28, 18), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						NULL, NULL));

	gui.add(new t_button(new t_text_button(
						"Which animation did you want ?", BLACK,
						gui.unit * t_vect(1.5, 1.5), gui.unit * t_vect(7, 1), 5,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						NULL, NULL));

	t_button *tile_name_button = new t_button(new t_text_button(
						"", BLACK,
						gui.unit * t_vect(8.75, 1.5), gui.unit * t_vect(7, 1), 5,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						menu_select_anim, NULL);

	string *tile_name = &(tile_name_button->button->text);

	tile_name_button->button->data_left = t_data(3, &gui, spell, tile_name); // 0 - t_gui * / 1 - t_spell * / 2 - &name

	*tile_name = "NULL";
	for (map<string, t_tileset>::const_iterator i = tileset_map.begin(); i != tileset_map.end(); ++i)
	{
		if (&(i->second) == spell->target_anim.tile)
		{
			*tile_name = i->first;
			break;
		}
	}

	gui.add(tile_name_button);

	int i = 1;

	t_iterator *start_frame = new s_iterator(&(spell->target_anim.start), NULL, 1, 1, 0, 100,
		new t_button(new s_text_button(
				"Starting frame : ", DARK_GREY,
				t_vect(1.5, 1.5 + (1.2 * (i))) * gui.unit, t_vect(7, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"-", DARK_GREY,
				t_vect(10.5, 1.5 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(11.75, 1.5 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"+", DARK_GREY,
				t_vect(13, 1.5 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i++;

	t_iterator *len_value = new s_iterator(&(spell->target_anim.len), NULL, 1, 1, 0, 100,
		new t_button(new s_text_button(
				"Animation len : ", DARK_GREY,
				t_vect(1.5, 1.5 + (1.2 * (i))) * gui.unit, t_vect(7, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"-", DARK_GREY,
				t_vect(10.5, 1.5 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(11.75, 1.5 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"+", DARK_GREY,
				t_vect(13, 1.5 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i++;

	int x = spell->target_anim.anim_size.x;
	t_iterator *size_x_value = new s_iterator(&(x), NULL, 1, 1, 1, 100,
		new t_button(new s_text_button(
				"Size x : ", DARK_GREY,
				t_vect(1.5, 1.5 + (1.2 * (i))) * gui.unit, t_vect(7, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"-", DARK_GREY,
				t_vect(10.5, 1.5 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(11.75, 1.5 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"+", DARK_GREY,
				t_vect(13, 1.5 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i++;

	int y = spell->target_anim.anim_size.y;

	t_iterator *size_y_value = new s_iterator(&(y), NULL, 1, 1, 1, 100,
		new t_button(new s_text_button(
				"Size y : ", DARK_GREY,
				t_vect(1.5, 1.5 + (1.2 * (i))) * gui.unit, t_vect(7, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"-", DARK_GREY,
				t_vect(10.5, 1.5 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(11.75, 1.5 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"+", DARK_GREY,
				t_vect(13, 1.5 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i++;

	t_iterator *anim_type = new s_iterator(&(spell->anim_type), NULL, 1, 1, 0, 2,
		new t_button(new s_text_button(
				"Animation type : ", DARK_GREY,
				t_vect(1.5, 1.5 + (1.2 * (i))) * gui.unit, t_vect(7, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"-", DARK_GREY,
				t_vect(10.5, 1.5 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(11.75, 1.5 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"+", DARK_GREY,
				t_vect(13, 1.5 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i++;

	gui.add(start_frame);
	gui.add(len_value);
	gui.add(size_x_value);
	gui.add(size_y_value);
	gui.add(anim_type);

	while (play == true)
	{
		spell->target_anim.anim_size = t_vect(x, y);
		prepare_screen();

		if (old_gui != NULL)
			old_gui->draw_self();

		gui.draw_self();

		if (spell->target_anim.tile != NULL)
		{
			t_vect coord = t_vect(16, 1.5);
			t_vect sprite = t_vect(0, 0);
			t_vect size = (t_vect(30, 20) - (coord + 2)) / spell->target_anim.tile->nb_sprite;
			if (size.x > size.y)
				size.x = size.y;
			else
				size.y = size.x;

			draw_border_rectangle(coord * gui.unit, (size * spell->target_anim.tile->nb_sprite + 0.5) * gui.unit, 4, t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6));

			int count = 0;
			while (sprite.x < spell->target_anim.tile->nb_sprite.x && sprite.y < spell->target_anim.tile->nb_sprite.y)
			{
				if (count == spell->target_anim.start)
					draw_border_rectangle((coord + (sprite * size) + 0.25) * gui.unit, size * gui.unit, 4, t_color(0.11, 0.54, 1), t_color(0.53, 0.8, 0.93));
				if (spell->target_anim.len != 0 && count - spell->target_anim.start == spell->target_anim.len)
					draw_border_rectangle((coord + (sprite * size) + 0.25) * gui.unit, size * gui.unit, 4, t_color(0.94, 0.50, 0.5), t_color(0.82, 0.32, 0.32));
				spell->target_anim.tile->draw_self((coord + (sprite * size) + 0.25) * gui.unit, size * gui.unit, sprite);
				count++;
				sprite.x++;
				if (sprite.x >= spell->target_anim.tile->nb_sprite.x)
				{
					sprite.x = 0;
					sprite.y++;
				}
			}
		}

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
