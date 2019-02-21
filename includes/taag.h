#ifndef TAAG_H
# define TAAG_H

# include "template.h"
# include "gui.h"
# include "taag_gui.h"

extern map<string, struct s_tileset>	tileset_map;//the dictionnary holding on every
													//tileset used as tile for the prog, in extern to accessibility
extern map<string, struct s_tileset>	interface_map;
													//the dictionnary holding on every
													//tileset used by interface for the prog, in extern to accessibility
extern map<string, struct s_tileset>	sprite_map; //the dictionnary holding on every
													//tileset used by sprite for the prog, in extern to accessibility
extern map<string, struct s_tileset>	animation_map; //the dictionnary holding on every
													//tileset used by sprite for the prog, in extern to accessibility
extern vector<string>					tileset_name; //stock the name of every tileset in tileset_map
extern vector<string>					sprite_name;//stock the name of every tileset in sprite_map
extern vector<string>					human_sprite_name;//stock the name of every tileset in sprite_map
extern vector<string>					interface_name;//stock the name of every tileset in interface_map
extern vector<string>					animation_name;//stock the name of every tileset in interface_map

# define SPELL_BUTTON	3

#define ACTOR_PATH "ressources/game_object/actor/"
#define ACTOR_EXT ".act"
#define MONSTER_PATH "ressources/game_object/monster/"
#define MONSTER_EXT ".act"

#define MAP_PATH "ressources/map/"
#define MAP_EXT ".map"

#define NODE_PATH "ressources/game_object/node/"
#define NODE_EXT ".node"
#define OBS_PATH "ressources/game_object/object/"
#define OBS_EXT ".obs"

#define SPELL_PATH "ressources/spell/"
#define SPELL_EXT ".spell"

#define TILESET_TILESET_PATH "ressources/tileset/tileset/"
#define TILESET_TILESET_EXT ".tls"
#define TILESET_CHARSET_PATH "ressources/tileset/charset/"
#define TILESET_CHARSET_EXT ".tls"
#define TILESET_INTERFACE_PATH "ressources/tileset/interface/"
#define TILESET_INTERFACE_EXT ".tls"
#define TILESET_ANIMATION_PATH "ressources/tileset/animation/"
#define TILESET_ANIMATION_EXT ".tls"

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

extern t_node			empty_node;	//the basic node for cell

typedef struct			s_value
{
	int					value;		//actual value of the stat
	int					max;		//Maximum value of the stat
						s_value();
						s_value(int p_max);
						s_value(int p_value, int p_max);
	int					percent();
}						t_value;

#define STAT_VALUE_HP	0	//refer to the HP value of the actor stat
#define STAT_VALUE_PA	1	//refer to the PA value of the actor stat
#define STAT_VALUE_PM	2	//refer to the PM value of the actor stat
#define STAT_VALUE_INIT	3	//refer to the init value of the actor stat

typedef struct			s_stat
{
	t_value				hp;			//health point
	t_value				pa;			//action point
	t_value				pm;			//mouvement point
	int					init;		//initiative
						s_stat();
						s_stat(t_value p_hp, t_value p_pa, t_value p_pm, int p_init);
}						t_stat;

#define INT_TRUE 1
#define INT_FALSE 0

#define R_CIRCLE 0
#define R_LINE 1

#define Z_DIAM 0
#define Z_CROSS 1
#define Z_LINE 2
#define Z_SQUARE 3

typedef struct			s_effect_stat
{
	int					value[4];
						s_effect_stat();
						s_effect_stat(int value0, int value1, int value2, int value3);
}						t_effect_stat;

typedef void	(*event)(struct s_actor*, struct s_actor*, s_effect_stat);

extern vector<string>	list_effect_name;
extern vector<event>	g_effects;

typedef struct			s_effect
{
	event				effect;
	t_effect_stat		stat;
						s_effect();
						s_effect(event p_effect, int value0, int value1, int value2, int value3);
}						t_effect;

