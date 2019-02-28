#include "gui.h"

void		s_button_comp::set_text(string p_text)
{
	text = p_text;

	if (text != "")
		text_size = calc_text_size(text, size[1] - (coord[0].x - coord[1].x) * 2);
	else
		text_size = -1;
}

void		s_button_comp::set_funct_param(int type, gui_funct p_funct, t_data p_data, d_funct p_draw_funct)
{
	if (type == MOUSE_LEFT)
	{
		this->funct_left = p_funct;
		this->data_left = p_data;
	}
	if (type == MOUSE_RIGHT)
	{
		this->funct_right = p_funct;
		this->data_right = p_data;
	}
	if (p_draw_funct != NULL)
		this->draw_funct = p_draw_funct;
}

void		s_button_comp::set_funct_param(int type, gui_funct p_funct, t_data p_data)
{
	if (type == MOUSE_LEFT)
	{
		this->funct_left = p_funct;
		this->data_left = p_data;
	}
	if (type == MOUSE_RIGHT)
	{
		this->funct_right = p_funct;
		this->data_right = p_data;
	}
}

bool		s_button_comp::check_click(t_vect mouse)
{
	if (mouse.x > this->coord[0].x && mouse.x < this->coord[0].x + this->size[0].x &&
		mouse.y > this->coord[0].y && mouse.y < this->coord[0].y + this->size[0].y)
			return (true);
	return (false);
}

bool		s_button_comp::click_left(t_vect mouse)
{
	if (mouse.x > this->coord[0].x && mouse.x < this->coord[0].x + this->size[0].x &&
		mouse.y > this->coord[0].y && mouse.y < this->coord[0].y + this->size[0].y)
		if (this->funct_left != NULL)
		{
			this->funct_left(this->data_left);
			return (true);
		}
	return (false);
}

bool		s_button_comp::click_right(t_vect mouse)
{
	if (mouse.x > this->coord[0].x && mouse.x < this->coord[0].x + this->size[0].x &&
		mouse.y > this->coord[0].y && mouse.y < this->coord[0].y + this->size[0].y)
		if (this->funct_right != NULL)
		{
			this->funct_right(this->data_right);
			return (true);
		}
	return (false);
}

bool		s_button_comp::key_press(SDL_Event *event)
{
	(void)event;
	return (false);
}
