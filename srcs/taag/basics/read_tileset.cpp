#include "taag.h"

#define TILESET_TILESET_PATH "ressources/tileset/tileset/"
#define TILESET_TILESET_EXT ".tls"
#define TILESET_CHARSET_PATH "ressources/tileset/charset/"
#define TILESET_CHARSET_EXT ".tls"
#define TILESET_INTERFACE_PATH "ressources/tileset/interface/"
#define TILESET_INTERFACE_EXT ".tls"

map<string, t_tileset>		tileset_map;
map<string, t_tileset>		sprite_map;
map<string, t_tileset>		interface_map;

vector<string>				tileset_name;
vector<string>				sprite_name;
vector<string>				interface_name;

void				read_tileset()
{
	vector<string>	texture_file;
	ifstream		myfile;
	vector<string>	tab;
	string			name;
	string			path;
	t_vect			size;

	texture_file = list_files(TILESET_TILESET_PATH, TILESET_TILESET_EXT);
	size_t i = 0;
	while (i < texture_file.size())
	{
		myfile.open(TILESET_TILESET_PATH + texture_file[i] + TILESET_TILESET_EXT);
		if (myfile.fail())
			printf("can't open such file : %s%s%s\n", TILESET_TILESET_PATH, texture_file[i].c_str(), TILESET_TILESET_EXT);
		name = get_strsplit(&myfile, ":", 2)[1];
		path = get_strsplit(&myfile, ":", 2)[1];
		tab = get_strsplit(&myfile, ":", 3);
		size = t_vect(atoi(tab[1].c_str()), atoi(tab[2].c_str()));

		tileset_map[name] = t_tileset(path, size);
		tileset_name.push_back(name);
		myfile.close();

		i++;
	}
	texture_file = list_files(TILESET_CHARSET_PATH, TILESET_CHARSET_EXT);
	i = 0;
	while (i < texture_file.size())
	{
		myfile.open(TILESET_CHARSET_PATH + texture_file[i] + TILESET_CHARSET_EXT);
		if (myfile.fail())
			printf("can't open such file : %s%s%s\n", TILESET_CHARSET_PATH, texture_file[i].c_str(), TILESET_CHARSET_EXT);
		name = get_strsplit(&myfile, ":", 2)[1];
		path = get_strsplit(&myfile, ":", 2)[1];
		tab = get_strsplit(&myfile, ":", 3);
		size = t_vect(atoi(tab[1].c_str()), atoi(tab[2].c_str()));

		sprite_map[name] = t_tileset(path, size);
		sprite_name.push_back(name);

		myfile.close();

		i++;
	}
	texture_file = list_files(TILESET_INTERFACE_PATH, TILESET_INTERFACE_EXT);
	i = 0;
	while (i < texture_file.size())
	{
		myfile.open(TILESET_INTERFACE_PATH + texture_file[i] + TILESET_INTERFACE_EXT);
		if (myfile.fail())
			printf("can't open such file : %s%s%s\n", TILESET_INTERFACE_PATH, texture_file[i].c_str(), TILESET_INTERFACE_EXT);
		name = get_strsplit(&myfile, ":", 2)[1];
		path = get_strsplit(&myfile, ":", 2)[1];
		tab = get_strsplit(&myfile, ":", 3);
		size = t_vect(atoi(tab[1].c_str()), atoi(tab[2].c_str()));

		interface_map[name] = t_tileset(path, size);
		interface_name.push_back(name);

		myfile.close();

		i++;
	}

}
