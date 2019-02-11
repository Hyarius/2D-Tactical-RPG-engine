#include "taag.h"

s_animation::s_animation(t_tileset *p_tile, int p_start, int p_len, t_vect p_coord, t_vect p_size)
{
	tile = p_tile;
	start = p_start;
	len = p_len;
	index = 0;
	coord = p_coord;
	size = p_size;
}

void			s_animation::draw_self(t_vect target, t_vect offset, t_vect size)
{
	if (index <= len)
	{
		t_vect reel_size = size * this->size;
		t_vect reel_coord = (coord + target) * size + offset - reel_size / 2;
		tile->draw_self(reel_coord, reel_size, index + start);
		index += (double)len / 60.0;
	}
}
