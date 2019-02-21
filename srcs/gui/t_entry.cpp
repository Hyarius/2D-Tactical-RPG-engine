#include "gui.h"

s_entry::s_entry(t_entry_comp *p_entry)
{
	this->entry = p_entry;
}


void			s_entry::draw_self()
{
	this->entry->draw_self();
}

bool			s_entry::click_left(t_vect mouse)
{
	if (this->entry->click_left(mouse) == true)
		return (true);
	return (false);
}
bool			s_entry::click_right(t_vect mouse)
{
	if (this->entry->click_right(mouse) == true)
		return (true);
	return (false);
}

bool			s_entry::key_press(SDL_Event *event)
{
	if (this->entry->key_press(event) == true)
		return (true);
	return (false);
}