typedef struct			s_animation
{
	t_tileset			*tile;
	int					start; //on what frame this anim start
	int					len; //len total of the animation
	double				index; //where we are into this animation
	t_vect				anim_size;
						s_animation();
						s_animation(t_tileset *p_tile, int p_start, int p_len, t_vect anim_size);
	void				draw_self(t_vect coord, t_vect target, t_vect offset, t_vect size);
}						t_animation;

typedef struct			s_spell
{
	string				name;			//name of the spell -
	string				desc;			//description of the spell
	int					m_spell;		//true = monster only
	t_tileset			*tile;			//tile of the icon
	t_vect				icon;			//the sprite to use to print the icon
	int					cost_pa;		//cost in PA-
	int					cost_pm;		//cost in PM -
	int					range[2];		//0 - range min / 1 - range max -
	int					block;			//did this spell need vision : 0 - he need vision / 1 - he can go over obs
	int					on_target;		//0 - click on target only / 1 - click on target and empty tile / 2 - click only on empty tile
	int					range_type;		//what kind of vision is it ? -
	int					zone_type;		//what kind of zone is it ? -
	int					zone_size;		//size of the zone
	vector<t_effect>	effect;			//list of effect
	s_animation			target_anim;	//anim to display on the target zone
	int					anim_type; 		//0 - single anim on the clicked coord | 1 - anim on every cell | 2 - anim on every target
						s_spell();
						s_spell(string p_name, string p_desc, t_tileset *p_tile, t_vect p_icon, int p_m_spell,
								int p_cost_pa, int p_cost_pm,
								int range_min, int range_max, int p_block, int p_on_target,
								int p_range_type, int p_zone_type, int p_zone_size,
								vector<t_effect> p_effect, t_animation p_target_anim, int p_anim_type);
}						t_spell;

extern map<string, t_spell>	spell_map;	//The dictionnary holding every spell
										//from the game
extern vector<string>		spell_name;
extern vector<string>		spell_heros_name;
typedef struct			s_visual_info
{
	string				text;
	int					text_color;
	int					text_size;
	int					index;
	vector<t_vect>		text_coord;
						s_visual_info();
						s_visual_info(string p_text, int p_text_color, int p_text_size, vector<t_vect> p_text_coord);
						s_visual_info(string p_text, int p_text_color, int p_text_size, int p_index, vector<t_vect> p_text_coord);
	void				draw_self(t_vect target, t_vect offset, t_vect size, double zoom);
}						t_visual_info;

typedef struct			s_ai_helper
{
	vector<int>			value;
						s_ai_helper();
						s_ai_helper(vector<int> p_value);
	void 				operator = (vector<int> data);
}						t_ai_helper;

typedef struct          s_actor
{
	string				path;	//path for the file where it came from, utils for saving map
	string				name;		//Name of the actor
	bool				selected;	//is this actor the selected ?
	t_tileset			*tile;		//tile used by the actor, describe in the .act file
	t_vect				sprite;		//the sprite to use in a x/y axis
	int					dir;		//direction where the actor look
	t_stat				stat;		//stat of the actor
	t_vect				coord;		//position of the actor in game_space
	vector<t_vect>		destination;//list of coord the actor will take while moving
	vector<t_visual_info>	visual_info; //list of every infos we need to print on screen
	int					team;		//0 - neutral / 1 - team / 2 - enemy / 3 - ally
	t_spell				*spell[6];
	vector<t_ai_helper>	gambit; //list of every action the actor need to do if controled by AI

						s_actor();
						s_actor(string p_name, t_tileset *p_tile, t_vect p_sprite, t_stat p_stat);
						s_actor(string p_name, t_tileset *p_tile, t_vect p_sprite, t_stat p_stat, t_spell **p_spell);
	void				reset_value();//reset the value of PA and PM to max
	void				draw_self(t_vect target, t_vect offset, t_vect size); //draw the actor on him place on the screen
	void				draw_visual_info(t_vect target, t_vect offset, t_vect size, double zoom); //draw the actor visual info on the screen
}						t_actor;

