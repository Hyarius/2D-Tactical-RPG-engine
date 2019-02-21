#include "gui.h"

s_button::s_button(t_button_comp *p_button, gui_funct p_funct, t_data p_data)
{
	int i = 0;

	this->button = p_button;
	this->button->set_funct_param(MOUSE_LEFT, p_funct, p_data);
}

s_button::s_button(t_button_comp *p_button, gui_funct p_funct, t_data p_data, d_funct p_draw_funct)
{
	int i = 0;

	this->button = p_button;
	this->button->set_funct_param(MOUSE_LEFT, p_funct, p_data, p_draw_funct);
}

void			s_button::draw_self()
{
	this->button->draw_self();
}

bool			s_button::click_left(t_vect mouse)
{
	if (this->button->click_left(mouse) == true)
		return (true);
	return (false);
}

bool			s_button::click_right(t_vect mouse)
{
	if (this->button->click_right(mouse) == true)
		return (true);
	return (false);
}

bool			s_button::key_press(SDL_Event *event)
{
	if (this->button->key_press(event) == true)
		return (true);
	return (false);
}
