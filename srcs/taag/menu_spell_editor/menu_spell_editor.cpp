#include "taag.h"

vector<string>		list_action_name;

static void			set_list_action_name()
{
	list_action_name.push_back("Empty effect");
	list_action_name.push_back("deal dmg"); 		//0
	list_action_name.push_back("heal");			//1
	list_action_name.push_back("change pm");		//2
	list_action_name.push_back("change pa");		//3
	list_action_name.push_back("push actor");	//4
	list_action_name.push_back("pull actor");	//5
	list_action_name.push_back("heal caster");	//6
	list_action_name.push_back("dmg caster");	//7
	list_action_name.push_back("move caster"); 	//8
	list_action_name.push_back("swap actor");	//9
	list_action_name.push_back("change caster pa");	//10
	list_action_name.push_back("change caster pm");	//11
	list_action_name.push_back("push caster");	//12
	list_action_name.push_back("pull caster");	//13
	list_action_name.push_back("apply poison");	//13
	list_action_name.push_back("apply regeneration");	//13
	list_action_name.push_back("apply change PA");	//13
	list_action_name.push_back("apply change PM");	//13
	list_action_name.push_back("cure poison");	//13
	list_action_name.push_back("cure regeneration");	//13
	list_action_name.push_back("cure malus pa");	//13
	list_action_name.push_back("cure malus pm");	//13
	list_action_name.push_back("cure bonus pa");	//13
	list_action_name.push_back("cure bonus pm");	//13
	list_action_name.push_back("give armor");	//13
	list_action_name.push_back("remove armor");	//13
	list_action_name.push_back("true damage");	//13
	list_action_name.push_back("give armor caster");	//13
	list_action_name.push_back("remove armor caster");	//13
}

