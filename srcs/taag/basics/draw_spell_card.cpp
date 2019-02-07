#include "taag.h"

void			draw_spell_card(t_spell *spell, t_vect coord, t_vect size)
{
	t_gui gui;
	(void)spell;
	t_vect unit = size / t_vect(40, 60); //unit inside the card

	t_vect tmp_coord = coord;//t_vect(0.9, 0.9);
	t_vect tmp_size = size;//t_vect(4.7, 7.2);

	draw_border_rectangle(coord, size, 8, t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6));

	draw_rectangle((coord + unit * t_vect(1, 1)), unit * t_vect(11, 11), t_color(0.4, 0.4, 0.4));
	if (spell->tile != NULL)
		spell->tile->draw_self((coord + unit * t_vect(1.5, 1.5)), unit * t_vect(10, 10), spell->icon);

	draw_border_rectangle((coord + unit * t_vect(13, 1)), unit * t_vect(26, 11), 8, t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6));

	if (spell->name != "NULL")
	{

	int text_size = calc_text_max_size("mmmmmmmm", unit * t_vect(12, 5.5));
	draw_centred_text(spell->name, text_size, (((coord + unit * t_vect(13, 1)) + unit * t_vect(26, 11) / 2)), BLACK);
	draw_border_rectangle((coord + unit * t_vect(1, 13)), unit * t_vect(12, 5.5), 8, t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6));
	draw_centred_text(to_string(spell->cost_pa) + " PA", text_size, (((coord + unit * t_vect(1, 13)) + unit * t_vect(12, 5.5) / 2)), BLACK);
	draw_border_rectangle((coord + unit * t_vect(14, 13)), unit * t_vect(12, 5.5), 8, t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6));
	draw_centred_text(to_string(spell->cost_pm) + " PM", text_size, (((coord + unit * t_vect(14, 13)) + unit * t_vect(12, 5.5) / 2)), BLACK);
	draw_border_rectangle((coord + unit * t_vect(27, 13)), unit * t_vect(12, 5.5), 8, t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6));
	draw_centred_text(to_string(spell->range[0]) + "-" + to_string(spell->range[1]) + " PO", text_size, (((coord + unit * t_vect(27, 13)) + unit * t_vect(12, 5.5) / 2)), BLACK);
	draw_border_rectangle((coord + unit * t_vect(1, 19.5)), unit * t_vect(38, 39.5), 8, t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6));
	draw_paragraphe(spell->desc, text_size, (coord + unit * t_vect(2, 20.5)), unit * t_vect(36.6, 38), BLACK);

	int i = 0;
	int tmp = (spell->zone_size != 0 ? (int)(spell->zone_type) : 4);
	interface_map["simple_cursor"].draw_self((coord + unit * t_vect(29.5, 49.5)), unit * t_vect(8.5, 8.5), t_vect(tmp, 5));
	interface_map["simple_cursor"].draw_self((coord + unit * t_vect(20, 49.5)), unit * t_vect(8.5, 8.5), t_vect((int)(spell->range_type) + 2, 4));
	interface_map["simple_cursor"].draw_self((coord + unit * t_vect(10.5, 49.5)), unit * t_vect(8.5, 8.5), t_vect((spell->block == false ? 1 : 0), 4));

	}
}
