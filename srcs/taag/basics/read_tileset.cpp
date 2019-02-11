#include "taag.h"

#define TILESET_TILESET_PATH "ressources/tileset/tileset/"
#define TILESET_TILESET_EXT ".tls"
#define TILESET_CHARSET_PATH "ressources/tileset/charset/"
#define TILESET_CHARSET_EXT ".tls"
#define TILESET_INTERFACE_PATH "ressources/tileset/interface/"
#define TILESET_INTERFACE_EXT ".tls"
#define TILESET_ANIMATION_PATH "ressources/tileset/animation/"
#define TILESET_ANIMATION_EXT ".tls"

map<string, t_tileset>		tileset_map;
map<string, t_tileset>		sprite_map;
map<string, t_tileset>		interface_map;
map<string, t_tileset>		animation_map;

vector<string>				tileset_name;
vector<string>				sprite_name;
vector<string>				interface_name;
vector<string>				animation_name;

void				generate_charset_tileset()
{
	vector<string>	texture_file;
	ofstream		myfile;

	texture_file = list_files("ressources/tmp/", ".png");
	size_t i = 0;
	while (i < texture_file.size())
	{
		myfile.open(TILESET_CHARSET_PATH + texture_file[i] + TILESET_CHARSET_EXT);
		if (myfile.fail())
			printf("can't open such file : %s%s%s\n", TILESET_CHARSET_PATH, texture_file[i].c_str(), TILESET_CHARSET_EXT);
		printf("%s\n", texture_file[i].c_str());
		myfile << "tile_name:" + texture_file[i] + "\n";
		myfile << "file:ressources/assets/charset/" + texture_file[i] + ".png\n";
		myfile << "size:12:8\n";
		myfile.close();
		if (copy_file("ressources/tmp/" + texture_file[i] + ".png", "ressources/assets/charset/" + texture_file[i] + ".png") == false)
			error_exit("can't copy ressources/tmp/" + texture_file[i] + ".png to ressources/assets/charset/" + texture_file[i] + ".png", 15462);
		remove(("ressources/tmp/" + texture_file[i] + ".png").c_str());
		i++;
	}
}

static void			read_texture_folder(string p_path, string p_ext, map<string, struct s_tileset> *map, vector<string> *vector_name)
{
	vector<string>	texture_file;
	ifstream		myfile;
	vector<string>	tab;
	string			name;
	string			path;
	t_vect			size;

	texture_file = list_files(p_path, p_ext);
	size_t i = 0;
	while (i < texture_file.size())
	{
		myfile.open(p_path + texture_file[i] + p_ext);
		if (myfile.fail())
			printf("can't open such file : %s%s%s\n", p_path.c_str(), texture_file[i].c_str(), p_ext.c_str());
		name = get_strsplit(&myfile, ":", 2)[1];
		path = get_strsplit(&myfile, ":", 2)[1];
		tab = get_strsplit(&myfile, ":", 3);
		size = t_vect(atoi(tab[1].c_str()), atoi(tab[2].c_str()));

		(*map)[name] = t_tileset(path, size);
		vector_name->push_back(name);

		myfile.close();

		i++;
	}
}

void				read_tileset()
{
	read_texture_folder(TILESET_TILESET_PATH, TILESET_TILESET_EXT, &tileset_map, &tileset_name);
	read_texture_folder(TILESET_CHARSET_PATH, TILESET_CHARSET_EXT, &sprite_map, &sprite_name);
	read_texture_folder(TILESET_INTERFACE_PATH, TILESET_INTERFACE_EXT, &interface_map, &interface_name);
	read_texture_folder(TILESET_ANIMATION_PATH, TILESET_ANIMATION_EXT, &animation_map, &animation_name);

}
