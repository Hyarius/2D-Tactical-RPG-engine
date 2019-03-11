#include "taag.h"

gambit_command command[] = {
	&s_game_board::get_close_enemy, // 0
	&s_game_board::get_close_enemy_weak, //1
	&s_game_board::get_close_enemy_percent, // 2
	&s_game_board::flee_enemy, //3
	&s_game_board::get_close_ally, //4
	&s_game_board::get_close_ally_percent, // 5
	&s_game_board::get_close_ally_damaged, // 6
	&s_game_board::attack, // 7
	&s_game_board::attack_weak, // 8
	&s_game_board::attack_percent, // 9
	&s_game_board::attack_caster_hp, //10
	&s_game_board::help, // 11
	&s_game_board::help_weak, // 12
	&s_game_board::help_percent, // 13
	&s_game_board::help_caster_hp, //14
	&s_game_board::action_on_turn, // 15
};

bool				s_game_board::execute_gambit(t_actor *source)
{
	if (check_anim() == false || check_visual() == false ||
		source->destination.size() != 0 ||
		enemy_list.size() == 0 || ally_list.size() == 0)
		return (false);
	size_t i = 0;
	while (i < source->gambit.size())
	{
		check_alive();
		int index = source->gambit[i].value[0];
		t_ai_helper data = source->gambit[i];
		if (index >= 0)
		{
			if ((this->*(command[index]))(data) == true)
				return (false);
		}
		i++;
		if (source->stat.hp.value <= 0)
			return (true);
	}
	return (true);
}
