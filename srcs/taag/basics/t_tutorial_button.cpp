#include "taag.h"

s_tutorial_button::s_tutorial_button()
{
	button = NULL;
	paragraph = NULL;
	allowed = false;
}

s_tutorial_button::s_tutorial_button(t_button *p_button, t_button *p_paragraph, bool p_allowed)
{
	button = p_button;
	paragraph = p_paragraph;
	allowed = p_allowed;
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
	if (paragraph != NULL && paragraph->button->funct_left == NULL && paragraph->button->check_click(mouse) == true)
	{
		if (allowed == true)
			increment_tutorial(NULL);
		return (true);
	}
	else if (button != NULL && button->button->funct_left != NULL && button->button->check_click(mouse) == true)
	{
		if (allowed == true)
			increment_tutorial(NULL);
		return (this->button->click_left(mouse));
	}
	return (false);
}

bool			s_tutorial_button::click_right(t_vect mouse)
{
	if ((paragraph != NULL && paragraph->button->check_click(mouse) == true && paragraph->button->funct_right == NULL))
	{
		increment_tutorial(NULL);
		return (true);
	}
	else if (button != NULL && button->button->funct_right != NULL)
	{
		if (button->button->check_click(mouse) == true && allowed == true)
			increment_tutorial(NULL);
		return (this->button->click_right(mouse));
	}
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
