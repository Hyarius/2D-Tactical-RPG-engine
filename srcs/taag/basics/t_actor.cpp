#include "taag.h"

void					s_actor::draw_self(t_vect target, t_vect offset, t_vect size)
{
	if (selected == false && team >= 0 && team < 4)
		get_interface_tile("simple_cursor")->draw_self((coord + target) * size + offset, size, t_vect(team, 1));
	else if (selected == true)
		get_interface_tile("simple_cursor")->draw_self((coord + target) * size + offset, size, t_vect(2, 0));
	if (tile != NULL)
	{
		int type = get_frame_state(4);
		if (type == 3)
			type = 1;
		tile->draw_self((coord + target) * size + offset, size, sprite + t_vect(type, dir));
	}
	t_vect bar_coord = (coord + target) * size + offset + t_vect((size.x - size.x * 0.8) / 2, 0);
	t_vect bar_size = t_vect(size.x * 0.8, size.y / 10);
 	draw_rectangle(bar_coord, bar_size, t_color(0.0, 0.0, 0.0));
	draw_rectangle(bar_coord + 2, bar_size - 4, t_color(0.6, 0.6, 0.6));
	int percent = 100 * stat.hp.value / stat.hp.max;
	if (percent > 0)
		draw_rectangle(bar_coord + 2, t_vect(size.x * 0.8 * percent / 100, size.y / 10) - 4, t_color(1.0, 0.0, 0.0));
	if (stat.armor > 0)
	{
		bar_coord = (coord + target) * size + offset + t_vect((size.x - size.x * 0.8) / 2, -size.y / 10);
	 	draw_rectangle(bar_coord, bar_size, t_color(0.0, 0.0, 0.0));
		draw_rectangle(bar_coord + 2, bar_size - 4, t_color(0.6, 0.6, 0.6));
		percent = 100 * stat.armor / stat.hp.max;
		if (percent > 100)
			percent = 100;
		if (percent > 0)
			draw_rectangle(bar_coord + 2, t_vect(size.x * 0.8 * percent / 100, size.y / 10) - 4, t_color(34, 139, 34));
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
	tab = get_strsplit(&myfile, ":", -1);
	name = (tab.size() > 1 ? tab[1] : "");
	tile = get_sprite_tile(get_strsplit(&myfile, ":", 2)[1]);
	tab = get_strsplit(&myfile, ":", 3);
	sprite = t_vect(atoi(tab[1].c_str()), atoi(tab[2].c_str()));
	hp = t_value(atoi(get_strsplit(&myfile, ":", 2)[1].c_str()));
	pa = t_value(atoi(get_strsplit(&myfile, ":", 2)[1].c_str()));
	pm = t_value(atoi(get_strsplit(&myfile, ":", 2)[1].c_str()));
	init = atoi(get_strsplit(&myfile, ":", 2)[1].c_str());
	for (int i = 0; i < 6; i++)
		spell[i] = &(spell_map[get_strsplit(&myfile, ":", 2)[1]]);
	t_actor result = s_actor(name, tile, sprite, t_stat(hp, pa, pm, init), spell);
	if (!myfile.eof())
		result.gambit.clear();
	while (!(myfile.eof()))
	{
		tab = get_strsplit(&myfile, ":", -1);
		vector<int> value;
		for (size_t i = 0; i < tab.size(); i++)
			value.push_back(atoi(tab[i].c_str()));
		result.gambit.push_back(s_ai_helper(value));
	}
	return (result);
}

s_actor::s_actor()
{
	name = "";
	selected = false;
	tile = NULL;
	sprite = t_vect(0, 0);
	stat = t_stat(t_value(15), t_value(4), t_value(2), 5);
	dir = 0;
	effect_list = s_effect_list();
	visual_info = {};
	for (int i = 0; i < 6; i++)
	{
		spell[i] = &(spell_map["NULL"]);
		cooldown[i] = 0;
	}
	gambit.clear();
	for (int i = 0; i < 6; i++)
		spell_used[i] = 0;
	for (int i = 0; i < 24; i++)
		total_effect[i] = 0;
	gambit.push_back(s_ai_helper({7, 0, -1, -1}));
	gambit.push_back(s_ai_helper({7, 1, -1, -1}));
	gambit.push_back(s_ai_helper({7, 2, -1, -1}));
	gambit.push_back(s_ai_helper({7, 3, -1, -1}));
	gambit.push_back(s_ai_helper({7, 4, -1, -1}));
	gambit.push_back(s_ai_helper({7, 5, -1, -1}));
	gambit.push_back(s_ai_helper({-1}));
	gambit.push_back(s_ai_helper({3, 25, -1}));
	gambit.push_back(s_ai_helper({0, 1, 0, 0, -1}));
}

s_actor::s_actor(string p_name, t_tileset *p_tile, t_vect p_sprite, t_stat p_stat)
{
	name = p_name;
	selected = false;
	tile = p_tile;
	sprite = p_sprite;
	stat = p_stat;
	dir = 0;
	effect_list = s_effect_list();
	visual_info = {};
	for (int i = 0; i < 6; i++)
	{
		spell[i] = &(spell_map["NULL"]);
		cooldown[i] = 0;
	}
	gambit.clear();
	for (int i = 0; i < 6; i++)
		spell_used[i] = 0;
	for (int i = 0; i < 24; i++)
		total_effect[i] = 0;
}

s_actor::s_actor(string p_name, t_tileset *p_tile, t_vect p_sprite, t_stat p_stat, t_spell **p_spell)
{
	name = p_name;
	selected = false;
	tile = p_tile;
	sprite = p_sprite;
	stat = p_stat;
	dir = 0;
	effect_list = s_effect_list();
	visual_info = {};
	for (int i = 0; i < 6; i++)
	{
		spell[i] = p_spell[i];
		cooldown[i] = 0;
	}
	gambit.clear();
	for (int i = 0; i < 6; i++)
		spell_used[i] = 0;
	for (int i = 0; i < 24; i++)
		total_effect[i] = 0;
}

void			s_actor::reset_value()
{
	stat.pa.value = stat.pa.max;
	stat.pm.value = stat.pm.max;
}

void			s_actor::apply_effect(int type)
{

	size_t i = 0;
	t_action poison_effect = s_action(deal_dmg, 0, 0, 0, 0);
	t_action heal_effect = s_action(heal, 0, 0, 0, 0);
	t_action pa_effect = s_action(change_pa, 0, 0, 0, 0);
	t_action pm_effect = s_action(change_pm, 0, 0, 0, 0);
	while (i < this->effect_list.poison.size())
	{
		if (this->effect_list.poison[i].effect_type == type)
		{
			int damage = this->effect_list.poison[i].action[0].stat.value[0];
			poison_effect.stat.value[0] += damage;
			this->effect_list.poison[i].source->total_effect[12] += damage;
		}
		i++;
	}
	i = 0;
	while (i < this->effect_list.regeneration.size())
	{
		if (this->effect_list.regeneration[i].effect_type == type)
		{
			int damage = this->effect_list.regeneration[i].action[0].stat.value[0];
			heal_effect.stat.value[0] += damage;
			this->effect_list.regeneration[i].source->total_effect[13] += damage;
		}
		i++;
	}
	i = 0;
	while (i < this->effect_list.change_pa.size())
	{
		if (this->effect_list.change_pa[i].effect_type == type)
		{
			int damage = this->effect_list.change_pa[i].action[0].stat.value[0];
			pa_effect.stat.value[0] += damage;
			this->effect_list.change_pa[i].source->total_effect[(damage < 0 ? 14 : 16)] += damage;
		}
		i++;
	}
	i = 0;
	while (i < this->effect_list.change_pm.size())
	{
		if (this->effect_list.change_pm[i].effect_type == type)
		{
			int damage = this->effect_list.change_pm[i].action[0].stat.value[0];
			pm_effect.stat.value[0] += damage;
			this->effect_list.change_pm[i].source->total_effect[(damage < 0 ? 15 : 17)] += damage;
		}
		i++;
	}
	poison_effect.effect(NULL, this, poison_effect.stat);
	heal_effect.effect(NULL, this, heal_effect.stat);
	pa_effect.effect(NULL, this, pa_effect.stat);
	pm_effect.effect(NULL, this, pm_effect.stat);
}

void			s_actor::handle_effect_duration()
{
	size_t i = 0;
	while (i < this->effect_list.poison.size())
	{
		if (this->effect_list.poison[i].duration == 0)
			this->effect_list.poison.erase(this->effect_list.poison.begin() + i);
		else
		{
			this->effect_list.poison[i].duration--;
			i++;
		}
	}
	i = 0;
	while (i < this->effect_list.regeneration.size())
	{
		if (this->effect_list.regeneration[i].duration == 0)
			this->effect_list.regeneration.erase(this->effect_list.regeneration.begin() + i);
		else
		{
			this->effect_list.regeneration[i].duration--;
			i++;
		}
	}
	i = 0;
	while (i < this->effect_list.change_pa.size())
	{
		if (this->effect_list.change_pa[i].duration == 0)
			this->effect_list.change_pa.erase(this->effect_list.change_pa.begin() + i);
		else
		{
			this->effect_list.change_pa[i].duration--;
			i++;
		}
	}
	i = 0;
	while (i < this->effect_list.change_pm.size())
	{
		if (this->effect_list.change_pm[i].duration == 0)
			this->effect_list.change_pm.erase(this->effect_list.change_pm.begin() + i);
		else
		{
			this->effect_list.change_pm[i].duration--;
			i++;
		}
	}
}

void				s_actor::change_stat_hp(int value)
{
	change_stat_hp(value, RED);
}

void				s_actor::change_stat_pa(int value)
{
	change_stat_pa(value, BLUE);
}

void				s_actor::change_stat_pm(int value)
{
	change_stat_pm(value, GREEN);
}

void				s_actor::apply_effect(string text)
{
	apply_effect(text, GREY);
}

void				s_actor::change_stat_armor(int value)
{
	change_stat_armor(value, DARK_GREEN);
}

void				s_actor::change_stat_hp_ignore_armor(int value)
{
	change_stat_hp_ignore_armor(value, WHITE);
}

void				s_actor::change_stat_hp(int value, int color)
{
	if (value < 0)
	{
		total_effect[2] -= value;
		if (this->stat.armor >= -value)
		{
			this->stat.armor += value;
			this->visual_info.push_back(create_visual_info(to_string(value) + " armor", DARK_GREEN, 10, this->coord - t_vect(0.0, 0.5 * this->visual_info.size())));
			value = 0;
		}
		else
		{
			value += this->stat.armor;
			this->stat.armor = 0;
			this->stat.hp.value += value;
			this->visual_info.push_back(create_visual_info(to_string(value) + "hp", color, 10, this->coord - t_vect(0.0, 0.5 * this->visual_info.size())));
		}
	}
	else
	{
		this->stat.hp.value += value;
		this->visual_info.push_back(create_visual_info(to_string(value) + "hp", color, 10, this->coord - t_vect(0.0, 0.5 * this->visual_info.size())));
	}
}

void				s_actor::change_stat_pa(int value, int color)
{
	this->stat.pa.value += value;
	this->visual_info.push_back(create_visual_info(to_string(value) + "pa", color, 10, this->coord - t_vect(0.0, 0.5 * this->visual_info.size())));
}

void				s_actor::change_stat_pm(int value, int color)
{
	this->stat.pm.value += value;
	this->visual_info.push_back(create_visual_info(to_string(value) + "pm", color, 10, this->coord - t_vect(0.0, 0.5 * this->visual_info.size())));
}

void				s_actor::apply_effect(string text, int color)
{
	this->visual_info.push_back(create_visual_info(text, color, 10, this->coord - t_vect(0.0, 0.5 * this->visual_info.size())));
}

void				s_actor::change_stat_armor(int value, int color)
{
	if (value < 0 && -value > this->stat.armor)
		value = -(this->stat.armor);
	this->stat.armor += value;
	this->visual_info.push_back(create_visual_info(to_string(value) + " armor", color, 10, this->coord - t_vect(0.0, 0.5 * this->visual_info.size())));
}

void				s_actor::change_stat_hp_ignore_armor(int value, int color)
{
	this->stat.hp.value += value;
	this->visual_info.push_back(create_visual_info(to_string(value) + " true damage", color, 10, this->coord - t_vect(0.0, 0.5 * this->visual_info.size())));
}
