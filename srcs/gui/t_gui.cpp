#include "gui.h"

s_gui::s_gui()
{
	entry = NULL;
	unit = t_vect(get_win_size().x / 30, get_win_size().y / 20);
	tutorial_value = 0.0;
}

s_gui::s_gui(int x, int y)
{
	entry = NULL;
	unit = t_vect(get_win_size().x / x, get_win_size().y / y);
	tutorial_value = 0.0;
}

void		s_gui::draw_self()
{
	size_t i = 0;
	size_t j;

	while (i < object_list.size())
	{
		if (i == TUTORIAL_NUM && object_list[i].size() != 0)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
			glDepthFunc(GL_LESS);
		}
		j = 0;
		while (j < object_list[i].size())
		{
			object_list[i].at(j)->draw_self();
			j++;
		}
		if (i == TUTORIAL_NUM && object_list[i].size() != 0)
		{
			draw_rectangle(t_vect(0, 0), get_win_size(), t_color(0.2,0.2,0.2,tutorial_value));
			if (tutorial_value < 0.7)
				tutorial_value += 0.7 / 60.0;
			glClear(GL_DEPTH_BUFFER_BIT);
			glDepthFunc(GL_ALWAYS);
		}
		i++;
	}
}

bool		s_gui::click(SDL_Event *event)
{
	if (object_list.size() == 0)
		return (true);
	size_t i = 0;
	size_t j = 0;
	t_vect mouse = get_mouse_coord();

	while (object_list.size() >= ENTRY_NUM && i < object_list[ENTRY_NUM].size())
	{
		((t_entry *)(object_list[ENTRY_NUM][i]))->entry->selected = false;
		i++;
	}
	entry = NULL;
	SDL_StopTextInput();
	if (event->button.button == SDL_BUTTON_LEFT)
	{
		i = 0;
		while (i < object_list.size())
		{
			j = 0;
			while (j < object_list[i].size())
			{
				if (object_list[i].at(j)->click_left(mouse) == true)
						return (true);
				j++;
			}
			i++;
		}
	}
	else
	{
		i = 0;
		while (i < object_list.size())
		{
			j = 0;
			while (j < object_list[i].size())
			{
				if (object_list[i].at(j)->click_right(mouse) == true)
					return (true);
				j++;
			}
			i++;
		}
	}
	return (false);
}

bool		s_gui::key_press(SDL_Event *event)
{
	if (object_list.size() == 0)
		return (true);
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
