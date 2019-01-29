#include "taag.h"

s_spell_card::s_spell_card()
{
	button[0] = NULL;
	button[1] = NULL;
	button[2] = NULL;
	button[3] = NULL;
	spell = NULL;
}

s_spell_card::s_spell_card(t_spell *p_spell, t_vect p_coord, t_vect p_size, gui_funct p_funct, t_data p_data)
{
	t_button_comp	*button0 = NULL;
	t_button_comp	*button1 = NULL;
	t_button_comp	*button2 = NULL;
	t_button_comp	*button3 = NULL;

	t_vect			coord = p_coord;
	t_vect			size = p_size;

	spell = p_spell;

	button0 = new t_text_button("", DARK_GREY, //text info
						coord, size, 4, //object info
						t_color(0.5, 0.5, 0.5), t_color(0.6, 0.6, 0.6));
	coord = coord + 7;
	size = t_vect(size.x / 4, size.x / 4);
	button1 = new t_text_button("", DARK_GREY, //text info
						coord, size, 4, //object info
						t_color(0.5, 0.5, 0.5), t_color(0.6, 0.6, 0.6));

	coord = coord + t_vect(size.x + 4, 0);
	size = t_vect(p_size.x - size.x - 21, size.x);
	button2 = new t_text_button("", DARK_GREY, //text info
						coord, size, 4, //object info
						t_color(0.5, 0.5, 0.5), t_color(0.6, 0.6, 0.6));
	button2->text = (spell != NULL ? spell->name : "");
	coord = p_coord + t_vect(7, size.y + 10);
	size = t_vect(p_size.x - 14, p_size.y - size.y - 17);

	button3 = new t_text_button("", DARK_GREY, //text info
		coord, size, 4, //object info
		t_color(0.5, 0.5, 0.5), t_color(0.6, 0.6, 0.6));

	button[0] = button0;
	button[0]->set_funct_param(p_funct, p_data, NULL);
	button[1] = button1;
	button[2] = button2;
	button[3] = button3;

	desc_size = calc_text_max_size("mmmmm mmmm mmm", button2->size[1] / t_vect(1, 2));
}

void			s_spell_card::draw_self()
{
	size_t i = 0;

	while (i < 4)
	{
		if (button[i] != NULL)
			button[i]->draw_self();
		i++;
	}
	if (spell != NULL)
		spell->tile->draw_self(button[1]->coord[1], button[1]->size[1], spell->icon);
	draw_paragraphe(spell->desc, desc_size, button[3]->coord[1], button[3]->size[1], DARK_GREY);
}

bool			s_spell_card::click(t_vect mouse)
{
	size_t i = 0;

	while (i < 4)
	{
		if (button[i] != NULL && button[i]->click(mouse) == true)
			return (true);
		i++;
	}
	return (false);
}

bool			s_spell_card::key_press(SDL_Event *event)
{
	size_t i = 0;

	while (i < 4)
	{
		if (button[i] != NULL && button[i]->key_press(event) == true)
			return (true);
		i++;
	}
	return (false);
}
