#include "taag.h"

t_node empty_node = t_node();

s_cell::s_cell()
{
	coord = t_vect(-1, -1);
	node = &empty_node;
	actor = NULL;
	cursor = t_vect(0, 0);
	m_dist = 999;
}

s_cell::s_cell(t_vect p_coord, t_node *p_node)
{
	coord = p_coord;
	node = p_node;
	actor = NULL;
	cursor = t_vect(0, 0);
	m_dist = 999;
}

void					s_cell::draw_cell(t_vect target, t_vect offset, t_vect size)
{
	if (node != NULL && node->tile != NULL)
		node->tile->prepare_print((coord + target) * size + offset, size, node->sprite);
}

void					s_cell::draw_visual_info(t_vect target, t_vect offset, t_vect size, double zoom)
{
	if (visual_info.size())
	{
		for (size_t i = 0; i < visual_info.size(); i++)
		{
			visual_info[i].draw_self(target, offset, size, zoom);
			if (visual_info[i].index == (int)visual_info[i].text_coord.size())
				visual_info.erase(visual_info.begin() + i);
		}

	}
}
