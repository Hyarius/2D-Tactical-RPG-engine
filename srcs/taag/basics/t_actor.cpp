#include "taag.h"

void					s_actor::draw_self(t_vect target, t_vect offset, t_vect size)
{
	if (tile != NULL)
	{
		int type = get_frame_state(4);
		if (type == 3)
			type = 1;
		tile->draw_self((coord + target) * size + offset, size, sprite + t_vect(type, dir));
	}
	if (selected == false && team >= 0 && team < 4)
		interface_map["simple_cursor"].draw_self((coord + target) * size + offset, size, t_vect(team, 1));
	else if (selected == true)
		interface_map["simple_cursor"].draw_self((coord + target) * size + offset, size, t_vect(2, 0));
	t_vect bar_coord = (coord + target) * size + offset + t_vect((size.x - size.x * 0.8) / 2, 0);
	t_vect bar_size = t_vect(size.x * 0.8, size.y / 10);
 	draw_rectangle(bar_coord, bar_size, t_color(0.0, 0.0, 0.0));
	draw_rectangle(bar_coord + 2, bar_size - 4, t_color(0.6, 0.6, 0.6));
	int percent = 100 * stat.hp.value / stat.hp.max;
	if (percent > 0)
		draw_rectangle(bar_coord + 2, t_vect(size.x * 0.8 * percent / 100, size.y / 10) - 4, t_color(1.0, 0.0, 0.0));
}

void					s_actor::draw_visual_info(t_vect target, t_vect offset, t_vect size, double zoom)
{
	if (visual_info.size())
	{
		for (size_t i = 0; i < visual_info.size(); i++)
		{
			visual_info[i].draw_self(target, offset, size, zoom);
			if (visual_info[i].index == (int)visual_info[i].text_coord.size())
				visual_info.erase(visual_info.begin() + i);
		}
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
	tile = &(sprite_map[get_strsplit(&myfile, ":", 2)[1]]);
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
	sprite = t_vect(-1, -1);
	stat = t_stat(t_value(50), t_value(6), t_value(3), 5);
	dir = 0;
	for (int i = 0; i < 6; i++)
		spell[i] = &(spell_map["NULL"]);
}

s_actor::s_actor(string p_name, t_tileset *p_tile, t_vect p_sprite, t_stat p_stat)
{
	name = p_name;
	selected = false;
	tile = p_tile;
	sprite = p_sprite;
	stat = p_stat;
	dir = 0;
	for (int i = 0; i < 6; i++)
		spell[i] = &(spell_map["NULL"]);
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