typedef struct			s_cell
{
	t_vect				coord;		//coord of the cell in the game_space
	int					m_dist;		//utils for pathfinding - mouvement distance
	int					v_dist;		//utils for vision line - vision distance
	t_actor				*actor;		//is there an actor here ?
	t_node				*node;		//what node this cell is
	t_vect				cursor;		//the sprite of the cursor to print up the cell
	vector<t_visual_info>
						visual_info; //list of every infos we need to print on screen
	vector<t_animation>	animation; //list of every animation in this cell
						s_cell();
						s_cell(t_vect p_coord, t_node *p_node);
	void				draw_cell(t_vect target, t_vect offset, t_vect size); //draw the cell on her place on the screen
	void				draw_visual_info(t_vect target, t_vect offset, t_vect size, double zoom); //draw the cell visual info on the screen
}						t_cell;

typedef struct			s_game_board
{
	t_vect				board_size;	//size in x / y of the map
	vector<t_node>		node_list;	//list all the node printable by the board
	t_tileset			*cursor_tile;//the tile to use to print cursor up the cell
	vector<t_vect>		placement_list; //list of the node where you can place your actor
	vector<t_vect>		enemy_placement_list; //list of the node where you can place your actor
	vector<vector<t_cell>>
						cell_layer;	//list every cell of the map
	vector<t_actor *>	actor_list;	//contain every actor
	vector<t_actor *>	enemy_list;	//contain every enemy on the map
	vector<t_actor *>	ally_list;	//contain every ally on the map

	t_vect				offset;		//the offset of the center of the map
	t_vect				target;		//utils for zooming/moving the camera
	t_vect				sprite_unit;//the size of one sprite on the screen without the zoom
	double				zoom;		//the zoom to apply to print everything

						s_game_board();
						s_game_board(string p_path);
	t_cell				*get_cell(int x, int y);
	t_cell				*get_cell(t_vect target);
	bool				check_anim(); //check if an anim is ended
	void				add_actor(t_actor *new_actor);
	void				remove_actor(t_actor *old_actor);
	t_vect				get_mouse_pos();//return the position of the mouse on the map / -1 -1 if not on map
	void				draw_cursor(t_vect coord, t_vect target, t_vect size, t_vect offset, t_vect sprite);
									//draw a cursor on a certain coord
	void				draw_self();//draw everything about the map
	void				draw_cell_layer();//draw only the cell on the screen
	void				draw_mouse_cursor();//draw the mouse up the cell
	void				draw_cursor_layer();//draw only the cursor on the screen
	void				draw_placement(); //draw tile where we can place actor
	void				draw_actor_list();//draw every actor on the screen
	void				draw_visual_info();//draw every visual info on screen
	void				draw_animation();//draw every animation info on screen
	void				draw_cell_border();//draw a 1 grey pixel border around every tile
	void				reset_board(); //reset every cursor on the map to 0, 0
	void				handle_mouvement(SDL_Event *event);//handle the left click motion of the mouse to move the camera
	void				handle_zoom(SDL_Event *event);//handle the wheel of the mouse, zooming the camera
}						t_game_board;

