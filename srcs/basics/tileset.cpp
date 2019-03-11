#include "template.h"

s_tileset::s_tileset()
{
	surface = NULL;
}

s_tileset::s_tileset(string p_path, t_vect p_size)
{
	surface = IMG_Load(p_path.c_str());
	if (surface == NULL)
		error_exit("Can't load " + p_path + " to create a t_tileset");
	if (surface->format->BytesPerPixel == 3)
	{
		internal_format = GL_RGB;
		if (surface->format->Rmask == 0xff)
			format = GL_RGB;
		else
			format = GL_BGR;
	}
	else if (surface->format->BytesPerPixel == 4)
	{
		internal_format = GL_RGBA;
		if (surface->format->Rmask == 0xff)
			format = GL_RGBA;
		else
			format = GL_BGRA;
	}
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, surface->w,
				surface->h, 0, format,
				GL_UNSIGNED_BYTE, surface->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	nb_sprite = p_size;
	size = t_vect(1.0 / p_size.x, 1.0 / p_size.y);
	t_vect	tmp = t_vect(0, 0);

	while (tmp.x * tmp.y < p_size.x * (p_size.y - 1))
	{
		if (tmp.x >= p_size.x)
		{
			tmp.x = 0;
			tmp.y++;
		}
		sprite_list.push_back(t_vect(tmp.x / p_size.x, tmp.y / p_size.y));
		tmp.x++;
	}
}

int		s_tileset::get_sprite(t_vect p_sprite)
{
	int value = (int)(p_sprite.y) * (int)(nb_sprite.x) + (int)(p_sprite.x);

	if (value >= nb_sprite.x * nb_sprite.y || value < 0)
		return (0);
	return (value);
}

void	s_tileset::draw_self(t_vect p_tl, t_vect p_tr, t_vect p_dl, t_vect p_dr, int p_sprite, double p_alpha)
{
	if (surface == NULL)
		return ;
	glBindTexture(GL_TEXTURE_2D, texture_id);
	t_point		tl = screen_to_opengl(p_tl);
	t_point		tr = screen_to_opengl(p_tr);
	t_point		dl = screen_to_opengl(p_dl);
	t_point		dr = screen_to_opengl(p_dr);

	int			i;
	if ((unsigned int)p_sprite >= sprite_list.size())
		i = 0;
	else
		i = p_sprite;

	t_vect	sprite_coord = t_vect(sprite_list[i]);

	draw_triangle_texture(	t_point(tl.x, tl.y, sprite_coord.x, sprite_coord.y, p_alpha),
							t_point(tr.x, tr.y, sprite_coord.x + size.x, sprite_coord.y, p_alpha),
							t_point(dl.x, dl.y, sprite_coord.x, sprite_coord.y + size.y, p_alpha));

	draw_triangle_texture(	t_point(dl.x, dl.y, sprite_coord.x, sprite_coord.y + size.y, p_alpha),
							t_point(dr.x, dr.y, sprite_coord.x + size.x, sprite_coord.y + size.y, p_alpha),
							t_point(tr.x, tr.y, sprite_coord.x + size.x, sprite_coord.y, p_alpha));
}


void 			s_tileset::draw_self(t_vect p_coord, t_vect p_size, int p_sprite)
{
	draw_self(p_coord, p_coord + t_vect(p_size.x, 0), p_coord + t_vect(0, p_size.y), p_coord + p_size, p_sprite, 1.0);
}


void 			s_tileset::draw_self(t_vect p_coord, t_vect p_size, int p_sprite, double p_alpha)
{
	draw_self(p_coord, p_coord + t_vect(p_size.x, 0), p_coord + t_vect(0, p_size.y), p_coord + p_size, p_sprite, p_alpha);
}

void	s_tileset::draw_self(t_vect p_tl, t_vect p_tr, t_vect p_dl, t_vect p_dr, t_vect p_sprite, double p_alpha)
{
	int value = (int)(p_sprite.y) * (int)(nb_sprite.x) + (int)(p_sprite.x);
	draw_self(p_tl, p_tr, p_dl, p_dr, value, p_alpha);
}


