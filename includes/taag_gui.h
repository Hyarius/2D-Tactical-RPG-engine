#ifndef ITERATOR_H
# define ITERATOR_H

#include "template.h"
# include "gui.h"

typedef struct	s_iterator : t_gui_obj
{
	int			value;
	int			*linked_value;
	t_button	*desc;
	t_button	*minus;
	t_button	*container;
	t_button	*plus;
				s_iterator();
				s_iterator(int base_value, int *pool, int delta, int pool_cost, int min, int max,
							t_button *p_desc, t_button *p_minus, t_button *p_contain, t_button *p_plus);
				s_iterator(int *p_linked_value, int *pool, int delta, int pool_cost, int min, int max,
							t_button *p_desc, t_button *p_minus, t_button *p_contain, t_button *p_plus);
	void		draw_self();
	bool		click(t_vect mouse);
	bool		key_press(SDL_Event *event);
}				t_iterator;

typedef struct	s_sprite_iterator : t_gui_obj
{
	size_t		i;		//the key to get the correct sprite
	t_vect		delta;	//describ the way we run throught the tileset
	t_button	*desc;
	t_button	*minus;
	t_button	*container;
	t_button	*plus;
				s_sprite_iterator();
				s_sprite_iterator(t_vect p_delta, t_button *p_desc, t_button *p_minus, t_button *p_contain, t_button *p_plus);
	void		draw_self();
	bool		click(t_vect mouse);
	bool		key_press(SDL_Event *event);
}				t_sprite_iterator;

typedef struct	s_tileset_iterator : t_gui_obj
{
	size_t		i;		//the key to get the correct sprite
	t_vect		delta;	//describ the way we run throught the tileset
	int			change;	//describ if we need to checkout the tile before printing it;
	t_button	*desc;
	t_button	*minus;
	t_button	*container;
	t_button	*plus;
				s_tileset_iterator();
				s_tileset_iterator(t_vect p_delta, t_button *p_desc, t_button *p_minus, t_button *p_contain, t_button *p_plus);
	void		draw_self();
	bool		click(t_vect mouse);
	bool		key_press(SDL_Event *event);
}				t_tileset_iterator;

typedef struct		s_spell_card : t_gui_obj
{
	t_button		*button; //0 - background | 1 - image | 2 - name | 3 - desc
	struct s_spell	**spell;		//spell of the card

					s_spell_card();
					s_spell_card(struct s_spell **p_spell, t_vect coord, t_vect size, gui_funct p_funct, t_data p_data);
	void			draw_self();
	bool			click(t_vect mouse);
	bool			key_press(SDL_Event *event);
}					t_spell_card;

#endif
