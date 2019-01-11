#include "taag.h"

#define TILESET_PATH "ressources/game_object/tileset/"
#define TILESET_EXT ".tls"

map<string, t_tileset>		tileset_map;

void				read_tileset()
{
	vector<string>	texture_file;
	ifstream		myfile;
	vector<string>	tab;
	string			name;
	string			path;
	t_vect			size;

	texture_file = list_files(TILESET_PATH, TILESET_EXT);
	size_t i = 0;
	while (i < texture_file.size())
	{
		myfile.open(TILESET_PATH + texture_file[i] + TILESET_EXT);
		if (myfile.fail())
			printf("can't open such file : %s%s%s\n", TILESET_PATH, texture_file[i].c_str(), TILESET_EXT);
		name = get_strsplit(&myfile, ":", 2)[1];
		path = get_strsplit(&myfile, ":", 2)[1];
		tab = get_strsplit(&myfile, ":", 3);
		size = t_vect(atoi(tab[1].c_str()), atoi(tab[2].c_str()));

		tileset_map[name] = t_tileset(path, size);

		myfile.close();

		i++;
	}

}