typedef struct			s_game_engine
{
	t_game_board		board;		//The board where the game take place
	vector<t_actor *>	turn_order;	//the list of every actor of the team 1, 2 and 3
	size_t				turn_index;	//iterator to the turn_order
	t_gui				gui;		//Graphical User Interface of the game, contain every image on the screen
	bool				calculated;	//did we need to calculate something ?
	int					s_spell;	//what spell is selected : -1 for none
	int					turn_num;	//the number of turn
	vector<t_actor *>	actor_pool;	//every actor that you can place on map

						s_game_engine();
						s_game_engine(string p_path);
	void				draw_board();	//draw the board at the center of the screen
	void				draw_gui(); 	//draw the gui, and the multiples value/image to print on it
	void				draw_actor_info_on_gui(); //draw HP, PA, PM on the gui
	void				draw_cell_info_on_gui(); //draw the cell name, cost, if occuped, etc etc on the gui
	void				draw_path(); //draw the path the actor will follow on screen
	void				draw_select_wheel(int *index);	//draw the wheel the player can use to select what actor gonna be placed
	void				initiate_turn_order();	//create the vector for tun order
	void				next_turn();	//pass to the next player
	void				insert_actor(t_actor *new_actor);	//insert an actor into the turn order in respect of him initiative
	void				delete_actor(t_actor *old_actor);	//delete an actor
	void				invoke_actor(t_actor *new_actor, t_vect coord);	//invoke a new actor at a place
	void				outvoke_actor(t_actor *new_actor);	//outvoke an actor
	void				m_calc_cell(vector<t_vect> *to_calc, int i, int x, int j, int y);	//Utils of calculate_distance
	bool				v_calc_cell(t_vect source, t_vect target, int prev_dist); //utils to check if a cell can be seen
	void				calculate_distance();		//compute what tile the current actor can acces by foot
	void				calculate_vision_circle();	//compute what tile the current actir can see
	void				calculate_vision_line();	//compute what tile the current actir can see
	void				calculate_zone();			//compute what tile gonna been affected by the active spell
	vector<t_vect>		pathfinding(t_vect dest);	//get the list of destination the actor gonna pass to go on the dest tile
	vector<t_vect>		calc_path(t_vect dest);		//get the list of tile to go to targeted tile
	vector<t_vect>		calc_diam(int size);		//calc the list of cell to hit with a cross zone
	vector<t_vect>		calc_cross(int size);		//calc the list of cell to hit with a cross_line zone
	vector<t_vect>		calc_line(int size, t_vect dir);	//calc the list of cell to hit with a line zone
	vector<t_vect>		calc_square(int size);		//calc the list of cell to hit with a square zone
	void				move_actor(t_vect dest);	//check if the distance is close enought than start the pathfinding for the actor
	void				cast_spell(t_vect mouse);	//check if the distance is close enought to cast the selected spell
	void				check_alive();				//check if an actor is dead
	void				update_board();			//update the state of the screen, updating the actor_list.destination
	void				handle_control_camera(SDL_Event *event); //handle the control refering to the camera motion
	void				handle_control_game(SDL_Event *event); //handle the control refering to the game
	void				handle_actor_placement(SDL_Event *event, int *index);
	void				placement_phase();
	void				game_loop();
	void				ending_fight(bool *play);

	void				enemy_turn();

	bool				get_close_enemy(t_ai_helper data);
	bool				get_close_enemy_weak(t_ai_helper data);
	bool				get_close_enemy_percent(t_ai_helper data);
	bool				get_close_ally(t_ai_helper data);
	bool				get_close_ally_percent(t_ai_helper data);
	bool				flee_enemy(t_ai_helper data);
	bool				attack(t_ai_helper data);
	bool				attack_weak(t_ai_helper data);
	bool				attack_percent(t_ai_helper data);
	bool				help(t_ai_helper data);
	bool				help_weak(t_ai_helper data);
	bool				help_percent(t_ai_helper data);
	bool				action_on_turn(t_ai_helper data);

	bool				execute_gambit(t_actor *source);

}						t_game_engine;


#define CHARGE			0	//run to the closest enemy at range value[1], delta value[2] and range type value[3]
#define CHARGE_WEAK		1	//run to the enemy with less hp at range value[1], delta value[2] and range type value[3]
#define CHARGE_PERCENT	2	//run to the enemy with less % hp at range value[1], delta value[2] and range type value[3]
#define RETREAT			3	//retreat to the safest tile if stat hp < value[1] %
#define SUPPORT			4	//run to closest ally at range value[1], delta value[2] and range type value[3]
#define SUPPORT_PERCENT	5	//run to closest ally with less % hp at range value[1], delta value[2] and range type value[3]
#define ATTACK			6	//cast the spell num value[1] on the first enemy availible in range if possible
#define ATTACK_WEAK		7	//cast the spell num value[1] on the enemy with the less hp in range if possible
#define ATTACK_PERCENT	8	//cast the spell num value[1] on the enemy with the less % hp in range if possible
#define HELP			9	//cast the spell num value[1] on an ally
#define HELP_WEAK		10	//cast the spell num value[1] on the ally with less % HP
#define HELP_PERC		11	//cast the spell num value[2] on the ally if HP % < value[1]
#define TURN			12	//if turn == value[1] --> execute command num value[2] with verification helped by value[3]

