#include "gui.h"

void			t_entry_comp::add_text(string new_text)
{
	if (text.size() + new_text.size() < max_len)
		if (calc_text_len(text + new_text, text_size) <= size[1].x)
			text += new_text;
}

void			t_entry_comp::delete_text()
{
	if (text.size())
		text.pop_back();
}

bool			t_entry_comp::click_left(t_vect mouse)
{
	if (mouse.x > this->coord[0].x && mouse.x < this->coord[0].x + this->size[0].x &&
		mouse.y > this->coord[0].y && mouse.y < this->coord[0].y + this->size[0].y)
	{
		selected = true;
		SDL_StartTextInput();
		return (true);
	}
	selected = false;
	return (false);
}

bool			t_entry_comp::click_right(t_vect mouse)
{
	(void)mouse;
	selected = false;
	return (false);
}

bool			t_entry_comp::key_press(SDL_Event *event)
{
	if (selected == true)
	{
		if (event->type == SDL_TEXTINPUT)
		{
			add_text(event->text.text);
			return (true);
		}
		else if (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_BACKSPACE)
		{
			delete_text();
			return (true);
		}
	}
	return (false);
}

void			t_entry_comp::set_funct_param(int type, gui_funct p_funct, t_data p_data, d_funct p_draw_funct)
{
	(void)type;
	(void)p_funct;
	(void)p_data;
	(void)p_draw_funct;
}

void		t_entry_comp::set_back(string p_back)
{
	back = p_back;
}

void		t_entry_comp::set_front(string p_front)
{
	front = p_front;
}
