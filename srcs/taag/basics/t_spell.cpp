#include "taag.h"

#define SPELL_PATH "ressources/spell/"
#define SPELL_EXT ".spell"

map<string, t_spell>	spell_map;
vector<string>			spell_name;
vector<string>			spell_heros_name;

s_spell::s_spell()
{
	name = "NULL";
	desc = "";
	m_spell = false;
	tile = NULL;
	icon = t_vect(-1, -1);
	cost_pa = 0;
	cost_pm = 0;
	range[0] = -1;
	range[1] = -1;
	block = true;
	on_target = 0;
	range_type = R_CIRCLE;
	zone_type = Z_CROSS;
	zone_size = 0;
}

s_spell::s_spell(	string p_name, string p_desc, t_tileset *p_tile, t_vect p_icon, bool p_m_spell,
					int p_cost_pa, int p_cost_pm, int range_min, int range_max, bool p_block, int p_on_target,
					e_range_type p_range_type, e_zone_type p_zone_type, int p_zone_size,
					vector<t_effect> p_effect)
{
	name = p_name;
	desc = p_desc;
	m_spell = p_m_spell;
	tile = p_tile;
	icon = p_icon;
	cost_pa = p_cost_pa;
	cost_pm = p_cost_pm;
	range[0] = range_min;
	range[1] = range_max;
	block = p_block;
	on_target = p_on_target;
	range_type = p_range_type;
	zone_type = p_zone_type;
	zone_size = p_zone_size;
	effect = p_effect;
}

void		read_spell()
{
	vector<string>	spell_file;
	ifstream		myfile;
	vector<string>	tab;
	string			name;
	string			desc;
	t_tileset		*tile;
	t_vect			icon;
	bool			m_spell;
	int				cost_pa;
	int				cost_pm;
	int				range[2];
	bool			block;
	int				on_target;
	e_range_type	range_type;
	e_zone_type		zone_type;
	int				zone_size;
	vector<t_effect> effect;

	spell_file = list_files(SPELL_PATH, SPELL_EXT);
	spell_map["NULL"] = t_spell();
	spell_name.push_back("NULL");
	spell_heros_name.push_back("NULL");
	size_t i = 0;
	while (i < spell_file.size())
	{
		myfile.open(SPELL_PATH + spell_file[i] + SPELL_EXT);
		if (myfile.fail())
			printf("can't open such file : %s%s%s\n", SPELL_PATH, spell_file[i].c_str(), SPELL_EXT);
		name = get_strsplit(&myfile, ":", 2)[1];
		desc = get_strsplit(&myfile, ":", 2)[1];
		tile = &(interface_map[get_strsplit(&myfile, ":", 2)[1]]);
		tab = get_strsplit(&myfile, ":", 3);
		icon = t_vect(atoi(tab[1].c_str()), atoi(tab[2].c_str()));
		m_spell = (atoi(get_strsplit(&myfile, ":", 2)[1].c_str()) == 0 ? false : true);
		cost_pa = atoi(get_strsplit(&myfile, ":", 2)[1].c_str());
		cost_pm = atoi(get_strsplit(&myfile, ":", 2)[1].c_str());
		tab = get_strsplit(&myfile, ":", 4);
		range[0] = atoi(tab[1].c_str());
		range[1] = atoi(tab[2].c_str());
		block = (atoi(tab[3].c_str()) == 0 ? true : false);
		range_type = (e_range_type)(atoi(get_strsplit(&myfile, ":", 2)[1].c_str()));
		on_target = atoi(get_strsplit(&myfile, ":", 2)[1].c_str());
		tab = get_strsplit(&myfile, ":", 3);
		zone_type = (e_zone_type)(atoi(tab[1].c_str()));
		zone_size = (e_zone_type)(atoi(tab[2].c_str()));
		while (!myfile.eof())
		{
			tab = get_strsplit(&myfile, ":", -1);
			if (tab.size() == 6)
				effect.push_back(t_effect(g_effects[atoi(tab[1].c_str())], atof(tab[2].c_str()), atof(tab[3].c_str()), atof(tab[4].c_str()), atof(tab[5].c_str())));
		}
		spell_map[name] = t_spell(name, desc, tile, icon, m_spell, cost_pa, cost_pm, range[0], range[1], block, on_target, range_type, zone_type, zone_size, effect);
		if (m_spell == false)
			spell_heros_name.push_back(name);
		spell_name.push_back(name);
		effect.clear();
		myfile.close();

		i++;
	}
}
