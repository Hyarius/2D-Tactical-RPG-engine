#ifndef TAAG_H
# define TAAG_H

# include "template.h"

extern map<string, t_tileset>		tileset_map;	//the dictionnary holding on every
													//tileset of the prog, in extern to accessibility

typedef struct			s_node
{
	string				name;		//name of the node
	t_tileset			*tile;		//tile used by this node, describ in the .node file
	t_vect				sprite;		//the sprite to print in a x/y axis
	int					cost;		//how many MP to move pass here
	bool				m_obs;		//can we move on it ?
	bool				v_obs;		//can we see thought it ?
						s_node();
						s_node(string p_name, t_tileset *p_tile, t_vect p_sprite, int p_cost, bool p_m_obs, bool p_v_obs);
}						t_node;

typedef struct			s_value
{
	int					value;
	int					max;
						s_value();
						s_value(int p_max);
						s_value(int p_value, int p_max);
}						t_value;

typedef struct			s_stat
{
	t_value				hp;
	t_value				pa;
	t_value				pm;
	int					init;
						s_stat();
						s_stat(t_value p_hp, t_value p_pa, t_value p_pm, int p_init);
}						t_stat;

typedef struct          s_actor
{
	string				name;		//Name of the actor
	bool				selected;
	t_tileset			*tile;		//tile used by the actor, describe in the .act file
	t_vect				sprite;		//the sprite to use in a x/y axis
	int					dir;		//direction where the actor look
	t_stat				stat;		//stat of the actor
	t_vect				coord;		//position of the actor in game_space
	vector<t_vect>		destination;//list of coord the actor will take while moving
	int					team;		//0 - neutral / 1 - team / 2 - enemy / 3 - ally
						s_actor();
						s_actor(string p_name, t_tileset *p_tile, t_vect p_sprite, t_stat p_stat);
	void				reset_value();
	void				draw_self(t_vect target, t_vect offset, t_vect size);
}						t_actor;

typedef struct			s_cell
{
	t_vect				coord;		//coord of the cell in the game_space
	int					m_dist;		//utils for pathfinding - mouvement distance
	t_actor				*actor;		//is there an actor here ?
	t_node				*node;		//what node this cell is
	t_vect				cursor;		//the sprite of the cursor to print up the cell
						s_cell();
						s_cell(t_vect p_coord, t_node *p_node);
	void				draw_cell(t_vect target, t_vect offset, t_vect size);
}						t_cell;

typedef struct			s_game_board
{
	t_vect				board_size;	//size in x / y of the map
	vector<t_node>		node_list;	//list all the node printable by the board
	t_tileset			*cursor_tile;//the tile to use to print cursor up the cell

	vector<vector<t_cell>>
						cell_layer;	//list every cell of the map
	vector<t_actor *>	actor_list;	//contain every actor

	t_vect				offset;		//the offset of the center of the map
	t_vect				target;		//utils for zooming/moving the camera
	t_vect				sprite_unit;//the size of one sprite on the screen without the zoom
	double				zoom;		//the zoom to apply to print everything

						s_game_board();
						s_game_board(string p_path);
	t_cell				*get_cell(int x, int y);
	t_vect				get_mouse_pos();//return the position of the mouse on the map / -1 -1 if not on map
	void				draw_cursor(t_vect coord, t_vect target, t_vect size, t_vect offset, t_vect sprite);
									//draw a cursor on a certain coord
	void				draw_self();//draw everything about the map
	void				draw_cell_layer();//draw only the cell on the screen
	void				draw_mouse_cursor();//draw the mouse up the cell
	void				draw_cursor_layer();//draw only the cursor on the screen
	void				draw_actor_list();//draw every actor on the screen
	void				reset_board();
	void				handle_mouvement(SDL_Event *event);//handle the left click motion of the mouse to move the camera
	void				handle_zoom(SDL_Event *event);//handle the wheel of the mouse, zooming the camera
}						t_game_board;

t_node					read_node(string p_path); //read one .node file and return a t_node
t_actor					read_actor(string p_path);//read one .act file and return a t_actor

#endif
