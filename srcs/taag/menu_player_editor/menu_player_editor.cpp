#include "taag.h"

void					menu_player_editor(t_data data)
{
	(void)data;
	SDL_Event	event;
	bool		play = true;
	t_gui 		gui;

	int i = 0;

	t_button *back_ground = new t_button(new t_image_button(t_image(t_color(0.2, 0.2, 0.2)), t_vect(0, 0), get_win_size()), NULL, NULL);

	t_entry *entry_path = new s_entry(new s_text_entry(	"File name of your character", "", BLACK,
			t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(8, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6), t_color(1.0, 0.95, 0)));
	i++;

	t_entry *entry_name = new s_entry(new s_text_entry(	"Name of your character", "", BLACK,
			t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(8, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6), t_color(1.0, 0.95, 0)));
	i++;

	int		pool = 30;
	t_iterator *pool_iterator = new s_iterator(&pool, &pool, 0, 0, 0, 100,
		new t_button(new s_text_button(
				"Attrib points (AtbP) left : ", DARK_GREY,
				t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL,
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(3.5, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		NULL);
	i++;

	t_iterator *hp_iterator = new s_iterator(50, &pool, 5, 1, 30, 75,
		new t_button(new s_text_button(
				"Health points (5 hp / AtbP): ", DARK_GREY,
				t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"-", DARK_GREY,
				t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"+", DARK_GREY,
				t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i++;

	t_iterator *action_iterator = new s_iterator(6, &pool, 1, 4, 5, 12,
		new t_button(new s_text_button(
				"Action points (1 act / 4 AtbP): ", DARK_GREY,
				t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"-", DARK_GREY,
				t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"+", DARK_GREY,
				t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i++;

	t_iterator *mouvement_iterator = new s_iterator(3, &pool, 1, 3, 3, 6,
		new t_button(new s_text_button(
				"Mouvement points (1 mvt / 3 AtbP): ", DARK_GREY,
				t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"-", DARK_GREY,
				t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"+", DARK_GREY,
				t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i++;

	t_iterator *initiative_iterator = new s_iterator(6, &pool, 1, 1, 4, 12,
		new t_button(new s_text_button(
				"Initiative (1 init / AtbP): ", DARK_GREY,
				t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"-", DARK_GREY,
				t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"", DARK_GREY,
				t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
				"+", DARK_GREY,
				t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 1) * gui.unit, 5,
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i++;

	size_t *index;
	i = 1;
	t_sprite_iterator *tileset_selector = new t_sprite_iterator(
				t_vect(4, 4),
				new t_button(new s_text_button(
						"Tileset : ", DARK_GREY,
						t_vect(9.2, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
				new t_button(new s_text_button(
						"-", DARK_GREY,
						t_vect(13.7, 1.0 + (1.2 * (i - 1))) * gui.unit, t_vect(1, 1 + (1.2 * (i + 1))) * gui.unit, 5,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
				new t_button(new s_tileset_button(&(sprite_map["simple_actor"]), t_vect(0, 0),
						t_vect(14.9, 1.0 + (1.2 * (i - 1))) * gui.unit, t_vect(1 + (1.2 * (i + 1)), 1 + (1.2 * (i + 1))) * gui.unit, 5),
						menu_choose_sprite, t_data(2, &gui, &index)),
				new t_button(new s_text_button(
						"+", DARK_GREY,
						t_vect(18.5, 1.0 + (1.2 * (i - 1))) * gui.unit, t_vect(1, 1 + (1.2 * (i + 1))) * gui.unit, 5,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i++;

			index = &(tileset_selector->i);
	string	*name = &(entry_name->entry->text);
	string	*path = &(entry_path->entry->text);
	int		*hp = &(hp_iterator->value);
	int		*action = &(action_iterator->value);
	int		*mouvement = &(mouvement_iterator->value);
	int		*initiative = &(initiative_iterator->value);

	gui.add(back_ground);
	gui.add(ENTRY_NUM, entry_name);
	gui.add(ENTRY_NUM, entry_path);
	gui.add(pool_iterator);
	gui.add(hp_iterator);
	gui.add(action_iterator);
	gui.add(mouvement_iterator);
	gui.add(initiative_iterator);
	gui.add(tileset_selector);

	while (play)
	{
		prepare_screen();

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
