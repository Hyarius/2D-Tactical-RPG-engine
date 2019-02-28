#ifndef GUI_H
# define GUI_H

#include "template.h"

using namespace std;

# define BUTTON_NUM		1
# define ENTRY_NUM		2
# define TUTORIAL_NUM	3

# define MOUSE_LEFT		0
# define MOUSE_RIGHT	1

typedef struct		s_data
{
	vector<void *>	data;
					s_data();
					s_data(void *p_data);
					s_data(int nb_param, void *p_ptr, ...);
	void			operator = (void *p_data);
	void			operator + (void *p_ptr);
}					t_data;

typedef 			void(*gui_funct)(t_data);

typedef				int(*d_funct)(string, int, t_vect, int);

typedef struct		s_gui_comp
{
	virtual void	set_funct_param(int type, gui_funct p_funct, t_data p_data, d_funct p_draw_funct) = 0;
	virtual void	draw_self() = 0;
	virtual bool	click_left(t_vect mouse) = 0;
	virtual bool	click_right(t_vect mouse) = 0;
}					t_gui_comp;

typedef struct 		s_button_comp : t_gui_comp
{
	string			text; //text to draw on screen
	string			saved_text; //memory buffer of the text we created in image_text;
	int				text_size;
	int				text_color;
	SDL_Surface		*surface;
	t_image			*text_image;
	t_vect			image_coord;
	t_vect 			image_size;
	t_vect			coord[3]; //0 - coord first rect || 1 - coord second rect || 2 - text coord
	t_vect			size[2]; //0 - size first rect || 1 - size second rect
	gui_funct		funct_left; //function to use when clicked  with left click : initialized empty
	t_data			data_left; // data to send to the function  with left click : initialized empty
	gui_funct		funct_right; //function to use when clicked with right click : initialized empty
	t_data			data_right; // data to send to the function with right click : initialized empty
	d_funct			draw_funct;

	void			set_funct_param(int type, gui_funct p_funct, t_data p_data, d_funct p_draw_funct);
	void			set_funct_param(int type, gui_funct p_funct, t_data p_data);
	void			set_text(string text);
	virtual void	draw_self() = 0;
	bool			click_left(t_vect mouse);
	bool			click_right(t_vect mouse);
	bool			check_click(t_vect mouse);
	bool			key_press(SDL_Event *event);
}					t_button_comp;


typedef struct		s_text_button : t_button_comp
{
	t_color			color[2]; //0 - color back || 1 - color front
					s_text_button(	string p_text, int p_text_color, // text info
								t_vect p_coord, t_vect p_size, int border, // coord / size info
								t_color p_color1, t_color p_color2);
	void			draw_self(); // draw the button
}					t_text_button;


typedef struct		s_paragraph_button : t_button_comp
{
	t_color			color[2]; //0 - color back || 1 - color front
	vector<t_image *>
					text_paragraph;
	vector<t_vect>	paragraph_coord;
	vector<t_vect>	paragraph_size;
					s_paragraph_button(	string p_text, int p_text_color, int p_text_size, // text info
								t_vect p_coord, t_vect p_size, int border, // coord / size info
								t_color p_color1, t_color p_color2);
	void			draw_self(); // draw the button
}					t_paragraph_button;

typedef struct		s_image_button : t_button_comp
{
	t_image			image; //image to draw on the screen
					s_image_button(string p_text, int p_text_color,
						t_image p_image, t_vect p_coord, t_vect p_size, int border); //info about the image and it's place
						s_image_button( t_image p_image, t_vect p_coord, t_vect p_size); //info about the image and it's place
	void			draw_self(); // draw the button
}					t_image_button;

