#include "taag.h"

static string 			create_basic_actor()
{
	string text = "name:\n";
	text.append("tileset:H00\n");
	text.append("sprite_pos:0:0\n");
	text.append("health:50\n");
	text.append("action:6\n");
	text.append("mouvement:3\n");
	text.append("initiative:5\n");
	text.append("spell0:Slash\n");
	text.append("spell1:NULL\n");
	text.append("spell2:NULL\n");
	text.append("spell3:NULL\n");
	text.append("spell4:NULL\n");
	text.append("spell5:NULL");
	return (text);
}

void					menu_actor_editor(t_data data)
{
	SDL_Event	event;
	bool		play = true;
	t_gui 		gui;

	double i = 0;

	t_button *back_ground = new t_button(new t_image_button(t_image(t_color(0.2, 0.2, 0.2)), t_vect(0, 0), get_win_size()), NULL, NULL);

	string *path = (string *)(data.data[0]);
	if (*path == "NULL" || check_file_exist(ACTOR_PATH + *path + ACTOR_EXT) == false)
	{
		int j = 0;
		string text = ("actor" + to_string(j));
		while (check_file_exist(ACTOR_PATH + text + ACTOR_EXT) == true)
		{
			j++;
			text = "actor" + to_string(j);
		}
		rewrite_on_file(ACTOR_PATH + text + ACTOR_EXT, create_basic_actor());
		*path = text;
	}

	t_actor actor = read_actor(ACTOR_PATH + (*path) + ACTOR_EXT);

	t_entry *entry_name = new s_entry(new s_text_entry(	"Name of your character", actor.name, BLACK,
			t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(8, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6), t_color(1.0, 0.95, 0)));
	i++;
	string *name = &(entry_name->entry->text);

	entry_name->entry->max_len = 32;

	t_actor base;
	int pool = 30 - ((actor.stat.hp.max - base.stat.hp.max) / 5 + (actor.stat.pa.max - base.stat.pa.max) * 3 + (actor.stat.pm.max - base.stat.pm.max) * 3 + (actor.stat.init - base.stat.init));
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


	t_iterator *hp_iterator = new s_iterator(&(actor.stat.hp.max), &pool, 5, 1, 30, 150,
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

	t_iterator *action_iterator = new s_iterator(&(actor.stat.pa.max), &pool, 1, 3, 5, 12,
		new t_button(new s_text_button(
				"Action points (1 act / 3 AtbP): ", DARK_GREY,
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

	t_iterator *mouvement_iterator = new s_iterator(&(actor.stat.pm.max), &pool, 1, 3, 2, 6,
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

	t_iterator *initiative_iterator = new s_iterator(&(actor.stat.init), &pool, 1, 1, 4, 12,
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

	size_t	*tile_index;
	t_vect	*sprite_target;

	i += 3.4;
	t_button	*save_button = new t_button(new s_text_button(
			"Save actor", DARK_GREY,
			t_vect(1, 1 + (1.2 * i)) * gui.unit, t_vect(8, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
			menu_save_actor, t_data(5, &gui, &actor, path, &tile_index, &sprite_target));//0 - gui / 1 - t_actor * / 2 - & file name
	i++;

	t_button	*load_button = new t_button(new s_text_button(
			"Load actor", DARK_GREY,
			t_vect(1, 1 + (1.2 * i)) * gui.unit, t_vect(8, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
			menu_load_actor, t_data(7, &gui, &actor, name, path, &pool, &tile_index, &sprite_target));// 0 - t_gui * / 1 - t_actor * / 2 - &name / 3 - &path / 4 - &pool
	i++;

	t_button	*delete_button = new t_button(new s_text_button(
			"Delete actor", DARK_GREY,
			t_vect(1, 1 + (1.2 * i)) * gui.unit,
			t_vect(8, 1) * gui.unit,
			5,
			t_color(0.4, 0.4, 0.4),
			t_color(0.6, 0.6, 0.6)),
			menu_delete_actor,
			t_data(7, &gui, &actor, name, path, &pool, &tile_index, &sprite_target));// 0 - t_gui * / 1 - t_actor * / 2 - &name / 3 - &path / 4 - &pool
	i++;

	t_button	*quit_button = new t_button(new s_text_button(
			"Quit", DARK_GREY,
			t_vect(1, 1 + (1.2 * i)) * gui.unit, t_vect(8, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
			menu_quit, t_data(2, &gui, &play));
	i++;

	i = 1;
	t_tileset_button *tile_button = new s_tileset_button(actor.tile, actor.sprite, t_vect(4, 0),
			t_vect(14.9, 1.0 + (1.2 * (i - 1))) * gui.unit, t_vect(1 + (1.2 * (i + 1)), 1 + (1.2 * (i + 1))) * gui.unit, 5);
	//t_vect *sprite = &(tile_button->selected);
	t_sprite_iterator *tileset_selector = new t_sprite_iterator(
				t_vect(3, 4), &(account->tile_unlock),
				new t_button(new s_text_button(
						"Tileset : ", DARK_GREY,
						t_vect(9.2, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 1) * gui.unit, 5,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
				new t_button(new s_text_button(
						"-", DARK_GREY,
						t_vect(13.7, 1.0 + (1.2 * (i - 1))) * gui.unit, t_vect(1, 1 + (1.2 * (i + 1))) * gui.unit, 5,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
				new t_button(tile_button,
						NULL, NULL),
				new t_button(new s_text_button(
						"+", DARK_GREY,
						t_vect(18.5, 1.0 + (1.2 * (i - 1))) * gui.unit, t_vect(1, 1 + (1.2 * (i + 1))) * gui.unit, 5,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	((t_tileset_button *)(tileset_selector->container->button))->data_left = t_data(4, &gui, &(tileset_selector->i), &((t_tileset_button *)(tileset_selector->container->button))->selected, &(account->tile_unlock));
	i++;

	tile_index = &(tileset_selector->i);
	sprite_target = &(((t_tileset_button *)(tileset_selector->container->button))->selected);

	gui.add(back_ground);
	gui.add(ENTRY_NUM, entry_name);
	gui.add(pool_iterator);
	gui.add(hp_iterator);
	gui.add(action_iterator);
	gui.add(mouvement_iterator);
	gui.add(initiative_iterator);
	gui.add(tileset_selector);
	gui.add(save_button);
	gui.add(load_button);
	gui.add(delete_button);
	gui.add(quit_button);

	for (int i = 0; i < 6; i++)
		gui.add(new t_spell_card(&actor.spell[i], gui.unit * t_vect(9.2 + (i % 3) * 4 + (0.2 * (i % 3)), (4.6 + (i / 3) * 6 + (0.2 * (i / 3)))), gui.unit * t_vect(4, 6), menu_choose_spell, t_data(3, &gui, i, &actor)));

	while (play)
	{
		actor.name = *name;
		actor.tile = &(sprite_map[account->tile_unlock[tileset_selector->i % account->tile_unlock.size()]]);
		actor.sprite = *sprite_target;

		prepare_screen();

		gui.draw_self();

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				menu_quit(t_data(2, &gui, &play));
			else if (event.type == SDL_MOUSEBUTTONUP)
				gui.click(&event);
			else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
				gui.key_press(&event);

		}
		render_screen(true);
	}
	account->recharge_actor();
}
