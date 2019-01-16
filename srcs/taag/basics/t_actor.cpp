#include "taag.h"

void					s_actor::draw_self(t_vect target, t_vect offset, t_vect size)
{
	if (tile != NULL)
		tile->draw_self((coord + target) * size + offset, size, sprite + t_vect(get_frame_state(4), dir));
	if (selected == false && team >= 0 && team < 4)
		tileset_map["simple_cursor"].draw_self((coord + target) * size + offset, size, t_vect(team, 1));
	else if (selected == true)
		tileset_map["simple_cursor"].draw_self((coord + target) * size + offset, size, t_vect(2, 0));
	draw_rectangle((coord + target) * size + offset, t_vect(size.x, size.y / 15), t_color(0.6, 0.6, 0.6));
	int percent = 100 * stat.hp.value / stat.hp.max;
	draw_rectangle((coord + target) * size + offset, t_vect(size.x * percent / 100, size.y / 15), t_color(1.0, 0.0, 0.0));
}

void					s_actor::draw_visual_info(t_vect target, t_vect offset, t_vect size, double zoom)
{
	if (visual_info.size())
	{
		for (size_t i = 0; i < visual_info.size(); i++)
			visual_info[i].draw_self(target, offset, size, zoom);
	}
}

t_actor					read_actor(string p_path)
{
	ifstream		myfile;
	vector<string>	tab;
	string			name;
	t_tileset		*tile;
	t_vect			sprite;
	t_value			hp;
	t_value			pa;
	t_value			pm;
	t_spell			*spell[6];
	int				init;

	myfile.open(p_path);
	if (myfile.fail())
		printf("can't open such file : %s\n", p_path.c_str());
	name = get_strsplit(&myfile, ":", 2)[1];
	tile = &(tileset_map[get_strsplit(&myfile, ":", 2)[1]]);
	tab = get_strsplit(&myfile, ":", 3);
	sprite = t_vect(atoi(tab[1].c_str()), atoi(tab[2].c_str()));
	hp = t_value(atoi(get_strsplit(&myfile, ":", 2)[1].c_str()));
	pa = t_value(atoi(get_strsplit(&myfile, ":", 2)[1].c_str()));
	pm = t_value(atoi(get_strsplit(&myfile, ":", 2)[1].c_str()));
	init = atoi(get_strsplit(&myfile, ":", 2)[1].c_str());
	for (int i = 0; i < 6; i++)
		spell[i] = &(spell_map[get_strsplit(&myfile, ":", 2)[1]]);
	return (s_actor(name, tile, sprite, t_stat(hp, pa, pm, init), spell));
}

s_actor::s_actor()
{
	name = "";
	selected = false;
	tile = NULL;
	dir = 0;
	sprite = t_vect(-1, -1);
}

s_actor::s_actor(string p_name, t_tileset *p_tile, t_vect p_sprite, t_stat p_stat, t_spell **p_spell)
{
	name = p_name;
	selected = false;
	tile = p_tile;
	sprite = p_sprite;
	stat = p_stat;
	dir = 0;
	for (int i = 0; i < 6; i++)
		spell[i] = p_spell[i];
}

void			s_actor::reset_value()
{
	stat.pa.value = stat.pa.max;
	stat.pm.value = stat.pm.max;
}
