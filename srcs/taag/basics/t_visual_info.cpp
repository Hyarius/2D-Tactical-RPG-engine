#include "taag.h"

s_visual_info::s_visual_info()
{
	text = "";
	text_color = BLACK;
	text_size = 0;
	index = -1;
	text_coord.resize(0);
}

s_visual_info::s_visual_info(string p_text, int p_text_color, int p_text_size, vector<t_vect> p_text_coord)
{
	text = p_text;
	text_color = p_text_color;
	text_size = p_text_size;
	index = 0;
	text_coord = p_text_coord;
}

s_visual_info::s_visual_info(string p_text, int p_text_color, int p_text_size, int p_index, vector<t_vect> p_text_coord)
{
	text = p_text;
	text_color = p_text_color;
	text_size = p_text_size;
	index = p_index;
	text_coord = p_text_coord;
}

void					s_visual_info::draw_self(t_vect target, t_vect offset, t_vect size, double zoom)
{
	if (index < (int)(text_coord.size()))
	{
		if (index >= 0)
			draw_centred_text(text, text_size * zoom, BOLD, 4, (text_coord[index] + target) * size + offset, text_color, BLACK);
		index++;
	}
}

t_visual_info			create_visual_info(string p_text, int p_text_color, int p_text_size, t_vect p_starting_coord)
{
	vector<t_vect>	text_coord;

	for (int i = 0; i < 25; i++)
		text_coord.push_back(t_vect(p_starting_coord.x + 0.5, p_starting_coord.y - 0.2 - 0.05 * i / 2));
	for (int i = 25; i; i--)
		text_coord.push_back(t_vect(p_starting_coord.x + 0.5, p_starting_coord.y - 0.2 - 0.05 * i / 2));
	for (int i = 10; i; i--)
		text_coord.push_back(t_vect(p_starting_coord.x + 0.5, p_starting_coord.y - 0.2));

	t_visual_info	result = t_visual_info(p_text, p_text_color, p_text_size, text_coord);

	return (result);
}
