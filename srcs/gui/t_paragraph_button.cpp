#include "gui.h"

s_paragraph_button::s_paragraph_button(	string p_text, int p_text_color, int p_text_size, //text info
								t_vect p_coord, t_vect p_size, int border, //object info
								t_color p_color1, t_color p_color2) // color info
{
	text_image = NULL;
	text = p_text;
	saved_text = "";
	text_color = p_text_color;
	text_size = p_text_size;
	coord[0] = p_coord;
	size[0] = p_size;
	coord[1] = p_coord + border;
	size[1] = p_size - (border * 2);
	coord[2] = coord[1] + size[1] / 2;
	text_image = NULL;
	color[0] = p_color1;
	color[1] = p_color2;
	funct_left = NULL;
	data_left = NULL;
	funct_right = NULL;
	data_right = NULL;
	draw_funct = draw_centred_text;
}

void		s_paragraph_button::draw_self()
{
	if (data_left.data.size() <= 1)
		glDepthFunc(GL_LEQUAL);
	for (int i = 0; i < 2; i++)
		draw_rectangle(coord[i], size[i], color[i]);
	if (text != saved_text)
	{
		saved_text = text;
		vector<string> lines = prepare_paragraphe(text, text_size, size[1] - (size[0] - size[1]));
		for (size_t j = 0; j < lines.size(); j++)
		{
			if (j < lines.size())
			{
				if (lines[j] != "" && lines[j] != "\n")
				{
					SDL_Surface *surface = TTF_RenderText_Blended(get_font(text_size), lines[j].c_str(), get_color(BLACK));
					if (surface == NULL)
						error_exit("Can't create the text for a t_image button with the string " + lines[j] + " at size " + to_string(text_size));
					text_paragraph.push_back(new t_image(surface));
					paragraph_size.push_back(t_vect(surface->w, surface->h));
					SDL_FreeSurface(surface);
				}
				else
				{
					text_paragraph.push_back(NULL);
					paragraph_size.push_back(t_vect(0, 0));
				}
			}
		}
	}
	for (size_t i = 0; i < text_paragraph.size(); i++)
	{
		t_vect image_coord = t_vect(coord[1] + t_vect(0, get_char(text_size, BLACK, 'M')->surface->h) * (int)(i)+(size[0] - size[1]));
		if (text_paragraph[i] != NULL)
			text_paragraph[i]->draw_self(image_coord, paragraph_size[i]);
	}
	if (data_left.data.size() <= 1)
		glDepthFunc(GL_LESS);
}
