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
	bool		click_left(t_vect mouse);
	bool		click_right(t_vect mouse);
	bool		key_press(SDL_Event *event);
}				t_iterator;

void		increment_iterator(t_data data);

typedef struct	s_sprite_iterator : t_gui_obj
{
	size_t		i;		//the key to get the correct sprite
	t_vect		delta;	//describ the way we run throught the tileset
	vector<string>	*vector_name;
	t_button	*desc;
	t_button	*minus;
	t_button	*container;
	t_button	*plus;
				s_sprite_iterator();
				s_sprite_iterator(t_vect p_delta, vector<string> *p_vector_name, t_button *p_desc, t_button *p_minus, t_button *p_contain, t_button *p_plus);
	void		draw_self();
	bool		click_left(t_vect mouse);
	bool		click_right(t_vect mouse);
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
	bool		click_left(t_vect mouse);
	bool		click_right(t_vect mouse);
	bool		key_press(SDL_Event *event);
}				t_tileset_iterator;

typedef struct		s_spell_card : t_gui_obj
{
	t_button		*button; //0 - background | 1 - image | 2 - name | 3 - desc
	struct s_spell	**spell;		//spell of the card
	int				border;
	t_vect			coord;
	t_vect			size;
	t_vect			unit;
	int				text_size;
	vector<string>	text;
	vector<t_image *>
					text_image;
	vector<t_vect>	image_coord;
	vector<t_vect>	image_size;
	vector<t_vect>	text_coord;
	t_vect			paragraphe_size;
					s_spell_card();
					s_spell_card(struct s_spell **p_spell, t_vect coord, t_vect size, gui_funct p_funct, t_data p_data);
	void			draw_self();
	bool			click_left(t_vect mouse);
	bool			click_right(t_vect mouse);
	bool			key_press(SDL_Event *event);
}					t_spell_card;

typedef struct		s_actor_card : t_gui_obj
{
	t_button		*button;
	struct s_actor	**actor;
	t_button		*button_name;
	t_button		*actor_frame;
					s_actor_card();
					s_actor_card(t_button *p_button, struct s_actor **p_actor, t_button *p_button_name, t_button *p_actor_frame);
	void			draw_self();
	bool			click_left(t_vect mouse);
	bool			click_right(t_vect mouse);
	bool			key_press(SDL_Event *event);
}					t_actor_card;

typedef struct		s_shop_spell : t_gui_obj
{
	s_spell_card	*button;
	struct s_shop_item	**item;
	t_vect			delta;
	t_vect			size;
	string			text;
	t_image			*text_image;
	t_vect			image_coord;
	t_vect			image_size;
					s_shop_spell();
					s_shop_spell(s_spell_card *p_button, struct s_shop_item **p_item, t_vect p_delta, t_vect p_size);
	void			draw_self();
	bool			click_left(t_vect mouse);
	bool			click_right(t_vect mouse);
	bool			key_press(SDL_Event *event);
}					t_shop_spell;

typedef struct		s_shop_map : t_gui_obj
{
	t_button		*button;
	struct s_shop_item	**item;
	int				text_size;
	t_vect			icon_coord;
	t_vect			icon_size;
	t_vect			price_coord;
	t_vect			price_size;
	t_vect			text_coord;
	vector<string>	text;
	vector<t_image *>
					text_image;
	vector<t_vect>	image_size;
					s_shop_map();
					s_shop_map(t_button *p_button, struct s_shop_item **p_item, t_vect p_icon_coord, t_vect p_icon_size, t_vect p_text_coord, t_vect p_price_coord, t_vect p_price_size);
	void			draw_self();
	bool			click_left(t_vect mouse);
	bool			click_right(t_vect mouse);
	bool			key_press(SDL_Event *event);
}					t_shop_map;

#endif
