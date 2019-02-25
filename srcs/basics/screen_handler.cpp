#include "template.h"

double				SCREEN_RATIO_X = 0.9;
double				SCREEN_RATIO_Y = 0.9;

SDL_GLContext		g_context;
SDL_Window			*g_window;
t_vect				g_window_size;

GLuint				program_color;
GLuint				program_sprite;

GLuint				vertex_array;

GLuint				vertex_buffer;
GLuint				color_buffer;
GLuint				texture_buffer;
GLuint				alpha_buffer;

GLenum				texture_format;

GLuint				textureID;

SDL_Window			*get_window()
{
	return (g_window);
}

void				window_initialisation(string window_name)
{
	SDL_DisplayMode current;
	int				win_x;
	int				win_y;

	SDL_Init(SDL_INIT_EVERYTHING);
	printSDLError();
	IMG_Init(IMG_INIT_PNG);
	printSDLError();
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 2);
	printSDLError();
	SDL_GetDesktopDisplayMode(0, &current);
	printSDLError();
	g_window = SDL_CreateWindow(window_name.c_str(),
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		(int)(current.w * SCREEN_RATIO_X), (int)(current.h * SCREEN_RATIO_Y),
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	printSDLError();
	SDL_GetWindowSize(g_window, &win_x, &win_y);
	printSDLError();
	g_window_size = t_vect(win_x, win_y);
	printSDLError();
	g_context = SDL_GL_CreateContext(g_window);
	printOpenGLError();
	printSDLError();

	glDisable(GL_CULL_FACE);
	printOpenGLError();
	printSDLError();

	SDL_GL_SetSwapInterval(0);
	printOpenGLError();
	printSDLError();

	#ifndef __APPLE__
		glewInit();
	#endif
		printOpenGLError();
		printSDLError();

	SDL_WarpMouseInWindow(g_window, (int)(g_window_size.x / 2), (int)(g_window_size.y) / 2);
	SDL_SetWindowGrab(g_window, SDL_FALSE);
	SDL_ShowCursor(1);

	t_color color = t_color(0.1, 0.1, 0.1);

	glClearColor((GLclampf)color.r, (GLclampf)color.g, (GLclampf)color.b, 0.0f);

	printOpenGLError();
	printSDLError();
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	glGenBuffers(1, &vertex_buffer);
	glGenBuffers(1, &color_buffer);
	glGenBuffers(1, &texture_buffer);
	glGenBuffers(1, &alpha_buffer);
	printOpenGLError();
	printSDLError();

	glBindVertexArray(get_vertex_array());
	printOpenGLError();
	printSDLError();

	program_color = LoadShaders(	"ressources/shader/color_shader.vert",	"ressources/shader/color_shader.frag");
	program_sprite = LoadShaders(	"ressources/shader/texture_shader.vert", "ressources/shader/texture_shader.frag");

	printOpenGLError();
	printSDLError();
	glGenTextures(1, &textureID);
	printOpenGLError();
	printSDLError();
	glBindTexture(GL_TEXTURE_2D, textureID);
	printOpenGLError();
	printSDLError();

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	printOpenGLError();
	printSDLError();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable( GL_DEPTH_TEST );
	printOpenGLError();
	printSDLError();

	srand(time(NULL));
	set_color_tab();
	TTF_Init();
	printOpenGLError();
	printSDLError();
}

t_vect				get_win_size(void)
{
	return (g_window_size);
}

void				prepare_screen()
{
	t_color color = t_color(0.1, 0.1, 0.1);

	glClearColor((GLclampf)color.r, (GLclampf)color.g, (GLclampf)color.b, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void				prepare_screen(t_color color)
{
	glClearColor((GLclampf)color.r, (GLclampf)color.g, (GLclampf)color.b, 0.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void				render_screen()
{
	check_frame();
	glFlush();
	SDL_GL_SwapWindow(g_window);
}

void				render_screen(bool value)
{
	check_frame(value);
	glFlush();
	SDL_GL_SwapWindow(g_window);
}

GLuint				get_program_color()
{
	return (program_color);
}

GLuint				get_program_sprite()
{
	return (program_sprite);
}

GLuint				get_vertex_array()
{
	return (vertex_array);
}

GLuint				get_vertex_buffer()
{
	return (vertex_buffer);
}

GLuint				get_color_buffer()
{
	return (color_buffer);
}

GLuint				get_texture_buffer()
{
	return (texture_buffer);
}

GLuint				get_alpha_buffer()
{
	return (alpha_buffer);
}

GLuint				get_texture_id()
{
	return (textureID);
}
