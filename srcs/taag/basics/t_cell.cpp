#include "taag.h"

s_cell::s_cell()
{
	coord = t_vect(-1, -1);
	node = NULL;
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
		node->tile->draw_self((coord + target) * size + offset, size, node->sprite);
}
