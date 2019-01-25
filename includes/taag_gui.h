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

#endif
