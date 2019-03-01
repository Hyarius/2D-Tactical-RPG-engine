#ifndef TAAG_H
# define TAAG_H

# include "template.h"
# include "gui.h"
# include "taag_gui.h"

extern map<string, struct s_tileset>	tileset_map;//the dictionnary holding on every
													//tileset used as tile for the prog, in extern to accessibility
extern vector<string>					tileset_name; //stock the name of every tileset in tileset_map
extern vector<string>					sprite_name;//stock the name of every tileset in sprite_map
extern vector<string>					human_sprite_name;//stock the name of every tileset in sprite_map
extern vector<string>					interface_name;//stock the name of every tileset in interface_map
extern vector<string>					animation_name;//stock the name of every tileset in interface_map

extern vector<t_gui>					gui_tutorial;

# define SPELL_BUTTON	3
# define TUTO_SIZE		30

#define ACTOR_PATH 				"ressources/game_object/actor/"
#define ACTOR_EXT 				".act"
#define MONSTER_PATH 			"ressources/game_object/monster/"
#define MONSTER_EXT 			".act"

#define MAP_PATH 				"ressources/map/"
#define MAP_EXT 				".map"

#define NODE_PATH 				"ressources/game_object/node/"
#define NODE_EXT 				".node"
#define OBS_PATH 				"ressources/game_object/object/"
#define OBS_EXT 				".obs"

#define SPELL_PATH 				"ressources/spell/"
#define SPELL_EXT 				".spell"

#define TILESET_TILESET_PATH 	"ressources/tileset/tileset/"
#define TILESET_TILESET_EXT 	".tls"
#define TILESET_CHARSET_PATH 	"ressources/tileset/charset/"
#define TILESET_CHARSET_EXT 	".tls"
#define TILESET_INTERFACE_PATH 	"ressources/tileset/interface/"
#define TILESET_INTERFACE_EXT 	".tls"
#define TILESET_ANIMATION_PATH 	"ressources/tileset/animation/"
#define TILESET_ANIMATION_EXT 	".tls"

#define SHOP_MAP_PATH			"ressources/shop/map/"
#define SHOP_TILE_PATH			"ressources/shop/sprite/"
#define SHOP_SPELL_PATH			"ressources/shop/spell/"
#define SHOP_EXT				".shop"

#define GAME_ENGINE_PATH		"ressources/game_object/game_engine"
#define GAME_ENGINE_EXT			".eng"

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

typedef struct			s_action_stat
{
	int					value[4];
						s_action_stat();
						s_action_stat(int value0, int value1, int value2, int value3);
}						t_action_stat;

typedef void	(*event)(struct s_actor*, struct s_actor*, s_action_stat);

extern vector<string>	list_action_name;
extern vector<event>	g_effects;

typedef struct			s_action
{
	event				effect;
	t_action_stat		stat;
						s_action();
						s_action(event p_effect, int value0, int value1, int value2, int value3);
}						t_action;

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
	int					price;			//utils for shop
	string				name;			//name of the spell -
	string				desc;			//description of the spell
	int					m_spell;		//true = monster only
	t_tileset			*tile;			//tile of the icon
	t_vect				icon;			//the sprite to use to print the icon
	int					cost_pa;		//cost in PA-
	int					cost_pm;		//cost in PM -
	int					cooldown;		//how many turn before re_casting it ?
	int					range[2];		//0 - range min / 1 - range max -
	int					block;			//did this spell need vision : 0 - he need vision / 1 - he can go over obs
	int					on_target;		//0 - click on target only / 1 - click on target and empty tile / 2 - click only on empty tile
	int					range_type;		//what kind of vision is it ? -
	int					zone_type;		//what kind of zone is it ? -
	int					zone_size;		//size of the zone
	vector<t_action>	effect;			//list of effect
	s_animation			target_anim;	//anim to display on the target zone
	int					anim_type; 		//0 - single anim on the clicked coord | 1 - anim on every cell | 2 - anim on every target
						s_spell();
						s_spell(string p_name, string p_desc, t_tileset *p_tile, t_vect p_icon, int p_m_spell,
								int p_cost_pa, int p_cost_pm, int p_cooldown,
								int range_min, int range_max, int p_block, int p_on_target,
								int p_range_type, int p_zone_type, int p_zone_size,
								vector<t_action> p_effect, t_animation p_target_anim, int p_anim_type);
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

typedef struct			s_effect
{
	struct s_actor		*source;
	int					effect_type; //-1 - ignore / 0 - on turn begin / 1 - on action / 2 - on mouvement
	vector<t_action>	action; //list every action this effect had
	int					duration; //how many time left
						s_effect();
						s_effect(struct s_actor *p_source, int p_effect_type, vector<t_action> p_action, int duration);
}						t_effect;

