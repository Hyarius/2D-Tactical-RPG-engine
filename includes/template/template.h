#ifndef TEMPLATE_H
# define TEMPLATE_H

# include "include.h"
# include "text_define.h"
# include "define.h"
# include "tileset.h"
# include "template_structure.h"
# include "template_class.h"

extern double	SCREEN_RATIO_X;
extern double	SCREEN_RATIO_Y;

using namespace std;

//			BASIC_FUNCTION
void				error_exit(string msg, int error);
t_vect				get_mouse_coord();
int					generate_nbr(int min, int max);
vector<string>		strsplit(string input, string c);
vector<string>		list_files(string path, string extension);
bool				check_file_exist(string path);
SDL_Surface			*create_surface_color(t_color p_color);
t_vect				second_degree_solver(double a, double b, double c, double d, double e, double f);
void				get_leaks(string msg);

void				set_lang_text(string path);
string				*get_text(string key);

string				get_str(ifstream *myfile);
vector<string>		get_strsplit(ifstream *myfile, const string c, int size);

double				degree_to_radius(double angle);
double				radius_to_degree(double radian);
string				convert_double(double i, int n);
string 				convert_int(int i);
t_point				screen_to_opengl(t_vect source);

void				draw_triangle_texture(t_point a, t_point b, t_point c);
void				draw_triangle_color(t_point a, t_point b, t_point c);
void				add_triangle_texture(t_point a, t_point b, t_point c);
void				add_triangle_color(t_point a, t_point b, t_point c);
void				render_triangle_texture(GLuint texture_id);
void				render_triangle_color();

void				draw_image(t_vect tl, t_vect tr, t_vect dl, t_vect dr, double p_alpha);
void				draw_image(t_vect coord, t_vect size);
void				draw_image(t_vect coord, t_vect size, double p_alpha);

void				draw_rectangle(t_vect p_tl, t_vect p_tr, t_vect p_dl, t_vect p_dr, t_color c);
void				draw_rectangle(t_vect p_coord, t_vect p_size, t_color c);
void				draw_centred_rectangle(t_vect p_coord, t_vect p_size, t_color c);

void				window_initialisation(string window_name);
SDL_Window			*get_window();
t_vect				get_win_size(void);
void				prepare_screen();
void				prepare_screen(t_color color);
void				render_screen();
void				render_screen(bool value);

GLuint				get_program_color();
GLuint				get_program_sprite();

GLuint				get_vertex_array();

GLuint				get_vertex_buffer();
GLuint				get_color_buffer();
GLuint				get_texture_buffer();
GLuint				get_alpha_buffer();

GLuint				get_texture_id();

GLuint				LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

int					get_frame_state(int div);
void				check_frame();
void				check_frame(bool draw);

int					draw_text(string text, int text_size, t_vect coord, int color_type);
int					calc_text_len(string text, int text_size);
int					calc_text_max_size(string text, t_vect size);
int					draw_centred_text(string text, int text_size, t_vect coord, int color_type);
int					draw_lined_text(string text, int text_size, t_vect coord, int color_type);
int					calc_paragraphe_size(string text, t_vect size);
void				draw_paragraphe(string text, t_vect coord, t_vect size, int color_type);
void				draw_paragraphe(string text, int text_size, t_vect coord, t_vect size, int color_type);

void				set_color_tab();
t_image				*get_char(int size, int p_color, char c);
TTF_Font			*get_font(int size);
SDL_Color			get_color(int i);
int					calc_text_size(string p_text, t_vect p_size);

#endif