void 			s_tileset::draw_self(t_vect p_coord, t_vect p_size, t_vect p_sprite)
{
	if (p_sprite != t_vect(-1, -1))
	{
		int value = (int)(p_sprite.y) * (int)(nb_sprite.x) + (int)(p_sprite.x);
		draw_self(p_coord, p_coord + t_vect(p_size.x, 0), p_coord + t_vect(0, p_size.y), p_coord + p_size, value, 1.0);
	}

}


void 			s_tileset::draw_self(t_vect p_coord, t_vect p_size, t_vect p_sprite, double p_alpha)
{
	if (p_sprite != t_vect(-1, -1))
	{
		int value = (int)(p_sprite.y) * (int)(nb_sprite.x) + (int)(p_sprite.x);
		draw_self(p_coord, p_coord + t_vect(p_size.x, 0), p_coord + t_vect(0, p_size.y), p_coord + p_size, value, p_alpha);
	}
}

void			s_tileset::prepare_print(t_vect p_tl, t_vect p_tr, t_vect p_dl, t_vect p_dr, int p_sprite, double p_alpha)
{
	t_point		tl = screen_to_opengl(p_tl);
	t_point		tr = screen_to_opengl(p_tr);
	t_point		dl = screen_to_opengl(p_dl);
	t_point		dr = screen_to_opengl(p_dr);

	int			i;
	if ((unsigned int)p_sprite >= sprite_list.size())
		i = 0;
	else
		i = p_sprite;

	t_vect	sprite_coord = t_vect(sprite_list[i]);

	add_triangle_texture(	t_point(tl.x, tl.y, sprite_coord.x, sprite_coord.y, p_alpha),
							t_point(tr.x, tr.y, sprite_coord.x + size.x, sprite_coord.y, p_alpha),
							t_point(dl.x, dl.y, sprite_coord.x, sprite_coord.y + size.y, p_alpha));

	add_triangle_texture(	t_point(dl.x, dl.y, sprite_coord.x, sprite_coord.y + size.y, p_alpha),
							t_point(dr.x, dr.y, sprite_coord.x + size.x, sprite_coord.y + size.y, p_alpha),
							t_point(tr.x, tr.y, sprite_coord.x + size.x, sprite_coord.y, p_alpha));
}

void			s_tileset::prepare_print(t_vect p_coord, t_vect p_size, int p_sprite, double p_alpha)
{
	prepare_print(p_coord, p_coord + t_vect(p_size.x, 0), p_coord + t_vect(0, p_size.y), p_coord + p_size, p_sprite, p_alpha);
}

void			s_tileset::prepare_print(t_vect p_coord, t_vect p_size, int p_sprite)
{
	prepare_print(p_coord, p_coord + t_vect(p_size.x, 0), p_coord + t_vect(0, p_size.y), p_coord + p_size, p_sprite, 1.0);
}

void			s_tileset::prepare_print(t_vect p_tl, t_vect p_tr, t_vect p_dl, t_vect p_dr, t_vect p_sprite, double p_alpha)
{
	int value = (int)(p_sprite.y) * (int)(nb_sprite.x) + (int)(p_sprite.x);
	prepare_print(p_tl, p_tr, p_dl, p_dr, value, p_alpha);
}

void			s_tileset::prepare_print(t_vect p_coord, t_vect p_size, t_vect p_sprite, double p_alpha)
{
	int value = (int)(p_sprite.y) * (int)(nb_sprite.x) + (int)(p_sprite.x);
	prepare_print(p_coord, p_coord + t_vect(p_size.x, 0), p_coord + t_vect(0, p_size.y), p_coord + p_size, value, p_alpha);
}

void			s_tileset::prepare_print(t_vect p_coord, t_vect p_size, t_vect p_sprite)
{
	int value = (int)(p_sprite.y) * (int)(nb_sprite.x) + (int)(p_sprite.x);
	prepare_print(p_coord, p_coord + t_vect(p_size.x, 0), p_coord + t_vect(0, p_size.y), p_coord + p_size, value, 1.0);
}
