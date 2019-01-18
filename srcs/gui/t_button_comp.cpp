#include "gui.h"

void		s_button_comp::set_text(string p_text)
{
	text = p_text;

	if (text != "")
		text_size = calc_text_size(text, size[1] - (coord[0].x - coord[1].x) * 2);
	else
		text_size = -1;
}

void		s_button_comp::set_funct_param(gui_funct p_funct, t_data p_data, d_funct p_draw_funct)
{
	this->funct = p_funct;
	this->data = p_data;
	if (p_draw_funct != NULL)
		this->draw_funct = p_draw_funct;
}

void		s_button_comp::set_funct_param(gui_funct p_funct, t_data p_data)
{
	this->funct = p_funct;
	this->data = p_data;
}

bool		s_button_comp::click(t_vect mouse)
{
	if (mouse.x > this->coord[0].x && mouse.x < this->coord[0].x + this->size[0].x &&
		mouse.y > this->coord[0].y && mouse.y < this->coord[0].y + this->size[0].y)
		if (this->funct != NULL)
		{
			this->funct(this->data);
			return (true);
		}
	return (false);
}
