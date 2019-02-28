#include "taag.h"

static void			return_true(t_data data)
{
	(void)data;
}

s_tutorial_button::s_tutorial_button()
{
	button = NULL;
}

s_tutorial_button::s_tutorial_button(t_button *p_button, t_button *p_paragraph)
{
	button = p_button;
	paragraph = p_paragraph;
	if (button != NULL && button->button->funct_left == NULL)
		button->button->funct_left = return_true;
}

void			s_tutorial_button::draw_self()
{
	if (button != NULL)
		draw_rectangle(button->button->coord[0], button->button->size[0], t_color(0,0,0,0));
	if (paragraph != NULL)
		paragraph->draw_self();
}

bool			s_tutorial_button::click_left(t_vect mouse)
{
	if (this->button == NULL || this->button->button->funct_left == NULL)
		return (true);
	if (this->button->click_left(mouse) == true)
		return (true);
	return (false);
}

bool			s_tutorial_button::click_right(t_vect mouse)
{
	if (this->button == NULL || this->button->button->funct_right == NULL)
		return (true);
	if (this->button->click_right(mouse) == true)
		return (true);
	return (false);
}

bool			s_tutorial_button::key_press(SDL_Event *event)
{
	if (this->button == NULL)
		return (true);
	if (this->button->key_press(event) == true)
		return (true);
	return (false);
}