typedef bool (s_game_engine:: *gambit_command)(t_ai_helper);

void					read_tileset();				//read every tileset file and place it into the tileset_map
t_node					read_node(string p_path); 	//read one .node file and return a t_node
t_actor					read_actor(string p_path);	//read one .act file and return a t_actor
t_spell					read_one_spell(string path);//read one spell and return it
void					read_spell();				//read every spell and place it into the spell_map
void					init_effects();				//initialize every effect spell can use
t_visual_info			create_visual_info(string p_text, int p_text_color, int p_text_size, t_vect p_starting_coord); //creating one visual_info

void					draw_spell_card(t_spell *spell, t_vect coord, t_vect size);	//draw one card info on the top-left corner

void					set_game_engine(t_game_engine *new_game);
void					set_coord_target(t_vect p_coord);

void					generate_charset_tileset();
void					generate_animation_tileset();
void					generate_node_file();

t_tileset				*get_tileset_tile(string p_name);
t_tileset				*get_tileset_tile(size_t name_num);
t_tileset				*get_sprite_tile(string p_name);
t_tileset				*get_sprite_tile(size_t name_num);
t_tileset				*get_interface_tile(string p_name);
t_tileset				*get_interface_tile(size_t name_num);
t_tileset				*get_animation_tile(string p_name);
t_tileset				*get_animation_tile(size_t name_num);

t_game_board			board_generator(int size_x, int size_y, int node);

void 					deal_dmg(t_actor *source, t_actor *target, t_effect_stat effect_stat);
void 					heal(t_actor *source, t_actor *target, t_effect_stat effect_stat);
void 					change_pm(t_actor *source, t_actor *target, t_effect_stat effect_stat);
void 					change_pa(t_actor *source, t_actor *target, t_effect_stat effect_stat);
void					heal_caster(t_actor *source, t_actor *target, t_effect_stat effect_stat);
void					dmg_caster(t_actor *source, t_actor *target, t_effect_stat effect_stat);
void					push_actor(t_actor *source, t_actor *target, t_effect_stat effect_stat);
void					pull_actor(t_actor *source, t_actor *target, t_effect_stat effect_stat);
void					move_caster(t_actor *source, t_actor *target, t_effect_stat effect_stat);
void					swap_actor(t_actor *source, t_actor *target, t_effect_stat effect_stat);
void					change_caster_pa(t_actor *source, t_actor *target, t_effect_stat effect_stat);
void					change_caster_pm(t_actor *source, t_actor *target, t_effect_stat effect_stat);

string					parse_gambit(t_ai_helper data);

void					menu_start();
void					menu_play(t_data data);
void						menu_choose_map(t_data data);
void					menu_player_editor(t_data data);
void						menu_choose_spell(t_data data);
void						menu_choose_sprite(t_data data);
void						menu_save_actor(t_data data);
void						menu_load_actor(t_data data);
void						menu_delete_actor(t_data data);
void					menu_monster_editor(t_data data);
void						menu_choose_spell_monster(t_data data);
void						menu_save_monster(t_data data);
void						menu_load_monster(t_data data);
void						menu_delete_monster(t_data data);
void					menu_map_editor(t_data data);
void						menu_generate_board(t_data data);
void 						menu_save_map(t_data data);
void 						menu_load_map(t_data data);
void 						menu_delete_map(t_data data);
void						menu_place_monster(t_data data);
void					menu_spell_editor(t_data data);
void						menu_save_spell(t_data data);
void						menu_load_spell(t_data data);
void						menu_delete_spell(t_data data);
void						menu_select_effect(t_data data);
void						menu_edit_animation(t_data data);
void							menu_select_anim(t_data data);
void 					menu_gambit_editor(t_data data);
void					menu_quit(t_data data);

#endif
