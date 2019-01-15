#include "template.h"

void				error_exit(string msg, int error)
{
	printf("Error %d : %s\n", error, msg.c_str());
	getchar();
	exit(error);
}

int					generate_nbr(int min, int max)
{
	return(rand() % max + min);
}

t_vect				get_mouse_coord()
{
	int		x;
	int		y;

	SDL_GetMouseState(&x, &y);
	return(t_vect(x, y));
}

void				get_leaks(string msg)
{
	//system("clear");
	system("leaks taag>/dev/null | grep 'total leaked bytes'");
	cout << msg << "\n\n\n"<< endl;
	//getchar();
}

vector<string>		strsplit(string input, string c)
{
	vector<string>	tab;
	string			val;
	int 			start = 0;
	int 			end = 0;

	while (end != -1)
	{
		end = input.find(c, start);
		val = input.substr(start, end - start);
		if (val.size() != 0)
			tab.push_back(val);
		start = end + c.size();
	}
	return (tab);
}

SDL_Surface			*create_surface_color(t_color p_color)
{
	SDL_Surface		*surface;
	Uint32			rmask, gmask, bmask, amask;

	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	    rmask = 0xff000000;
	    gmask = 0x00ff0000;
	    bmask = 0x0000ff00;
	    amask = 0x000000ff;
	#else
	    rmask = 0x000000ff;
	    gmask = 0x0000ff00;
	    bmask = 0x00ff0000;
	    amask = 0xff000000;
	#endif

    surface = SDL_CreateRGBSurface(0, 1, 1, 32, rmask, gmask, bmask, amask);
	SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format,
		(Uint8)(p_color.r * 255), (Uint8)(p_color.g * 255),
		(Uint8)(p_color.b * 255), (Uint8)(p_color.a * 255)));

	return (surface);
}

t_vect				second_degree_solver(double a, double b, double e, double c, double d, double f)
{
	// Equation type of :
	//	ax + by = e
	//	cx + dy = f
	double	x, y;

	double delta = a * d - b * c;
	if (delta != 0)
	{
		x = (e * d - b * f) / delta;
		y = (a * f - e * c) / delta;
	}
	else
		error_exit("Error on calc cell", 15642);
	return (t_vect(x, y));
}

vector<t_vect> 		calc_line_2d(double x1, double y1, double x2, double y2)
{
	vector<t_vect> result;
	const bool steep = (fabs(y2 - y1) > fabs(x2 - x1));
	if(steep)
	{
		swap(x1, y1);
		swap(x2, y2);
	}
	if(x1 > x2)
	{
		swap(x1, x2);
		swap(y1, y2);
	}

	double dx = x2 - x1;
	double dy = fabs(y2 - y1);

	double error = dx / 2.0f;
	int ystep = (y1 < y2) ? 1 : -1;
	int y = (int)y1;

	int maxX = (int)x2;

	for(int x=(int)x1; x<maxX; x++)
	{
		if(steep)
			result.push_back(t_vect(y,x));
		else
			result.push_back(t_vect(x,y));

		error -= dy;
		if(error < 0)
		{
			y += ystep;
			error += dx;
		}
	}
	return (result);
}

vector<t_vect> 		calc_line_2d(t_vect start, t_vect end)
{
	return (calc_line_2d(start.x, start.y, end.x, end.y));
}
