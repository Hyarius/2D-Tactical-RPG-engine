#include "template.h"

void			draw_image(t_vect tl, t_vect tr, t_vect dl, t_vect dr, double p_alpha)
{
	t_point		tl_opengl = screen_to_opengl(tl);
	t_point		tr_opengl = screen_to_opengl(tr);
	t_point		dl_opengl = screen_to_opengl(dl);
	t_point		dr_opengl = screen_to_opengl(dr);

	draw_triangle_texture(t_point(tl_opengl.x, tl_opengl.y, 0.0, 0.0, p_alpha),
		t_point(tr_opengl.x, tr_opengl.y, 1.0, 0.0, p_alpha),
		t_point(dl_opengl.x, dl_opengl.y, 0.0, 1.0, p_alpha));

	draw_triangle_texture(t_point(dl_opengl.x, dl_opengl.y, 0.0, 1.0, p_alpha),
		t_point(dr_opengl.x, dr_opengl.y, 1.0, 1.0, p_alpha),
		t_point(tr_opengl.x, tr_opengl.y, 1.0, 0.0, p_alpha));
}

void			draw_image(t_vect coord, t_vect size)
{
	draw_image(	t_vect(coord.x, coord.y),
				t_vect(coord.x + size.x, coord.y),
				t_vect(coord.x, coord.y + size.y),
				t_vect(coord.x + size.x, coord.y + size.y), 1.0);
}

void			draw_image(t_vect coord, t_vect size, double p_alpha)
{
	draw_image(	t_vect(coord.x, coord.y),
				t_vect(coord.x + size.x, coord.y),
				t_vect(coord.x, coord.y + size.y),
				t_vect(coord.x + size.x, coord.y + size.y), p_alpha);
}