typedef struct		s_tileset_button : t_button_comp
{
	t_tileset		*tile;
	t_vect			selected;
	t_vect			anim;
					s_tileset_button(	string p_text, int p_text_color,
										t_tileset *p_tile, t_vect p_selected,
										t_vect p_coord, t_vect p_size, int border);
					s_tileset_button(t_tileset *p_tile, t_vect p_selected,
										t_vect p_coord, t_vect p_size, int border);
					s_tileset_button(string p_text, int p_text_color,
									t_tileset *p_tile, t_vect p_selected, t_vect p_anim,
									t_vect p_coord, t_vect p_size, int border);
					s_tileset_button(t_tileset *p_tile, t_vect p_selected, t_vect p_anim,
										t_vect p_coord, t_vect p_size, int border);
	void			draw_self(); // draw the button
}					t_tileset_button;

typedef struct		s_entry_comp : t_gui_comp
{
	string			text; //text to draw on screen
	string			front;
	string			back;
	string			desc; //text to draw on screen
	int				text_size;
	size_t			max_len;
	int				desc_size;
	int				text_color;
	t_vect			coord[3]; // 0 - coord tl | 1 - coord + border | 2 - coord text
 	t_vect			size[2]; // 0 - size totale | 1 - size + border
	bool			selected;
	d_funct			draw_funct;

	void			set_funct_param(int type, gui_funct p_funct, t_data p_data, d_funct p_draw_funct);
	void			set_back(string p_back);
	void			set_front(string p_front);
	void			add_text(string new_text);
	void			delete_text();
	virtual void	draw_self() = 0; // draw the button
	bool			click_left(t_vect mouse); // test if the mouse is in the button and start the funct if yes
	bool			click_right(t_vect mouse); // test if the mouse is in the button and start the funct if yes
	bool			key_press(SDL_Event *event); // test if a key is pressed and react to it -> send true if we have done something with it
}					t_entry_comp;

typedef struct		s_text_entry : t_entry_comp
{
	t_color			color[3]; //0 - color back || 1 - color front || color_back when selected
					s_text_entry(	string p_desc, string p_text, int p_text_color, //text info
									t_vect p_coord, t_vect p_size, int border, // coord / size info
									t_color p_color1, t_color p_color2, t_color p_color3); // color info
	void			draw_self(); // draw the button
}					t_text_entry;

typedef struct		s_image_entry : t_entry_comp
{
	t_image			image; //image to draw on the screen
	t_image			selec_image; //image to draw on the screen when selected
					s_image_entry(	string p_desc, string p_text, int p_text_color, //text info
									t_image p_image, t_image p_selec_image,
									t_vect p_coord, t_vect p_size, int border); //info about the image and it's place
	void			draw_self(); // draw the button
}					t_image_entry;

typedef struct		s_gui_obj
{
	virtual void	draw_self() = 0;
	virtual bool	click_left(t_vect mouse) = 0;
	virtual bool	click_right(t_vect mouse) = 0;
	virtual bool	key_press(SDL_Event *event) = 0;
	virtual 		~s_gui_obj() {}
}					t_gui_obj;

typedef struct		s_button : t_gui_obj
{
	t_button_comp	*button;

					s_button(t_button_comp *p_button, gui_funct p_funct, t_data p_data);
					s_button(t_button_comp *p_button, gui_funct p_funct, t_data p_data, d_funct p_draw_funct);
	void			draw_self();
	bool			click_left(t_vect mouse);
	bool			click_right(t_vect mouse);
	bool			key_press(SDL_Event *event);
}					t_button;

typedef struct		s_entry : t_gui_obj
{
	t_entry_comp	*entry;

					s_entry(t_entry_comp *p_entry);
	void			draw_self();
	bool			click_left(t_vect mouse);
	bool			click_right(t_vect mouse);
	bool			key_press(SDL_Event *event);
}					t_entry;

typedef struct		s_gui
{
	vector<vector<t_gui_obj *>>
					object_list;
	t_entry_comp	*entry;
	t_vect			canvas;
	t_vect			unit;
	double			tutorial_value;

					s_gui();
					s_gui(int x, int y);
	void			add(t_gui_obj *object);
	void			add(int rep, t_gui_obj *object);
	void			draw_self();
	bool			click(SDL_Event *event);
	bool			key_press(SDL_Event *event);
}					t_gui;

#endif
