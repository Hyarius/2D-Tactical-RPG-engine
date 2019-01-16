#include "template.h"

int				draw_text(string text, int text_size, t_vect coord, int color_type)
{
	t_image			*image;
	size_t			i = 0;
	t_vect			rel_coord;
	int				delta = 0;

	while (i < text.length())
	{
		rel_coord = t_vect(coord.x + delta, coord.y);
		image = get_char(text_size, color_type, text[i]);
		image->draw_self(rel_coord, t_vect(image->surface->w, image->surface->h));
		delta += image->surface->w;
		i++;
	}
	return (delta);
}

int				draw_text(string text, int text_size, int type, t_vect coord, int color_type)
{
	t_image			*image;
	size_t			i = 0;
	t_vect			rel_coord;
	int				delta = 0;

	while (i < text.length())
	{
		rel_coord = t_vect(coord.x + delta, coord.y);
		image = get_char(type, text_size, color_type, text[i]);
		image->draw_self(rel_coord, t_vect(image->surface->w, image->surface->h));
		delta += image->surface->w;
		i++;
	}
	return (delta);
}

int				draw_text(string text, int text_size, int type, int outline, t_vect coord, int color_type, int color_type2)
{
	t_image			*image;
	size_t			i = 0;
	t_vect			rel_coord;
	int				delta = 0;

	while (i < text.length())
	{
		rel_coord = t_vect(coord.x + delta, coord.y);
		image = get_char(type, text_size, outline, color_type2, text[i]);
		image->draw_self(rel_coord, t_vect(image->surface->w, image->surface->h));
		image = get_char(type, text_size, 0, color_type, text[i]);
		image->draw_self(rel_coord + outline, t_vect(image->surface->w, image->surface->h));
		delta += image->surface->w;
		i++;
	}
	return (delta);
}

int				calc_text_len(string text, int text_size)
{
	t_image			*image;
	size_t			i = 0;
	int				delta = 0;

	while (i < text.length())
	{
		image = get_char(text_size, BLACK, text[i]);
		delta += image->surface->w;
		i++;
	}
	return (delta);
}

int				calc_text_max_size(string text, t_vect size)
{
	int i = 1;

	while (calc_text_len(text, i + 1) < size.x &&
			get_char(i, BLACK, 'M')->surface->h < size.y)
		i++;
	return (i);
}

int				draw_centred_text(string text, int text_size, t_vect coord, int color_type)
{
	int x = calc_text_len(text, text_size);
	int y = get_char(text_size, color_type, 'M')->surface->h;

	return (draw_text(text, text_size, t_vect(coord.x - x / 2, coord.y - y / 2), color_type));
}

int				draw_centred_text(string text, int text_size, int type, t_vect coord, int color_type)
{
	int x = calc_text_len(text, text_size);
	int y = get_char(text_size, color_type, 'M')->surface->h;

	return (draw_text(text, text_size, type, t_vect(coord.x - x / 2, coord.y - y / 2), color_type));
}

int				draw_centred_text(string text, int text_size, int type, int outline, t_vect coord, int color_type, int color_type2)
{
	int x = calc_text_len(text, text_size);
	int y = get_char(text_size, color_type, 'M')->surface->h;

	return (draw_text(text, text_size, type, outline, t_vect(coord.x - x / 2, coord.y - y / 2), color_type, color_type2));
}

int				draw_lined_text(string text, int text_size, t_vect coord, int color_type)
{
	int y = get_char(text_size, color_type, 'M')->surface->h;

	return (draw_text(text, text_size, coord + t_vect(0, (-y / 2)), color_type));
}

int				draw_lined_text(string text, int text_size, int type, t_vect coord, int color_type)
{
	int y = get_char(text_size, color_type, 'M')->surface->h;

	return (draw_text(text, text_size, type, coord + t_vect(0, (-y / 2)), color_type));
}

int				draw_lined_text(string text, int text_size, int type, int outline, t_vect coord, int color_type, int color_type2)
{
	int y = get_char(text_size, color_type, 'M')->surface->h;

	return (draw_text(text, text_size, type, outline, coord + t_vect(0, (-y / 2)), color_type, color_type2));
}

static bool			verify_paragraphe_size(vector<string> line, t_vect size, int text_size)
{
	t_vect	tmp = t_vect(0, 0);
	size_t	i = 0;

	while (i < line.size())
	{
		if (calc_text_len(line[i], text_size) > size.x)
			return (false);
		if (tmp.y + get_char(text_size, BLACK, 'M')->surface->h > size.y)
			return (false);
		if (tmp.x + calc_text_len(line[i], text_size) >= size.x)
		{
			tmp.x = 0;
			tmp.y = tmp.y + get_char(text_size, BLACK, 'M')->surface->h;
			if (tmp.y + get_char(text_size, BLACK, 'M')->surface->h > size.y)
				return (false);
		}
		tmp.x += calc_text_len(line[i] + " ", text_size);
		i++;
	}
	return (true);
}

int					calc_paragraphe_size(string text, t_vect size)
{
	int				i = 1;
	vector<string>	line;

	if (text.size() == 0)
		return (0);
	line = strsplit(text, " ");
	while (verify_paragraphe_size(line, size, i + 100) == true)
		i += 100;
	while (verify_paragraphe_size(line, size, i + 50) == true)
		i += 50;
	while (verify_paragraphe_size(line, size, i + 25) == true)
		i += 25;
	while (verify_paragraphe_size(line, size, i + 10) == true)
		i += 10;
	while (verify_paragraphe_size(line, size, i + 1) == true)
		i += 1;

	return (i);
}

void				draw_paragraphe(string text, t_vect coord, t_vect size, int color_type)
{
	size_t			i = 0;
	vector<string>	line;
	t_vect			tmp = t_vect(0, 0);
	size_t			text_size;

	if (text == "")
		return ;
	line = strsplit(text, " ");
	text_size = calc_paragraphe_size(text, size);
	while (i < line.size())
	{
		if (tmp.x + calc_text_len(line[i], text_size) > size.x)
		{
			tmp.x = 0;
			tmp.y = tmp.y + get_char(text_size, BLACK, 'M')->surface->h;
		}
		tmp.x += draw_text(line[i] + " ", text_size, tmp + coord, color_type);
		i++;
	}
}

void				draw_paragraphe(string text, int text_size, t_vect coord, t_vect size, int color_type)
{
	size_t			i = 0;
	vector<string>	line;
	t_vect			tmp = t_vect(0, 0);

	if (text == "")
		return ;
	line = strsplit(text, " ");
	while (i < line.size())
	{
		if (tmp.x + calc_text_len(line[i], text_size) > size.x)
		{
			tmp.x = 0;
			tmp.y = tmp.y + get_char(text_size, BLACK, 'M')->surface->h;
		}
		tmp.x += draw_text(line[i] + " ", text_size, tmp + coord, color_type);
		i++;
	}
}
