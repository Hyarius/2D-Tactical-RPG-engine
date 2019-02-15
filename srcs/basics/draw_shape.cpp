#include "template.h"

void			draw_rectangle(t_vect p_tl, t_vect p_tr, t_vect p_dl, t_vect p_dr, t_color c)
{
	t_point		tl = screen_to_opengl(p_tl);
	t_point		tr = screen_to_opengl(p_tr);
	t_point		dl = screen_to_opengl(p_dl);
	t_point		dr = screen_to_opengl(p_dr);

	draw_triangle_color(t_point(tl.x, tl.y, c.r, c.g, c.b, c.a),
						t_point(tr.x, tr.y, c.r, c.g, c.b, c.a),
						t_point(dl.x, dl.y, c.r, c.g, c.b, c.a));

	draw_triangle_color(t_point(dl.x, dl.y, c.r, c.g, c.b, c.a),
						t_point(dr.x, dr.y, c.r, c.g, c.b, c.a),
						t_point(tr.x, tr.y, c.r, c.g, c.b, c.a));
}

void			draw_rectangle(t_vect p_coord, t_vect p_size, t_color c)
{
	draw_rectangle(t_vect(p_coord.x, p_coord.y), t_vect(p_coord.x + p_size.x, p_coord.y), t_vect(p_coord.x, p_coord.y + p_size.y), t_vect(p_coord.x + p_size.x, p_coord.y + p_size.y), c);
}

void			draw_centred_rectangle(t_vect p_coord, t_vect p_size, t_color c)
{
	draw_rectangle(p_coord - p_size / 2, p_size, c);
}

void			draw_border_rectangle(t_vect p_coord, t_vect p_size, int border, t_color c, t_color c2)
{
	draw_rectangle(p_coord, p_size, c);
	draw_rectangle(p_coord + border, p_size - (border * 2), c2);
}
