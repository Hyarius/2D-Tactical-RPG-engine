#include "taag.h"

int				s_game_engine::calc_pool()
{
	return (((level - 1) * 2) + 15);
}

int 			s_game_engine::calc_max_exp(int level)
{
	return (level * 10 + 20);
}

void			s_game_engine::add_exp(int delta)
{
	exp.value += delta;
	while (exp.value >= exp.max)
	{
		exp.value -= exp.max;
		level++;
		exp.max = calc_max_exp(level);
	}
}

bool			s_game_engine::add_gold(int delta)
{
	if (gold + delta < 0)
		return (false);
	gold += delta;
	return (true);
}

void			save_game_engine()
{
	string text = "level:" + to_string(account->level) + "\n";
	text.append("exp:" + to_string(account->exp.value) + "\n");
	text.append("gold:" + to_string(account->gold) + "\n");
	text.append("tuto_state:" + to_string(account->tuto_state) + "\n");
	text.append("keyboard:" + to_string(account->keyboard[0]) + ":" + to_string(account->keyboard[1]) + ":" + to_string(account->keyboard[2]) + ":" + to_string(account->keyboard[3]) + ":" + to_string(account->keyboard[4]) + ":" + to_string(account->keyboard[5]) + "\n");

	text.append("char");
	for (size_t i = 0; i < 6; i++)
		text.append(":" + account->actor[i]);
	text.append("\n");
	text.append("spell:");
	for (size_t i = 0; i < account->spell_unlock.size(); i++)
		text.append(":" + account->spell_unlock[i]);
	text.append("\n");
	text.append("tile");
	for (size_t i = 0; i < account->tile_unlock.size(); i++)
		text.append(":" + account->tile_unlock[i]);
	text.append("\n");
	text.append("map");
	for (size_t i = 0; i < account->map_unlock.size(); i++)
		text.append(":" + account->map_unlock[i]);
	rewrite_on_file((GAME_ENGINE_PATH + "saved_game" + GAME_ENGINE_EXT).c_str(), text);
}

s_game_engine::s_game_engine()
{
	ifstream		myfile;
	myfile.open(GAME_ENGINE_PATH + "saved_game" + GAME_ENGINE_EXT);
	if (myfile.fail())
		printf("can't open such file : %s\n", (GAME_ENGINE_PATH + "saved_game" + GAME_ENGINE_EXT).c_str());
	level = atoi(get_strsplit(&myfile, ":", 2)[1].c_str());
	exp = s_value(atoi(get_strsplit(&myfile, ":", 2)[1].c_str()), calc_max_exp(level));
	gold = atoi(get_strsplit(&myfile, ":", 2)[1].c_str());
	tuto_state = atoi(get_strsplit(&myfile, ":", 2)[1].c_str());

	vector<string> tab = get_strsplit(&myfile, ":", 7);
	for (size_t i = 1; i < tab.size(); i++)
		keyboard[i - 1] = atoi(tab[i].c_str());

	tab = get_strsplit(&myfile, ":", 7);

	for (size_t i = 1; i < tab.size(); i++)
	{
		if (check_file_exist(ACTOR_PATH + tab[i] + ACTOR_EXT))
			actor[i - 1] = tab[i];
		else
			actor[i - 1] = "NULL";
	}

	for (size_t i = 0; i < 6; i++)
		actor_array[i] = NULL;
	recharge_actor();

	tab = get_strsplit(&myfile, ":", -1);

	for (size_t i = 1; i < tab.size(); i++)
		spell_unlock.push_back(tab[i]);

	tab = get_strsplit(&myfile, ":", -1);

	for (size_t i = 1; i < tab.size(); i++)
		tile_unlock.push_back(tab[i]);

	tab = get_strsplit(&myfile, ":", -1);

	for (size_t i = 1; i < tab.size(); i++)
		map_unlock.push_back(tab[i]);

	board = t_game_board();
	set_game_engine(&(this->board));
}

void		s_game_engine::start_game(string path)
{
	board = t_game_board(path);
	set_game_engine(&(this->board));
}

void		s_game_engine::recharge_actor()
{
	for (size_t i = 0; i < 6; i++)
	{
		if (actor[i] != "NULL" && check_file_exist(ACTOR_PATH + actor[i] + ACTOR_EXT) == true)
		{
			if (actor_array[i] == NULL)
				actor_array[i] = new t_actor(read_actor(ACTOR_PATH + actor[i] + ACTOR_EXT));
			else
				*(actor_array[i]) = read_actor(ACTOR_PATH + actor[i] + ACTOR_EXT);
			actor_array[i]->team = 1;
		}
		else
			actor_array[i] = NULL;
	}
}

void		s_game_engine::recharge_map()
{
	for (size_t i = 0; i < map_unlock.size(); i++)
	{
		if (check_file_exist(MAP_PATH + map_unlock[i] + MAP_EXT) == false)
		{
			map_unlock.erase(map_unlock.begin() + i);
			i--;
		}
	}
}
