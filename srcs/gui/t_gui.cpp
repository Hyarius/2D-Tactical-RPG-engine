#include "gui.h"

s_gui::s_gui()
{
	entry = NULL;
	unit = t_vect(get_win_size().x / 30, get_win_size().y / 20);
	object_list.resize(5);
}

s_gui::s_gui(int x, int y)
{
	entry = NULL;
	unit = t_vect(get_win_size().x / x, get_win_size().y / y);
	object_list.resize(5);
}

void		s_gui::draw_self()
{
	size_t i = 0;
	size_t j;

	while (i < object_list.size())
	{
		j = 0;
		while (j < object_list[i].size())
		{
			object_list[i].at(j)->draw_self();
			j++;
		}
		i++;
	}
}

bool		s_gui::click(SDL_Event *event)
{
	size_t i = 0;
	size_t j = 0;
	t_vect mouse = get_mouse_coord();

	while (i < object_list[ENTRY_NUM].size())
	{
		((t_entry *)(object_list[ENTRY_NUM][i]))->entry->selected = false;
		i++;
	}
	entry = NULL;
	SDL_StopTextInput();
	i = 0;
	while (i < object_list.size())
	{
		j = 0;
		while (j < object_list[i].size())
		{
			if (event->button.button == SDL_BUTTON_LEFT)
			{
				if (object_list[i].at(j)->click_left(mouse) == true)
						return (true);
			}
			else if (event->button.button == SDL_BUTTON_RIGHT)
			{
				if (object_list[i].at(j)->click_right(mouse) == true)
						return (true);
			}
			j++;
		}
		i++;
	}
	return (false);
}

bool		s_gui::key_press(SDL_Event *event)
{
	size_t i = 0;
	size_t j = 0;

	if (event->type == SDL_TEXTINPUT || (event->type == SDL_KEYDOWN && event->key.keysym.sym == SDLK_BACKSPACE))
	{
		i = 0;
		while (i < object_list.size())
		{
			j = 0;
			while (j < object_list[i].size())
			{
				if (object_list[i].at(j)->key_press(event) == true)
					return (true);
				j++;
			}
			i++;
		}
	}
	return (false);
}

void		s_gui::add(int rep, t_gui_obj *object)
{
	if (object_list.size() < (size_t)(rep + 1))
		object_list.resize(rep + 1);
	object_list[rep].push_back(object);
}

void		s_gui::add(t_gui_obj *object)
{
	if (object_list.size() < 1)
		object_list.resize(1);
	object_list[0].push_back(object);
}
