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
/*
name:Grass tile - type 0
tileset:simple_tile
sprite:0:0
cost:1
m_obstacle:0
v_obstacle:0
*/
void			generate_node_file()
{
	vector<string>	texture_file;
	ofstream		myfile;

	string	name[8] = {
		"Grass tile",
		"Sand tile",
		"Dirt tile",
		"Water tile",
		"Stone tile",
		"Tiled tile",
		"??? tile",
		"??? tile",
	};
	string	type[4] = {
		" - empty",
		" - Slowed",
		" - Move blocked",
		" - Move and vision blocked"
	};

	size_t i = 0;
	while (i < 8)
	{
		size_t j = 0;
		while (j < 10)
		{
			string text = to_string(i * 10 + j);
			if (text.size() != 2)
				text = "0" + text;
			myfile.open(NODE_PATH + text + NODE_EXT);
			if (myfile.fail())
				printf("can't open such file : %s%s%s\n", NODE_PATH, text.c_str(), NODE_EXT);
			string text_type = (j == 3 ? type[1] : (j == 4 || j == 5 ? type[2] : (j == 6 || j == 7 || j == 8 || j == 9 ? type[3] : type[0])));
			myfile << "name:" + name[i] + text_type + "\n";
			myfile << "tileset:simple_tile\n";
			myfile << "sprite:" + to_string(i) + ":" + to_string(j) + "\n";
			int cost = (i == 3 ? 2 : 1) + (j == 3 ? 1 : 0);
			myfile << "cost:" + to_string(cost) + "\n";
			int m_obs = (j == 4 || j == 5 || j == 6 || j == 7 || j == 8 || j == 9 ? 1 : 0);
			myfile << "m_obstacle:" + to_string(m_obs) + "\n";
			int v_obs = (j == 6 || j == 7 || j == 8 || j == 9 ? 1 : 0);
			myfile << "v_obstacle:" + to_string(v_obs) + "\n";
			myfile.close();
			j++;
		}
		i++;
	}
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

	return(t_node(name, get_tileset_tile(tile_name), sprite, cost, m_obs, v_obs));
}