typedef struct			s_effect_list
{
	vector<t_effect>	poison;
	vector<t_effect>	regeneration;
	vector<t_effect>	change_pa;
	vector<t_effect>	change_pm;
						s_effect_list();
}						t_effect_list;

/* effect type
0 - damage dealt
1 - heal dealt
2 - damage taken
3 - pa malus
4 - pm malus
5 - pa bonus
6 - pm bonus
7 - push actor
8 - pull actor
9 - damage caster
10 - move / placement
11 - poison damage
12 - regeneration
13 - effect malus pa
14 - effect malus pm
15 - effect bonus pa
16 - effect bonus pm
17 - cure poison
18 - cure regen
19 - cure malus PA
20 - cure malus PM
21 - cure bonus PA
22 - cure bonus PM
*/

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
	vector<t_visual_info>	visual_info; //link to the visual info of the cell where we standing
	int					team;		//0 - neutral / 1 - team / 2 - enemy / 3 - ally
	t_spell				*spell[6];
	int					cooldown[6];
	t_effect_list		effect_list;	//list every effect this player is affected by
	vector<t_ai_helper>	gambit; //list of every action the actor need to do if controled by AI
	int					spell_used[6];	//count every time we use a spell and store it
	int					total_effect[23];	//store the total amount of effect dealt by spell when used
						s_actor();
						s_actor(string p_name, t_tileset *p_tile, t_vect p_sprite, t_stat p_stat);
						s_actor(string p_name, t_tileset *p_tile, t_vect p_sprite, t_stat p_stat, t_spell **p_spell);
	void				reset_value();//reset the value of PA and PM to max
	void				draw_self(t_vect target, t_vect offset, t_vect size); //draw the actor on him place on the screen
	void				apply_effect(int type);
	void				handle_effect_duration();
	void				change_stat_hp(int value);
	void				change_stat_pa(int value);
	void				change_stat_pm(int value);
	void				apply_effect(string text);
	void				change_stat_hp(int value, int color);
	void				change_stat_pa(int value, int color);
	void				change_stat_pm(int value,int color);
	void				apply_effect(string text, int color);
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

typedef struct			s_shop_item
{
	string				to_add;
	t_tileset			*tile;
	int					sprite;
	int					price;
						s_shop_item();
						s_shop_item(string path);
						s_shop_item(string p_to_add, t_tileset *p_tile, int p_sprite, int p_price);
}						t_shop_item;

