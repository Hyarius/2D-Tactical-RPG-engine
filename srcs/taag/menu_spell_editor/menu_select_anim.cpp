#include "taag.h"

vector<string>		list_file_anim_select;
string 				*text_list_anim_select[30];

static void	modify_index(t_data data)
{
	int *index = (int *)(data.data[0]);
	int delta = (int &)(data.data[1]);

	if (*index + delta >= -27 && *index + delta < (int)(((list_file_anim_select.size() / 3) * 3 + 3)))
		*index += delta;
	for (int i = 0; i < 30; i++)
	{
		if (*index + i >= (int)(list_file_anim_select.size()) || *index + i < 0)
			*(text_list_anim_select[i]) = "";
		else
			*(text_list_anim_select[i]) = list_file_anim_select[i + *index];
	}
}

static void		quit_select_anim(t_data data)// spell / entry_name / entry_path / pool / play / num
{
	t_spell		*spell = (t_spell *)(data.data[0]);
	string		*entry_name = (string *)(data.data[1]);
	bool 		*play = (bool *)(data.data[2]);
	int			i = (int &)(data.data[3]);

	if (*(text_list_anim_select[i]) != "")
	{
		if (*(text_list_anim_select[i]) != "NULL")
			spell->target_anim.tile = get_animation_tile(*(text_list_anim_select[i]));
		else
			spell->target_anim.tile = NULL;
		*entry_name = *(text_list_anim_select[i]);
		*play = false;
	}
}

void			menu_select_anim(t_data data) // 0 - t_gui * / 1 - t_spell * / 2 - &name
{
	t_gui		*old_gui = (t_gui *)(data.data[0]);
	t_spell		*spell = (t_spell *)(data.data[1]);
	string		*entry_name = (string *)(data.data[2]);
	t_gui		gui;
	bool		play = true;
	SDL_Event	event;
	int 		index = 0;

	gui.add(new t_button(new t_text_button(
						"", BLACK,
						gui.unit * t_vect(1, 1), gui.unit * t_vect(28, 18), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						NULL, NULL));

	gui.add(new t_button(new t_text_button(
						"Which animation cheep do you want ?", BLACK,
						gui.unit * t_vect(1.5, 1.5), gui.unit * t_vect(27, 2), 8,
						t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)
						),
						NULL, NULL));

	list_file_anim_select.clear();
	list_file_anim_select.push_back("NULL");
	list_file_anim_select.insert(list_file_anim_select.end(), animation_name.begin(), animation_name.end());
	int i = 0;
	while (i < 30)
	{
		t_button *button = new t_button(new t_text_button(
					(i < (int)(list_file_anim_select.size()) ? list_file_anim_select[i] : ""), BLACK,
					gui.unit * t_vect(1.5 + ((i % 3) * 8.3 + (i % 3)), 4 + ((i / 3) * 1.3)),
					gui.unit * t_vect(8.3, 1.1),
					8,
					t_color(0.4, 0.4, 0.4),
					t_color(0.6, 0.6, 0.6)),
					quit_select_anim, t_data(4, spell, entry_name, &play, i));// spell / entry_name / entry_path / pool / play / num
		text_list_anim_select[i] = &(button->button->text);
		gui.add(button);
		i++;
	}

	while (play == true)
	{
		prepare_screen();

		if (old_gui != NULL)
			old_gui->draw_self();

		gui.draw_self();

		render_screen(true);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT)
				menu_quit(t_data(1, &gui));
			if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				play = false;
			else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				gui.click(&event);
			else if (event.type == SDL_TEXTINPUT || event.type == SDL_KEYDOWN)
				gui.key_press(&event);
			else if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_UP))
			{
				modify_index(t_data(2, &index, -3));
			}
			else if ((event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_DOWN))
			{
				modify_index(t_data(2, &index, 3));
			}
			else if (event.type == SDL_MOUSEWHEEL)
			{
				if (event.wheel.y > 0)
					modify_index(t_data(2, &index, -3));
				else if (event.wheel.y < 0)
					modify_index(t_data(2, &index, 3));
			}
		}
	}
}
