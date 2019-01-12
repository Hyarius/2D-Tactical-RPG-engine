#include "gui.h"

s_entry::s_entry(t_entry_comp *p_entry)
{
	this->entry = p_entry;
}


void			s_entry::draw_self()
{
	this->entry->draw_self();
}

bool			s_entry::click(t_vect mouse)
{
	if (this->entry->click(mouse) == true)
		return (true);
	return (false);
}