typedef struct			s_game_board
{
	t_vect				board_size;	//size in x / y of the map
	vector<t_node>		node_list;	//list all the node printable by the board
	t_tileset			*cursor_tile;//the tile to use to print cursor up the cell
	vector<t_vect>		placement_list; //list of the node where you can place your actor
	vector<t_vect>		enemy_placement_list; //list of the node where you can place your actor
	vector<vector<t_cell>>
						cell_layer;	//list every cell of the map
	vector<t_actor *>	actor_pool;	//contain every actor
	vector<t_actor *>	actor_list;	//contain every actor
	vector<t_actor *>	enemy_list;	//contain every enemy on the map
	vector<t_actor *>	ally_list;	//contain every ally on the map
	vector<t_actor *>	turn_order;	//the list of every actor of the team 1, 2 and 3
	size_t				turn_index;	//iterator to the turn_order
	t_gui				gui;		//Graphical User Interface of the game, contain every image on the screen
	bool				calculated;	//did we need to calculate something ?
	int					s_spell;	//what spell is selected : -1 for none
	int					turn_num;	//the number of turn
	t_vect				offset;		//the offset of the center of the map
	t_vect				target;		//utils for zooming/moving the camera
	t_vect				sprite_unit;//the size of one sprite on the screen without the zoom
	double				zoom;		//the zoom to apply to print everything

	int					shop_icon;		//utils for shop
	int					price;			//utils for shop
	int					exp_reward;		//amount of exp win after finishing the board
	int					gold_reward;	//same for gold
	int					map_difficulty;	//an indicator of the difficulty

	//actor_handler
	void				add_actor(t_actor *new_actor);
	void				remove_actor(t_actor *old_actor);
	void				insert_actor(t_actor *new_actor);
	void				invoke_actor(t_actor *new_actor, t_vect coord);
	void				outvoke_actor(t_actor *new_actor);
	void				delete_actor(t_actor *new_actor);

	//AI_calc_function
	vector<t_vect>		AI_calc_diam(int size_min, int size_max);
	vector<t_vect>		AI_calc_cross(int size_min, int size_max);

	//AI_execution
	bool				execute_gambit(t_actor *source);

	//AI_function
	bool				get_close_enemy_percent(t_ai_helper data);
	bool				get_close_enemy_weak(t_ai_helper data);
	bool				get_close_enemy(t_ai_helper data);
	bool				get_close_ally(t_ai_helper data);
	bool				get_close_ally_percent(t_ai_helper data);
	bool				get_close_ally_damaged(t_ai_helper data);
	bool				flee_enemy(t_ai_helper data);
	bool				attack(t_ai_helper data);
	bool				attack_weak(t_ai_helper data);
	bool				attack_percent(t_ai_helper data);
	bool				help(t_ai_helper data);
	bool				help_weak(t_ai_helper data);
	bool				help_percent(t_ai_helper data);
	bool				help_caster_hp(t_ai_helper data);
	bool				attack_caster_hp(t_ai_helper data);
	bool				action_on_turn(t_ai_helper data);

	//AI_string_parsing
	string 				parse_effect_poison_desc(s_effect *effect);
	string 				parse_effect_regeneration_desc(s_effect *effect);
	string 				parse_effect_change_pm_desc(s_effect *effect);
	string 				parse_effect_change_pa_desc(s_effect *effect);

	//calc_distance
	void				m_calc_cell(vector<t_vect> *to_calc, int i, int x, int j, int y);
	void				calculate_distance(t_vect start);

	//calc_path
	vector<t_vect>		pathfinding(t_actor *player, t_vect dest, double speed);
	vector<t_vect>		calc_path(t_vect dest);

	//calc_range
	bool				v_calc_cell(t_vect source, t_vect target, int prev_dist);
	void				calculate_vision_circle();
	void				calculate_vision_line();

	//calc_zone
	vector<t_vect>		calc_diam(int size);
	vector<t_vect>		calc_cross(int size);
	vector<t_vect>		calc_line(int size, t_vect dir);
	vector<t_vect>		calc_square(int size);
	void				calculate_zone();

	//checker
	bool				check_anim();
	bool				check_visual();
	void				check_alive();
	void				ending_fight(bool *play);

	//draw_board
	void				draw_cell_border();
	void				draw_animation();
	void				draw_cursor(t_vect coord, t_vect target, t_vect size, t_vect offset, t_vect sprite);
	void				draw_board();
	void				draw_mouse_cursor();
	void				draw_cell_layer();
	void				draw_visual_info();
	void				draw_cursor_layer();
	void				draw_placement();
	void				draw_actor_list();
	void				draw_path();

	//draw_gui
	void				draw_actor_info_on_gui();
	void				draw_cell_info_on_gui();
	void				draw_select_wheel(int index);
	void				draw_gui();

	//draw_self
	void				draw_self();

	//end_game_menu
	void				end_game_win();
	void				end_game_lose();

	//game_loop
	void				game_loop();

	//getter
	t_cell				*get_cell(int x, int y);
	t_cell				*get_cell(t_vect target);
	t_vect				get_mouse_pos();

	//handle_input
	void				handle_mouvement(SDL_Event *event);
	void				handle_zoom(SDL_Event *event);
	void				handle_control_camera(SDL_Event *event);
	bool				cast_spell(t_vect mouse);
	void				move_actor(t_actor *player, t_vect dest, double speed);
	void				handle_actor_placement(SDL_Event *event, int *index, vector<t_actor *> *game_actor_list);
	void				handle_control_game(SDL_Event *event);

	//initialisation
						s_game_board();
						s_game_board(string p_path);
	void				initiate_turn_order();

	//placement_phase
	void				placement_phase(t_actor *game_actor_list[]);

	//reset
	void				reset_board();
	void				reset_vision(int min, int max);

	//turn_handler
	void				enemy_turn();
	void				next_turn();

	//updater
	void				update_board();

}						t_game_board;

typedef struct			s_game_engine
{
	t_game_board		board;		//The board where the game take place

	int					level;
	s_value				exp;
	int					gold;
	int					tuto_state;

	int					keyboard[6];

	string				actor[6];
	t_actor *			actor_array[6];

	vector<string>		spell_unlock;
	vector<string>		tile_unlock;
	vector<string>		map_unlock;

						s_game_engine();
						s_game_engine(string p_path);

	void				start_game(string path);
	void				recharge_actor();
	void				recharge_map();

	int 				calc_max_exp(int level);
	void				add_exp(int delta);
	bool				add_gold(int delta);
}						t_game_engine;

extern t_game_engine	*account;

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

typedef bool (s_game_board:: *gambit_command)(t_ai_helper);

