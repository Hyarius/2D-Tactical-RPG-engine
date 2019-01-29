#include "taag.h"

s_spell_card::s_spell_card()
{
	button = NULL;
	spell = NULL;
}

s_spell_card::s_spell_card(t_spell **p_spell, t_vect p_coord, t_vect p_size, gui_funct p_funct, t_data p_data)
{
	spell = p_spell;
	button = new t_button(new t_text_button(
				"", DARK_GREY, // text info
				p_coord, p_size, 8, // coord / size info
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
				p_funct, p_data);
}

void			s_spell_card::draw_self()
{
	button->draw_self();
	draw_spell_card(*spell, button->button->coord[0], button->button->size[0]);
}

bool			s_spell_card::click(t_vect mouse)
{
	size_t i = 0;

	if (button != NULL && button->click(mouse) == true)
		return (true);
	return (false);
}

bool			s_spell_card::key_press(SDL_Event *event)
{
	if (button != NULL && button->key_press(event) == true)
		return (true);
	return (false);
}
