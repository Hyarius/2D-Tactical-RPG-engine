#include "template.h"

string					get_str(ifstream *myfile)
{
    string line;

	if (myfile->eof())
		return ("");
    getline(*myfile, line);
	if (line.size() == 0)
		return ("");
    if (line[strlen((char *)line.c_str()) - 1] == '\n')
        line[strlen((char *)line.c_str()) - 1] = '\0';
    return (line);
}

vector<string>			get_strsplit(ifstream *myfile, const string c, int size)
{
    vector<string> tab;
    string line;
    string saved_line;
    char	*word;
	char	*context = NULL;

    line = get_str(myfile);
	if (line == "")
	{
		if (size != -1)
			error_exit(get_file_info(__FILE__, __LINE__) + " - Empty line in get_strsplit when espect a size");
		tab.resize(0);
		return (tab);
	}
	saved_line = line;
    word = strtok_r((char *)(line.c_str()), c.c_str(), &context);
    while(word != NULL)
    {
        tab.push_back(word);
        word = strtok_r(NULL, c.c_str(), &context);
    }
	if (size != -1 && tab.size() != (size_t)size)
		error_exit(get_file_info(__FILE__, __LINE__) + " - Bad lenght in get_strsplit");
    return tab;
}

vector<string>		list_files(string path, string extension)
{
	vector<string>	brut_files;
	vector<string>	files;
	string			line;
	DIR				*dir = opendir(path.c_str());

	if (dir == NULL)
		error_exit(get_file_info(__FILE__, __LINE__) + " - Folder " + path + " didn't exist");
	char			*context = NULL;

	string			test;

	size_t i = 2;
	struct dirent *dirent_ptr;

	while ((dirent_ptr = readdir(dir)) != NULL)
        brut_files.push_back(dirent_ptr->d_name);
	while (i < brut_files.size())
	{
		if (brut_files[i].find(extension.c_str(), 0, extension.size()) != string::npos)
		{
			test = strsplit(brut_files[i], extension)[0];
			files.push_back(test);
		}
		i++;
	}
	return files;
}

bool				check_file_exist(string path)
{
	ifstream file(path.c_str());
    return file.good();
}

bool				copy_file(string src, string dest)
{
	ifstream source;
	ofstream destination;

	source.open(src, ios::binary);
	destination.open(dest, ios::binary);

	source.seekg(0, ios::end);
	ifstream::pos_type size = source.tellg();
	source.seekg(0);
	char* buffer = new char[size];

	source.read(buffer, size);
	destination.write(buffer, size);

	return (true);
}

void				write_on_file(string path, string text)
{
	ofstream myfile;
	myfile.open(path.c_str(), ios::app);
	myfile << text << endl;
	myfile.close();
}

void				rewrite_on_file(string path, string text)
{
	ofstream myfile;
	myfile.open(path.c_str());
	myfile << text << endl;
	myfile.close();
}
