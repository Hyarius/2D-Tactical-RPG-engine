#include "taag.h"

void			s_game_engine::draw_spell_card(t_spell *spell)
{
	t_vect coord = t_vect(0.9, 0.9);
	t_vect size = t_vect(4.7, 7.2);
	draw_border_rectangle(gui.unit * coord, gui.unit * size, 10, t_color(0.3, 0.3, 0.3, 0.5), t_color(0.6, 0.6, 0.6, 0.5));


	coord = t_vect(1.1, 1.1);
	draw_rectangle(gui.unit * coord, gui.unit * t_vect(1.2, 1.2), t_color(0.3, 0.3, 0.3, 0.5));
	coord = t_vect(1.2, 1.2);
	spell->tile->draw_self(gui.unit * coord, gui.unit, spell->icon);

	coord = t_vect(2.4, 1.1);
	size = t_vect(3, 1.2);
	draw_border_rectangle(gui.unit * coord, gui.unit * size, 10, t_color(0.3, 0.3, 0.3, 0.5), t_color(0.4, 0.4, 0.4, 0.5));
	static int text_size = calc_text_max_size("mmmmmmmmmmmm", gui.unit * t_vect(4.3, 0.5));
	draw_centred_text(spell->name, text_size, gui.unit * (coord + size / 2), BLACK);

	coord = t_vect(1.1, 2.4);
	size = t_vect(1.35, 0.7);
	draw_border_rectangle(gui.unit * coord, gui.unit * size, 10, t_color(0.3, 0.3, 0.3, 0.5), t_color(0.4, 0.4, 0.4, 0.5));
	draw_centred_text(to_string(spell->cost_pa) + " PA", text_size, gui.unit * (coord + size / 2), BLACK);

	coord = coord + t_vect(size.x + 0.1, 0);
	draw_border_rectangle(gui.unit * coord, gui.unit * size, 10, t_color(0.3, 0.3, 0.3, 0.5), t_color(0.4, 0.4, 0.4, 0.5));
	draw_centred_text(to_string(spell->cost_pm) + " PM", text_size, gui.unit * (coord + size / 2), BLACK);

	coord = coord + t_vect(size.x + 0.1, 0);
	draw_border_rectangle(gui.unit * coord, gui.unit * size, 10, t_color(0.3, 0.3, 0.3, 0.5), t_color(0.4, 0.4, 0.4, 0.5));
	draw_centred_text(to_string(spell->range[0]) + "-" + to_string(spell->range[1]) + " PO", text_size, gui.unit * (coord + size / 2), BLACK);

	coord = t_vect(1.1, 3.2);
	size = t_vect(4.3, 4.7);
	t_vect size2 = t_vect(4.2, 4.2);
	draw_border_rectangle(gui.unit * coord, gui.unit * size, 10, t_color(0.3, 0.3, 0.3, 0.5), t_color(0.4, 0.4, 0.4, 0.5));
	draw_paragraphe(spell->desc, text_size, gui.unit * (coord + 0.1), gui.unit * size2, BLACK);

	coord = t_vect(4.4, 6.9);
	size = t_vect(1, 1);
	int i = 0;
	int tmp = (spell->zone_size != 0 ? (int)(spell->zone_type) : 4);
	tileset_map["simple_cursor"].draw_self(gui.unit * coord, gui.unit * size, t_vect(tmp, 5));
	coord = coord - t_vect(size.x, 0);
	tileset_map["simple_cursor"].draw_self(gui.unit * coord, gui.unit * size, t_vect((int)(spell->range_type) + 2, 4));
	coord = coord - t_vect(size.x, 0);
	tileset_map["simple_cursor"].draw_self(gui.unit * coord, gui.unit * size, t_vect((spell->block == false ? 1 : 0), 4));
	coord = coord - t_vect(size.x, 0);
}
