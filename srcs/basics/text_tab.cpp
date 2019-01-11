#include "template.h"

map<string, string>			text_map;

void				set_lang_text(string path)
{
	ifstream		file;
	vector<string>	line;
	int				i = 0;

	file.open(path);

	if (text_map.size() != 0)
		text_map.clear();
	while (file.eof() == false)
	{
		line = get_strsplit(&file, ";", 2);
		if (line.size() != 0)
			text_map[line[0]] = line[1];
		i++;
	}
}

string				*get_text(string key)
{
	if (text_map[key] == "")
		return (NULL);
	return (&(text_map[key]));
}
