#include "taag.h"

ofstream log_file;
int line;

string				get_file_info(string file, int line)
{
	vector<string>tab = strsplit(file, "\\");
	if (tab.size() == 1)
		vector<string>tab = strsplit(file, "/");
	string file_name = tab[tab.size() - 1];
	return("[" + file_name + "][" + to_string(line) + "]");
}

void charge_log_file()
{
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::ostringstream oss;
	oss << std::put_time(&tm, "-%d-%m-%Y");
	string str = oss.str();
	string path = LOG_PATH + "TAAG" + str + ".log";
	if (check_file_exist(path) == true)
	{
		for (int i = 1; check_file_exist(path) == true; i++)
			path = LOG_PATH + "TAAG" + str + " (" + to_string(i) + ")" + ".log";
	}
	log_file.open(path.c_str());
	line = 0;

	std::ostringstream oss2;
	oss2 << std::put_time(&tm, "%d-%m-%Y %Hh-%Mm-%Ss");
	str = oss2.str();
	string text = "===	TAAG - " + str + " -		===\n";
	log_file << text;
	text = "==========				===========\n";
	log_file << text;
}

void write_in_log(string msg)
{
	static string text;
	text += (text.size() == 0 ? "[" + to_string(line) + "] - " + msg : msg);
	if (msg.find('\n') != string::npos)
	{
		log_file << text;
		text.clear();
		line++;
	}
}