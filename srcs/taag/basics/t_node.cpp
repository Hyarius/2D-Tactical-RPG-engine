#include "taag.h"

s_node::s_node()
{
	name = "Empty tile";
	tile = NULL;
	sprite = t_vect(- 1, -1);
	cost = 0;
	m_obs = 1;
	v_obs = 0;
}

s_node::s_node(string p_name, t_tileset *p_tile, t_vect p_sprite, int p_cost, bool p_m_obs, bool p_v_obs)
{
	name = p_name;
	tile = p_tile;
	sprite = p_sprite;
	cost = p_cost;
	m_obs = p_m_obs;
	v_obs = p_v_obs;
}

t_node			read_node(string p_path)
{
	ifstream		myfile;
	vector<string>	tab;
	string			name;
	string			tile_name;
	t_vect			sprite;
	int				cost;
	bool			m_obs;
	bool			v_obs;


	myfile.open(p_path);
	if (myfile.fail())
		printf("can't open such file : %s\n", p_path.c_str());
	name = get_strsplit(&myfile, ":", 2)[1];
	tile_name = get_strsplit(&myfile, ":", 2)[1];
	tab = get_strsplit(&myfile, ":", 3);
	sprite = t_vect(atoi(tab[1].c_str()), atoi(tab[2].c_str()));
	cost = atoi(get_strsplit(&myfile, ":", 2)[1].c_str());
	m_obs = (atoi(get_strsplit(&myfile, ":", 2)[1].c_str()) == 1 ? true : false);
	v_obs = (atoi(get_strsplit(&myfile, ":", 2)[1].c_str()) == 1 ? true : false);

	if (tileset_map.find(tile_name) == tileset_map.end())
		return (t_node(name, NULL, sprite, cost, m_obs, v_obs));
	return(t_node(name, &(tileset_map[tile_name]), sprite, cost, m_obs, v_obs));
}
