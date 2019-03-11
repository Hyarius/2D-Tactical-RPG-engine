#include "template.h"

s_image::s_image()
{
	this->surface = NULL;
}

s_image::s_image(string path)
{
	this->surface = IMG_Load(path.c_str());
	if (this->surface == NULL)
		error_exit("Can't load " + path + " file");
	if (this->surface->format->BytesPerPixel == 3)
	{
		this->internal_format = GL_RGB;
		if (this->surface->format->Rmask == 0xff)
			this->format = GL_RGB;
		else
			this->format = GL_BGR;
	}
	else if (this->surface->format->BytesPerPixel == 4)
	{
		this->internal_format = GL_RGBA;
		if (this->surface->format->Rmask == 0xff)
			this->format = GL_RGBA;
		else
			this->format = GL_BGRA;
	}

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, this->internal_format, this->surface->w,
				this->surface->h, 0, this->format,
				GL_UNSIGNED_BYTE, this->surface->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
}

s_image::s_image(t_color p_color)
{
	this->surface = create_surface_color(p_color);
	if (this->surface == NULL)
		error_exit("Can't create a surface of color " + to_string((int)(p_color.r * 255)) + "/" + to_string((int)(p_color.g * 255)) + "/" + to_string((int)(p_color.b * 255)) + "/" + to_string((int)(p_color.a * 255)));
	if (this->surface->format->BytesPerPixel == 3)
	{
		this->internal_format = GL_RGB;
		if (this->surface->format->Rmask == 0xff)
			this->format = GL_RGB;
		else
			this->format = GL_BGR;
	}
	else if (this->surface->format->BytesPerPixel == 4)
	{
		this->internal_format = GL_RGBA;
		if (this->surface->format->Rmask == 0xff)
			this->format = GL_RGBA;
		else
			this->format = GL_BGRA;
	}

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, this->internal_format, this->surface->w,
				this->surface->h, 0, this->format,
				GL_UNSIGNED_BYTE, this->surface->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
}

s_image::s_image(SDL_Surface *p_surface)
{
	this->surface = p_surface;
	if (this->surface == NULL)
		error_exit("Surface send in t_image = NULL");
	if (this->surface->format->BytesPerPixel == 3)
	{
		this->internal_format = GL_RGB;
		if (this->surface->format->Rmask == 0xff)
			this->format = GL_RGB;
		else
			this->format = GL_BGR;
	}
	else if (this->surface->format->BytesPerPixel == 4)
	{
		this->internal_format = GL_RGBA;
		if (this->surface->format->Rmask == 0xff)
			this->format = GL_RGBA;
		else
			this->format = GL_BGRA;
	}

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, this->internal_format, this->surface->w,
				this->surface->h, 0, this->format,
				GL_UNSIGNED_BYTE, this->surface->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void				s_image::draw_self(t_vect p_coord, t_vect p_size)
{
	if (surface != NULL)
	{
		glBindTexture(GL_TEXTURE_2D, texture_id);
		draw_image(p_coord, p_size);
	}
	else
	{
		draw_rectangle(p_coord, p_size, t_color(0.2, 0.2, 0.2));
		draw_rectangle(p_coord + 4, p_size - 8, t_color(0.4, 0.4, 0.4));
	}
}

void				s_image::draw_centred_self(t_vect p_coord, t_vect p_size)
{
	if (surface != NULL)
	{
		glBindTexture(GL_TEXTURE_2D, texture_id);
		draw_image(p_coord - p_size / 2, p_size);
	}
	else
	{
		draw_rectangle(p_coord - p_size / 2, p_size, t_color(0.2, 0.2, 0.2));
		draw_rectangle(p_coord - p_size / 2 + 4, p_size - 8, t_color(0.4, 0.4, 0.4));
	}
}

void				s_image::draw_self(t_vect tl, t_vect tr, t_vect dl, t_vect dr)
{
	if (surface != NULL)
	{
		glBindTexture(GL_TEXTURE_2D, texture_id);
		draw_image(tl, tr, dl, dr, 1.0);
	}
	else
	{
		draw_rectangle(tl, tr, dl, dr, t_color(0.2, 0.2, 0.2));
	}
}
