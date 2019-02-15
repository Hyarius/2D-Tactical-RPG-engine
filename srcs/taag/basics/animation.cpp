#include "taag.h"

void				generate_animation_tileset()
{
	vector<string>	texture_file;
	ofstream		myfile;
	SDL_Surface		*surface;
	t_vect			sprite_size = t_vect(192, 192);

	texture_file = list_files("ressources/tmp/", ".png");
	size_t i = 0;
	while (i < texture_file.size())
	{
		myfile.open(TILESET_ANIMATION_PATH + texture_file[i] + TILESET_ANIMATION_EXT);
		if (myfile.fail())
			printf("can't open such file : %s%s%s\n", TILESET_ANIMATION_PATH, texture_file[i].c_str(), TILESET_ANIMATION_EXT);
		surface = IMG_Load(("ressources/tmp/" + texture_file[i] + ".png").c_str());
		t_vect size = t_vect(surface->w / sprite_size.x, surface->h / sprite_size.y);
		printf("%s - %.2f / %.2f\n", texture_file[i].c_str(), size.x, size.y);
		myfile << "tile_name:" + texture_file[i] + "\n";
		myfile << "file:ressources/assets/animation/" + texture_file[i] + ".png\n";
		myfile << "size:" + to_string((int)(size.x)) + ":" + to_string((int)(size.y)) + "\n";
		myfile.close();
		SDL_FreeSurface(surface);
		if (copy_file("ressources/tmp/" + texture_file[i] + ".png", "ressources/assets/animation/" + texture_file[i] + ".png") == false)
			error_exit("can't copy ressources/tmp/" + texture_file[i] + ".png to " + "ressources/assets/animation/" + texture_file[i] + ".png", 15462);
		remove(("ressources/tmp/" + texture_file[i] + ".png").c_str());
		i++;
	}
}

s_animation::s_animation()
{
	tile = NULL;
	start = 0;
	len = 0;
	index = 0;
	anim_size = t_vect(1, 1);
}

s_animation::s_animation(t_tileset *p_tile, int p_start, int p_len, t_vect p_size)
{
	tile = p_tile;
	start = p_start;
	len = p_len;
	index = 0;
	anim_size = p_size;
}

void			s_animation::draw_self(t_vect coord, t_vect target, t_vect offset, t_vect size)
{
	if (index < len)
	{
		t_vect reel_size = size * anim_size;
		t_vect reel_coord = (coord + target) * size + offset - reel_size / 2;
		tile->draw_self(reel_coord, reel_size, index + start);
		index += (double)len / ((60.0 / 15) * len);
	}
}
