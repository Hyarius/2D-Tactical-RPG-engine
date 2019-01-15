#include "taag.h"

#define SPELL_PATH "ressources/spell/"
#define SPELL_EXT ".spell"

map<string, t_spell>	spell_map;

s_spell::s_spell()
{
	name = "NULL";
	desc = "";
	tile = NULL;
	icon = t_vect(-1, -1);
	cost_pa = 0;
	cost_pm = 0;
	range[0] = -1;
	range[1] = -1;
	type = CIRCLE;
}

s_spell::s_spell(	string p_name, string p_desc, t_tileset *p_tile, t_vect p_icon,
					int p_cost_pa, int p_cost_pm, int range_min, int range_max,
					e_range_type p_type)
{
	name = p_name;
	desc = p_desc;
	tile = p_tile;
	icon = p_icon;
	cost_pa = p_cost_pa;
	cost_pm = p_cost_pm;
	range[0] = range_min;
	range[1] = range_max;
	type = p_type;
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
	int				cost_pa;
	int				cost_pm;
	int				range[2];
	e_range_type	type;

	spell_file = list_files(SPELL_PATH, SPELL_EXT);
	size_t i = 0;
	while (i < spell_file.size())
	{
		myfile.open(SPELL_PATH + spell_file[i] + SPELL_EXT);
		if (myfile.fail())
			printf("can't open such file : %s%s%s\n", SPELL_PATH, spell_file[i].c_str(), SPELL_EXT);
		name = get_strsplit(&myfile, ":", 2)[1];
		desc = get_strsplit(&myfile, ":", 2)[1];
		tile = &(tileset_map[get_strsplit(&myfile, ":", 2)[1]]);
		tab = get_strsplit(&myfile, ":", 3);
		icon = t_vect(atoi(tab[1].c_str()), atoi(tab[2].c_str()));
		cost_pa = atoi(get_strsplit(&myfile, ":", 2)[1].c_str());
		cost_pm = atoi(get_strsplit(&myfile, ":", 2)[1].c_str());
		tab = get_strsplit(&myfile, ":", 3);
		range[0] = atoi(tab[1].c_str());
		range[1] = atoi(tab[2].c_str());
		type = (e_range_type)(atoi(get_strsplit(&myfile, ":", 2)[1].c_str()));

		spell_map[name] = t_spell(name, desc, tile, icon, cost_pa, cost_pm, range[0], range[1], type);

		myfile.close();

		i++;
	}
}