void menu_spell_editor(t_data data)
{
	(void)data; (void)data;
	SDL_Event	event;
	bool		play = true;
	t_gui 		gui;

	double i = 0;

	if (list_action_name.size() == 0)
		set_list_action_name();

	t_button *back_ground = new t_button(new t_image_button(t_image(t_color(0.2, 0.2, 0.2)), t_vect(0, 0), get_win_size()), NULL, NULL);

	t_entry *entry_path = new s_entry(new s_text_entry("File name of your spell", "", BLACK,
		t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(8, 1) * gui.unit, 5,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6), t_color(1.0, 0.95, 0.0)));
	i++;

	entry_path->entry->back = SPELL_EXT;

	t_entry *entry_name = new s_entry(new s_text_entry("Name of your spell", "", BLACK,
		t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(8, 1) * gui.unit, 5,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6), t_color(1.0, 0.95, 0.0)));
	i++;

	entry_name->entry->max_len = 32;

	t_spell *spell = new t_spell();

	t_spell_card *card = new s_spell_card(&spell, t_vect(18.5, 1) * gui.unit, t_vect(10.5, 18) * gui.unit, NULL, NULL);

	t_iterator *spell_type_iterator = new s_iterator(&(spell->m_spell), NULL, 1, 1, 0, 1,
		new t_button(new s_text_button(
			"Spell restriction : ", DARK_GREY,
			t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"-", DARK_GREY,
			t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"+", DARK_GREY,
			t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i += 0.89;

	t_iterator *pa_iterator = new s_iterator(&(spell->cost_pa), NULL, 1, 1, 0, 10,
		new t_button(new s_text_button(
			"Cost PA: ", DARK_GREY,
			t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"-", DARK_GREY,
			t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"+", DARK_GREY,
			t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i += 0.89;


	t_iterator *pm_iterator = new s_iterator(&(spell->cost_pm), NULL, 1, 1, 0, 10,
		new t_button(new s_text_button(
			"Cost PM: ", DARK_GREY,
			t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"-", DARK_GREY,
			t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"+", DARK_GREY,
			t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i += 0.89;


	t_iterator *cooldown_iterator = new s_iterator(&(spell->cooldown), NULL, 1, 1, 0, 5,
		new t_button(new s_text_button(
			"Cooldown : ", DARK_GREY,
			t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"-", DARK_GREY,
			t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"+", DARK_GREY,
			t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i += 0.89;

	t_iterator *range_type_iterator = new s_iterator(&(spell->range_type), NULL, 1, 1, 0, 1,
		new t_button(new s_text_button(
			"Range type: ", DARK_GREY,
			t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"-", DARK_GREY,
			t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"+", DARK_GREY,
			t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i += 0.89;


	t_iterator *block_range_iterator = new s_iterator(&(spell->block), NULL, 1, 1, 0, 1,
		new t_button(new s_text_button(
			"Vision line bocked ? : ", DARK_GREY,
			t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"-", DARK_GREY,
			t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"+", DARK_GREY,
			t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i += 0.89;


	t_iterator *min_range_iterator = new s_iterator(&(spell->range[0]), NULL, 1, 1, 0, 10,
		new t_button(new s_text_button(
			"Range min: ", DARK_GREY,
			t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"-", DARK_GREY,
			t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"+", DARK_GREY,
			t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i += 0.89;


	t_iterator *max_range_iterator = new s_iterator(&(spell->range[1]), NULL, 1, 1, 0, 10,
		new t_button(new s_text_button(
			"Range max: ", DARK_GREY,
			t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"-", DARK_GREY,
			t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"+", DARK_GREY,
			t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i += 0.89;


	t_iterator *zone_type_iterator = new s_iterator(&(spell->zone_type), NULL, 1, 1, 0, 3,
		new t_button(new s_text_button(
			"Zone type: ", DARK_GREY,
			t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"-", DARK_GREY,
			t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"+", DARK_GREY,
			t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i += 0.89;


	t_iterator *zone_size_iterator = new s_iterator(&(spell->zone_size), NULL, 1, 1, 0, 30,
		new t_button(new s_text_button(
			"Zone size: ", DARK_GREY,
			t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"-", DARK_GREY,
			t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"+", DARK_GREY,
			t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i += 0.89;


	t_iterator *target_type_iterator = new s_iterator(&(spell->on_target), NULL, 1, 1, 0, 2,
		new t_button(new s_text_button(
			"Target type: ", DARK_GREY,
			t_vect(1, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"-", DARK_GREY,
			t_vect(5.5, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(6.75, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"+", DARK_GREY,
			t_vect(8, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i += 0.81;

	t_button	*save_button = new t_button(new s_text_button(
		"Save spell", DARK_GREY,
		t_vect(1, 1 + (1.2 * i)) * gui.unit, t_vect(8, 0.78) * gui.unit, 5,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		menu_save_spell, t_data(3, &gui, spell, &(entry_path->entry->text)));//0 - gui / 1 - t_spell * / 2 - & file name
	i += 0.81;

	t_vect *selected;
	string		*text[6];

	t_button	*load_button = new t_button(new s_text_button(
		"Load spell", DARK_GREY,
		t_vect(1, 1 + (1.2 * i)) * gui.unit, t_vect(8, 0.78) * gui.unit, 5,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		menu_load_spell, t_data(6, &gui, spell, &(entry_name->entry->text), &(entry_path->entry->text), &selected, text));// 0 - t_gui * / 1 - t_spell * / 2 - &name / 3 - &path / 4 - &sprite
	i += 0.81;

	t_button	*delete_button = new t_button(new s_text_button(
		"Delete spell", DARK_GREY,
		t_vect(1, 1 + (1.2 * i)) * gui.unit,
		t_vect(3.75, 0.78) * gui.unit,
		5,
		t_color(0.4, 0.4, 0.4),
		t_color(0.6, 0.6, 0.6)),
		menu_delete_spell, t_data(6, &gui, spell, &(entry_name->entry->text), &(entry_path->entry->text), &selected, text));// 0 - t_gui * / 1 - t_spell * / 2 - &name / 3 - &path / 4 - &sprite

	t_button	*new_button = new t_button(new s_text_button(
		"New spell", DARK_GREY,
		t_vect(5.25, 1 + (1.2 * i)) * gui.unit,
		t_vect(3.75, 0.78) * gui.unit,
		5,
		t_color(0.4, 0.4, 0.4),
		t_color(0.6, 0.6, 0.6)),
		menu_new_spell, t_data(6, &gui, spell, &(entry_name->entry->text), &(entry_path->entry->text), &selected, text));// 0 - t_gui * / 1 - t_spell * / 2 - &name / 3 - &path / 4 - &sprite

	i += 0.81;

	t_button	*quit_button = new t_button(new s_text_button(
		"Quit", DARK_GREY,
		t_vect(1, 1 + (1.2 * i)) * gui.unit, t_vect(8, 0.78) * gui.unit, 5,
		t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
		menu_quit, t_data(2, &gui, &play));

	i = 1;
	t_tileset_button *tile_button = new s_tileset_button(get_interface_tile("simple_iconset"), t_vect(0, 0), t_vect(1, 0),
		t_vect(13.7, 1.0 + (1.2 * (i - 1))) * gui.unit, t_vect(1 + (1.2 * (i + 1)), 1 + (1.2 * (i + 1))) * gui.unit, 5);
	s_tileset_iterator *tileset_selector = new s_tileset_iterator(
		t_vect(1, 1),
		new t_button(new s_text_button(
			"Tileset : ", DARK_GREY,
			t_vect(9.2, 1.0 + (1.2 * (i))) * gui.unit, t_vect(3, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"-", DARK_GREY,
			t_vect(12.5, 1.0 + (1.2 * (i - 1))) * gui.unit, t_vect(1, 1 + (1.2 * (i + 1))) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(tile_button,
			NULL, NULL),
		new t_button(new s_text_button(
			"+", DARK_GREY,
			t_vect(17.25, 1.0 + (1.2 * (i - 1))) * gui.unit, t_vect(1, 1 + (1.2 * (i + 1))) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i += 2;

	selected = &(tile_button->selected);

	gui.add(back_ground);


	int j = 0;
	while (j < 6)
	{
		t_button *button = new t_button(new s_text_button(
			"Effect " + to_string(j + 1) + " :", DARK_GREY,
			t_vect(9.2, 1 + (1.2 * i)) * gui.unit, t_vect(5, 0.7) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
			NULL, NULL);
		gui.add(button);

		i+= 0.7;
		int count = 0;

		while (count < (int)(g_effects.size()) && g_effects[count] != spell->effect[j].effect)
			count++;
		if (count == (int)(g_effects.size()))
			count = -1;
		else
			count++;
		string tmp_text = (count != -1 ? list_action_name[count] : list_action_name[0]);
		button = new t_button(new s_text_button(
			tmp_text, DARK_GREY,
			t_vect(9.2, 1 + (1.2 * i)) * gui.unit, t_vect(8, 1) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
			menu_select_action, NULL);
		button->button->data_left = t_data(3, &gui, spell, text);
		text[j] = &(button->button->text);
		i += 1;
		gui.add(button);
		j++;
	}

	t_button	*test = new t_button(new s_text_button(
		"Edit spell's animation", DARK_GREY,
		t_vect(9.2, 1 + (1.2 * i)) * gui.unit,
		t_vect(8, 0.8) * gui.unit,
		5,
		t_color(0.4, 0.4, 0.4),
		t_color(0.6, 0.6, 0.6)),
		menu_edit_animation, t_data(2, &gui, spell));
	i += 0.9;


	t_iterator *price_iterator = new s_iterator(&(spell->price), NULL, 1, 1, 0, 15000,
		new t_button(new s_text_button(
			"Price: ", DARK_GREY,
			t_vect(9.2, 1.0 + (1.2 * (i))) * gui.unit, t_vect(4.25, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"-", DARK_GREY,
			t_vect(5.5 + 8.2, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"", DARK_GREY,
			t_vect(6.75 + 8.2, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL),
		new t_button(new s_text_button(
			"+", DARK_GREY,
			t_vect(8 + 8.2, 1.0 + (1.2 * (i))) * gui.unit, t_vect(1, 0.78) * gui.unit, 5,
			t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)), NULL, NULL));
	i += 0.81;


	gui.add(entry_path);
	gui.add(entry_name);
	gui.add(spell_type_iterator);
	gui.add(pa_iterator);
	gui.add(pm_iterator);
	gui.add(cooldown_iterator);
	gui.add(block_range_iterator);
	gui.add(range_type_iterator);
	gui.add(min_range_iterator);
	gui.add(max_range_iterator);
	gui.add(zone_type_iterator);
	gui.add(zone_size_iterator);
	gui.add(target_type_iterator);
	gui.add(save_button);
	gui.add(load_button);
	gui.add(delete_button);
	gui.add(new_button);
	gui.add(quit_button);
	gui.add(tileset_selector);
	gui.add(price_iterator);
	gui.add(card);
	gui.add(test);

	spell->tile = get_interface_tile("simple_iconset");
	while (play)
	{
		spell->name = entry_name->entry->text;
		spell->icon = tile_button->selected;
		prepare_screen();

		gui.draw_self();
		if ((size_t)(account->tuto_state) < gui_tutorial.size())
			gui_tutorial[account->tuto_state].draw_self();

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

	read_spell();
	account->recharge_actor();
}
