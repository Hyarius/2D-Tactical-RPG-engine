#include "taag.h"

map<string, t_tileset>		tileset_map;

vector<string>				tileset_name;
vector<string>				sprite_name;
vector<string>				human_sprite_name;
vector<string>				interface_name;
vector<string>				animation_name;

t_tileset			*get_tileset_tile(string p_name)
{
	if (tileset_map[p_name].surface == NULL)
	{
		ifstream myfile;

		myfile.open(TILESET_TILESET_PATH + p_name + TILESET_TILESET_EXT);
		if (myfile.fail())
			error_exit("Can't open such tileset : " + TILESET_TILESET_PATH + p_name + TILESET_TILESET_EXT);
		string name = get_strsplit(&myfile, ":", 2)[1];
		string path = get_strsplit(&myfile, ":", 2)[1];
		vector<string> tab = get_strsplit(&myfile, ":", 3);
		t_vect size = t_vect(atoi(tab[1].c_str()), atoi(tab[2].c_str()));

		tileset_map[name] = t_tileset(path, size);

		myfile.close();
	}
	return (&(tileset_map[p_name]));
}

t_tileset			*get_tileset_tile(size_t name_num)
{
	if (name_num >= tileset_name.size())
		return (NULL);
	return (get_tileset_tile(tileset_name[name_num]));
}


t_tileset			*get_sprite_tile(string p_name)
{
	if (tileset_map[p_name].surface == NULL)
	{
		ifstream myfile;

		myfile.open(TILESET_CHARSET_PATH + p_name + TILESET_CHARSET_EXT);
		if (myfile.fail())
			error_exit("Can't open such tileset : " + TILESET_CHARSET_PATH + p_name + TILESET_CHARSET_EXT);
		string name = get_strsplit(&myfile, ":", 2)[1];
		string path = get_strsplit(&myfile, ":", 2)[1];
		vector<string> tab = get_strsplit(&myfile, ":", 3);
		t_vect size = t_vect(atoi(tab[1].c_str()), atoi(tab[2].c_str()));

		tileset_map[p_name] = t_tileset(path, size);

		myfile.close();
	}
	return (&(tileset_map[p_name]));
}

t_tileset			*get_sprite_tile(size_t name_num)
{
	if (name_num >= sprite_name.size())
		return (NULL);
	return (get_sprite_tile(sprite_name[name_num]));
}

t_tileset			*get_interface_tile(string p_name)
{
	if (tileset_map[p_name].surface == NULL)
	{
		ifstream myfile;

		myfile.open(TILESET_INTERFACE_PATH + p_name + TILESET_INTERFACE_EXT);
		if (myfile.fail())
			error_exit("Can't open such tileset : " + TILESET_INTERFACE_PATH + p_name + TILESET_INTERFACE_EXT);
		string name = get_strsplit(&myfile, ":", 2)[1];
		string path = get_strsplit(&myfile, ":", 2)[1];
		vector<string> tab = get_strsplit(&myfile, ":", 3);
		t_vect size = t_vect(atoi(tab[1].c_str()), atoi(tab[2].c_str()));

		tileset_map[name] = t_tileset(path, size);

		myfile.close();
	}
	return (&(tileset_map[p_name]));
}

t_tileset			*get_interface_tile(size_t name_num)
{
	if (name_num >= interface_name.size())
		return (NULL);
	return (get_interface_tile(interface_name[name_num]));
}

t_tileset			*get_animation_tile(string p_name)
{
	if (tileset_map[p_name].surface == NULL)
	{
		ifstream myfile;

		myfile.open(TILESET_ANIMATION_PATH + p_name + TILESET_ANIMATION_EXT);
		if (myfile.fail())
			error_exit("Can't open such tileset : " + TILESET_ANIMATION_PATH + p_name + TILESET_ANIMATION_EXT);
		string name = get_strsplit(&myfile, ":", 2)[1];
		string path = get_strsplit(&myfile, ":", 2)[1];
		vector<string> tab = get_strsplit(&myfile, ":", 3);
		t_vect size = t_vect(atoi(tab[1].c_str()), atoi(tab[2].c_str()));

		tileset_map[name] = t_tileset(path, size);

		myfile.close();
	}
	return (&(tileset_map[p_name]));
}

t_tileset			*get_animation_tile(size_t name_num)
{
	if (name_num >= animation_name.size())
		return (NULL);
	return (get_animation_tile(animation_name[name_num]));
}


void				generate_charset_tileset()
{
	vector<string>	texture_file;
	ofstream		myfile;

	texture_file = list_files(TMP_PATH, ".png");
	size_t i = 0;
	while (i < texture_file.size())
	{
		myfile.open(TILESET_CHARSET_PATH + texture_file[i] + TILESET_CHARSET_EXT);
		if (myfile.fail())
			error_exit("Can't generate such charset tileset : " + TILESET_CHARSET_PATH + texture_file[i] + TILESET_CHARSET_EXT);
		printf("%s\n", texture_file[i].c_str());
		myfile << "tile_name:" + texture_file[i] + "\n";
		myfile << string("file:") + TILESET_CHARSET_PATH + texture_file[i] + ".png\n";
		myfile << "size:12:8\n";
		myfile.close();
		if (copy_file(TMP_PATH + texture_file[i] + ".png", TILESET_CHARSET_PATH + texture_file[i] + ".png") == false)
			error_exit("Can't copy " + TMP_PATH + texture_file[i] + ".png into " + TILESET_CHARSET_PATH + texture_file[i] + ".png inside generate_charset_tileset");
		remove((TMP_PATH + texture_file[i] + ".png").c_str());
		i++;
	}
}

static void			read_texture_folder(string p_path, string p_ext, vector<string> *vector_name)
{
	vector<string>	texture_file;

	texture_file = list_files(p_path, p_ext);
	size_t i = 0;
	while (i < texture_file.size())
	{
		vector_name->push_back(texture_file[i]);
		if (vector_name == &sprite_name && texture_file[i][0] == 'H')
			human_sprite_name.push_back(texture_file[i]);
		i++;
	}
}

void				read_tileset()
{
	write_in_log("Reading " + TILESET_TILESET_PATH + "folder - looking for " + TILESET_TILESET_EXT + " : ");
	read_texture_folder(TILESET_TILESET_PATH, TILESET_TILESET_EXT, &tileset_name);
	write_in_log("Reading complete\n");
	write_in_log("Reading " + TILESET_CHARSET_PATH + "folder - looking for " + TILESET_CHARSET_EXT + " : ");
	read_texture_folder(TILESET_CHARSET_PATH, TILESET_CHARSET_EXT, &sprite_name);
	write_in_log("Reading complete\n");
	write_in_log("Reading " + TILESET_INTERFACE_PATH + "folder - looking for " + TILESET_INTERFACE_EXT + " : ");
	read_texture_folder(TILESET_INTERFACE_PATH, TILESET_INTERFACE_EXT, &interface_name);
	write_in_log("Reading complete\n");
	write_in_log("Reading " + TILESET_ANIMATION_PATH + "folder - looking for " + TILESET_ANIMATION_EXT + " : ");
	read_texture_folder(TILESET_ANIMATION_PATH, TILESET_ANIMATION_EXT, &animation_name);
	write_in_log("Reading complete\n");
}
