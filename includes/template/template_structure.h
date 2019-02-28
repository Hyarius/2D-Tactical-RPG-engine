#ifndef TEMPLATE_STRUCTURE_H
# define TEMPLATE_STRUCTURE_H

using namespace std;

enum X_ROTATE { X };
enum Y_ROTATE { Y };
enum Z_ROTATE { Z };
enum ROTATION { R };
enum TRANSLATION { T };
enum SCALE { S };
enum PERSPECTIVE { P };
enum IDENTITY { I };

typedef struct		s_color
{
	double r;
	double g;
	double b;
	double a;
					s_color();
					s_color(double p_r, double p_g, double p_b);
					s_color(double p_r, double p_g, double p_b, double p_a);
					s_color(int p_r, int p_g, int p_b);
					s_color(int p_r, int p_g, int p_b, int p_a);
	s_color			operator + (double add);
	s_color			operator - (double add);
}					t_color;

typedef struct		s_vect
{
	double			x;
	double			y;
					s_vect();
					s_vect(int x, int y);
					s_vect(double x, double y);
					s_vect(int x, double y);
					s_vect(double x, int y);
	s_vect			operator * (int mult);
	s_vect			operator * (double mult);
	s_vect			operator * (s_vect mult);
	s_vect			operator + (int add);
	s_vect			operator + (double add);
	s_vect			operator + (s_vect add);
	s_vect			operator / (int div);
	s_vect			operator / (double div);
	s_vect			operator / (s_vect div);
	s_vect			operator - (int min);
	s_vect			operator - (double min);
	s_vect			operator - (s_vect min);
	bool			operator < (const s_vect& delta) const;
	bool			operator > (const s_vect& delta) const;
	bool			operator == (const s_vect& delta) const;
	bool			operator != (const s_vect& delta) const;
}					t_vect;

typedef struct		s_vertex
{
	double			x;
	double			y;
	double			z;
	s_vertex(int p_x, int p_y, int p_z);
	s_vertex(double p_x, double p_y, double p_z);
	s_vertex();
}					t_vertex;

typedef struct	s_point
{
	double		x;
	double		y;
	double		r;
	double		g;
	double		b;
	double		a;
				s_point();
				s_point(double p_x, double p_y);
				s_point(double p_x, double p_y, double p_a, double p_b);
				s_point(double p_x, double p_y, double p_r, double p_g, double p_b);
				s_point(double p_x, double p_y, double p_r, double p_g, double p_b, double p_a);
}				t_point;

typedef struct		s_image
{
	SDL_Surface		*surface;
	GLenum			format;
	GLint			internal_format;
	GLuint			texture_id;
					s_image();
					s_image(string path);
					s_image(t_color p_color);
					s_image(SDL_Surface *p_surface);
	void			draw_self(t_vect p_coordd, t_vect p_size);
	void			draw_centred_self(t_vect p_coord, t_vect p_size);
	void			draw_self(t_vect tl, t_vect tr, t_vect dl, t_vect dr);
}					t_image;

typedef struct		s_angle
{
	double x;
	double y;
	double z;
					s_angle();
					s_angle(double x, double y, double z);
}					t_angle;

struct matrice
{
	double value[4][4];

	matrice(double a0, double a1, double a2, double a3, double b0, double b1, double b2, double b3, double c0, double c1, double c2, double c3,
					double d0, double d1, double d2, double d3);
	matrice(X_ROTATE, double angle);
	matrice(Y_ROTATE, double angle);
	matrice(Z_ROTATE, double angle);
	matrice(ROTATION, double x, double y, double z);
	matrice(TRANSLATION, double t_x, double t_y, double t_z);
	matrice(SCALE, double t_x, double t_y, double t_z);
	matrice operator * (matrice p_matrice);
	t_vertex operator * (t_vertex vertex);
	void print();
};

#endif
