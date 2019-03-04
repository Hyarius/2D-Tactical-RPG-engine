#include "taag.h"

s_spell_card::s_spell_card()
{
	button = NULL;
	spell = NULL;
}
/*
vector<string>	text;
vector<string>	saved_text;
vector<t_image *>
				text_image;
vector<t_vect>	image_coord;
vector<t_vect>	image_size;
*/

s_spell_card::s_spell_card(t_spell **p_spell, t_vect p_coord, t_vect p_size, gui_funct p_funct, t_data p_data)
{
	spell = p_spell;
	border = p_size.x / 70.0;
	button = new t_button(new t_text_button(
				"", DARK_GREY, // text info
				p_coord, p_size, border, // coord / size info
				t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6)),
				p_funct, p_data);
	coord = p_coord;
	size = p_size;
	unit = p_size / t_vect(40, 60);
	text.resize(15);
	text_image.resize(15);
	image_coord.resize(15);
	image_size.resize(15);
	text_size = calc_text_max_size("mmmmmm", unit * t_vect(12, 5.5));
	text_coord.resize(15);
	text_coord[0] = (((coord + unit * t_vect(13, 1)) + unit * t_vect(26, 11) / 2));
	text_coord[1] = (((coord + unit * t_vect(1, 13)) + unit * t_vect(12, 5.5) / 2));
	text_coord[2] = (((coord + unit * t_vect(14, 13)) + unit * t_vect(12, 5.5) / 2));
	text_coord[3] = (((coord + unit * t_vect(27, 13)) + unit * t_vect(12, 5.5) / 2));
	text_coord[4] = (((coord + unit * t_vect(14, 43)) + unit * t_vect(12, 5.5) / 2));
	paragraphe_size = unit * t_vect(36, 22);
	text_coord[5] = (coord + unit * t_vect(2, 20.5));
	for (size_t i = 6; i < 15; i++)
	{
		text_coord[i] = (text_coord[i - 1] + t_vect(0, get_char(text_size, BLACK, 'M')->surface->h));
	}

}

void			s_spell_card::draw_self()
{
	button->draw_self();

	draw_border_rectangle((coord + unit * t_vect(1, 1)), unit * t_vect(11, 11), border, t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6));
	if (spell != NULL && (*spell)->tile != NULL)
		(*spell)->tile->draw_self((coord + unit * t_vect(1, 1)), unit * t_vect(11, 11), (*spell)->icon);
	draw_border_rectangle((coord + unit * t_vect(13, 1)), unit * t_vect(26, 11), border, t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6));

	if ((*spell)->name != "NULL")
	{
		draw_border_rectangle((coord + unit * t_vect(1, 13)), unit * t_vect(12, 5.5), border, t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6));
		draw_border_rectangle((coord + unit * t_vect(14, 13)), unit * t_vect(12, 5.5), border, t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6));
		draw_border_rectangle((coord + unit * t_vect(27, 13)), unit * t_vect(12, 5.5), border, t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6));
		draw_border_rectangle((coord + unit * t_vect(1, 19.5)), unit * t_vect(38, 39.5), border, t_color(0.4, 0.4, 0.4), t_color(0.6, 0.6, 0.6));
		vector<string> actual;
		actual.push_back((*spell)->name);
		actual.push_back(to_string((*spell)->cost_pa) + " PA");
		actual.push_back(to_string((*spell)->cost_pm) + " PM");
		actual.push_back(to_string((*spell)->range[0]) + "-" + to_string((*spell)->range[1]) + " PO");
		actual.push_back(((*spell)->cooldown != 0 ? "(cooldown : " + to_string((*spell)->cooldown) + " turns)" : " "));
		actual.push_back((*spell)->desc);
		for (size_t i = 0; i < actual.size(); i++)
		{
			if (text[i] != actual[i] && i != 5 && actual[i] != "")
			{
				SDL_Surface *surface = TTF_RenderText_Blended(get_font(text_size), actual[i].c_str(), get_color(BLACK));
				if (surface == NULL)
					error_exit();
				text[i] = actual[i];
				text_image[i] = new t_image(surface);
				image_coord[i] = t_vect(text_coord[i].x, text_coord[i].y);
				image_size[i] = t_vect(surface->w, surface->h);
				SDL_FreeSurface(surface);
			}
			else if (text[i] != actual[i] && i == 5 && actual[i] != "")
			{
				text[i] = actual[i];
				vector<string> lines = prepare_paragraphe(text[i], text_size, paragraphe_size);
				for (size_t j = i; j < text_image.size(); j++)
				{
					text_image[j] = NULL;
					if (j - i < lines.size())
					{
						SDL_Surface *surface = TTF_RenderText_Blended(get_font(text_size), lines[j - i].c_str(), get_color(BLACK));
						if (surface == NULL)
							error_exit();
						text_image[j] = new t_image(surface);
						image_coord[j] = t_vect(text_coord[j].x + surface->w / 2, text_coord[j].y + surface->h / 2);
						image_size[j] = t_vect(surface->w, surface->h);
						SDL_FreeSurface(surface);
					}
				}
			}
		}
		for (size_t i = 0; i < text_image.size(); i++)
		{
			if (text_image[i] != NULL)
				text_image[i]->draw_centred_self(image_coord[i], image_size[i]);
		}
		int tmp = ((*spell)->zone_size != 0 ? (*spell)->zone_type : 4);
		get_interface_tile("simple_cursor")->draw_self((coord + unit * t_vect(29.5, 49.5)), unit * t_vect(8.5, 8.5), t_vect(tmp, 5));
		if ((int)((*spell)->zone_size) != 0)
			draw_centred_text(to_string((int)((*spell)->zone_size)), text_size, NORMAL, 4, (coord + unit * t_vect(30.5, 50.5)), LIGHT_GREY, BLACK);
		t_vect tmp2 = ((*spell)->range[1] != 0 ? t_vect((int)((*spell)->range_type) + 2, 4) : t_vect(4, 4));
		get_interface_tile("simple_cursor")->draw_self((coord + unit * t_vect(21.5, 49.5)), unit * t_vect(8.5, 8.5), tmp2);
		get_interface_tile("simple_cursor")->draw_self((coord + unit * t_vect(12.5, 49.5)), unit * t_vect(8.5, 8.5), t_vect((*spell)->block, 4));
		get_interface_tile("simple_cursor")->draw_self((coord + unit * t_vect(4.5, 49.5)), unit * t_vect(8.5, 8.5), t_vect(2 + (*spell)->on_target, 3));
	}
}

bool			s_spell_card::click_left(t_vect mouse)
{
	size_t i = 0;

	if (button != NULL && button->click_left(mouse) == true)
		return (true);
	return (false);
}

bool			s_spell_card::click_right(t_vect mouse)
{
	size_t i = 0;

	if (button != NULL && button->click_left(mouse) == true)
		return (true);
	return (false);
}

bool			s_spell_card::key_press(SDL_Event *event)
{
	if (button != NULL && button->key_press(event) == true)
		return (true);
	return (false);
}
