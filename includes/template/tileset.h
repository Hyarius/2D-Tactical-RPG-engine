#ifndef TILESET_H
# define TILESET_H

#include "template_structure.h"

typedef struct		s_tileset
{
	SDL_Surface		*surface;
	GLenum			format;
	GLint			internal_format;
	GLuint			texture_id;
	t_vect			nb_sprite;
	t_vect			size;
	vector<t_vect>	sprite_list;
					s_tileset();
					s_tileset(string p_path, t_vect p_size);
	int				get_sprite(t_vect p_sprite);
	void			prepare_print(t_vect p_tl, t_vect p_tr, t_vect p_dl, t_vect p_dr, int p_sprite, double p_alpha);
	void			prepare_print(t_vect p_coord, t_vect p_size, int p_sprite, double p_alpha);
	void			prepare_print(t_vect p_coord, t_vect p_size, int p_sprite);
	void			prepare_print(t_vect p_tl, t_vect p_tr, t_vect p_dl, t_vect p_dr, t_vect p_sprite, double p_alpha);
	void			prepare_print(t_vect p_coord, t_vect p_size, t_vect p_sprite, double p_alpha);
	void			prepare_print(t_vect p_coord, t_vect p_size, t_vect p_sprite);
	void			draw_self(t_vect p_tl, t_vect p_tr, t_vect p_dl, t_vect p_dr, int p_sprite, double p_alpha);
	void 			draw_self(t_vect p_coord, t_vect p_size, int p_sprite, double p_alpha);
	void 			draw_self(t_vect p_coord, t_vect p_size, int p_sprite);
	void			draw_self(t_vect p_tl, t_vect p_tr, t_vect p_dl, t_vect p_dr, t_vect p_sprite, double p_alpha);
	void 			draw_self(t_vect p_coord, t_vect p_size, t_vect p_sprite);
	void 			draw_self(t_vect p_coord, t_vect p_size, t_vect p_sprite, double p_alpha);
}					t_tileset;

#endif