void					read_tileset();				//read every tileset file and place it into the tileset_map
t_node					read_node(string p_path); 	//read one .node file and return a t_node
t_actor					read_actor(string p_path);	//read one .act file and return a t_actor
t_spell					read_one_spell(string path);//read one spell and return it
void					read_spell();				//read every spell and place it into the spell_map
void					init_actions();				//initialize every effect spell can use
t_visual_info			create_visual_info(string p_text, int p_text_color, int p_text_size, t_vect p_starting_coord); //creating one visual_info

void					draw_spell_card(t_spell *spell, t_vect coord, t_vect size);	//draw one card info on the top-left corner

void					set_game_engine(t_game_board *new_game);
void					set_coord_target(t_vect p_coord);
void					change_s_spell(t_data data);

void					generate_charset_tileset();
void					generate_animation_tileset();
void					generate_node_file();
void					generate_map_shop();
void					generate_sprite_shop();
void					generate_spell_shop();

t_tileset				*get_tileset_tile(string p_name);
t_tileset				*get_tileset_tile(size_t name_num);
t_tileset				*get_sprite_tile(string p_name);
t_tileset				*get_sprite_tile(size_t name_num);
t_tileset				*get_interface_tile(string p_name);
t_tileset				*get_interface_tile(size_t name_num);
t_tileset				*get_animation_tile(string p_name);
t_tileset				*get_animation_tile(size_t name_num);

t_game_board			board_generator(int size_x, int size_y, int node);

int						get_action_id(event type);

void 					deal_dmg(t_actor *source, t_actor *target, t_action_stat effect_stat);
void 					heal(t_actor *source, t_actor *target, t_action_stat effect_stat);
void 					change_pm(t_actor *source, t_actor *target, t_action_stat effect_stat);
void 					change_pa(t_actor *source, t_actor *target, t_action_stat effect_stat);
void					heal_caster(t_actor *source, t_actor *target, t_action_stat effect_stat);
void					dmg_caster(t_actor *source, t_actor *target, t_action_stat effect_stat);
void					push_actor(t_actor *source, t_actor *target, t_action_stat effect_stat);
void					pull_actor(t_actor *source, t_actor *target, t_action_stat effect_stat);
void					move_caster(t_actor *source, t_actor *target, t_action_stat effect_stat);
void					swap_actor(t_actor *source, t_actor *target, t_action_stat effect_stat);
void					change_caster_pa(t_actor *source, t_actor *target, t_action_stat effect_stat);
void					change_caster_pm(t_actor *source, t_actor *target, t_action_stat effect_stat);
void					push_caster(t_actor *source, t_actor *target, t_action_stat effect_stat);
void					pull_caster(t_actor *source, t_actor *target, t_action_stat effect_stat);

void					apply_poison(t_actor *source, t_actor *target, t_action_stat effect_stat);
void					apply_regeneration(t_actor *source, t_actor *target, t_action_stat effect_stat);
void					apply_pa_change(t_actor *source, t_actor *target, t_action_stat effect_stat);
void					apply_pm_change(t_actor *source, t_actor *target, t_action_stat effect_stat);

void					cure_poison(t_actor *source, t_actor *target, t_action_stat effect_stat);
void					cure_regeneration(t_actor *source, t_actor *target, t_action_stat effect_stat);
void					cure_malus_pa(t_actor *source, t_actor *target, t_action_stat effect_stat);
void					cure_malus_pm(t_actor *source, t_actor *target, t_action_stat effect_stat);
void					cure_bonus_pa(t_actor *source, t_actor *target, t_action_stat effect_stat);
void					cure_bonus_pm(t_actor *source, t_actor *target, t_action_stat effect_stat);

string					parse_gambit(t_ai_helper data);

void					menu_start();
void					menu_play(t_data data);
void						menu_choose_map(t_data data);
void					menu_actor_editor(t_data data);
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
void						menu_select_action(t_data data);
void						menu_edit_animation(t_data data);
void							menu_select_anim(t_data data);
void 					menu_gambit_editor(t_data data);
void 						menu_remove_gambit(t_data data);
void 						menu_add_gambit(t_data data);
void 					menu_shop(t_data data);
void						create_tab_sprite(t_data data);
void							modify_index_sprite_tab(t_data data);
void						create_tab_map(t_data data);
void							actualize_map_tab(int *index);
void							modify_index_map_tab(t_data data);
void						create_tab_spell(t_data data);
void							modify_index_spell_tab(t_data data);
void					menu_quit(t_data data);

string					create_new_game_engine();
void 					check_prog_architecture();

void					save_game_engine();

void					start_tutorial(t_gui *gui);
void					increment_tutorial(t_data data);
void					tmp_function(t_data data);
void					setting_keyboard(t_gui *gui);

#endif
