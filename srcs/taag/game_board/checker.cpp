#include "taag.h"

bool				s_game_board::check_anim()
{
	int i = 0;

	while ((size_t)i < board_size.x)
	{
		int j = 0;
		while ((size_t)j < board_size.y)
		{

			if (get_cell(i, j))
			{
				size_t k = 0;
				while (k < get_cell(i, j)->animation.size())
				{
					if (get_cell(i, j)->animation[k].index < get_cell(i, j)->animation[k].len)
						return (false);
					k++;
				}
			}
			j++;
		}
		i++;
	}
	return (true);
}

bool				s_game_board::check_visual()
{
	for (size_t i = 0; i < actor_list.size(); i++)
		if (actor_list[i]->visual_info.size() != 0)
			return (false);
	return (true);
}

void				s_game_board::check_alive()
{
	size_t i = 0;
	while (i < actor_list.size())
	{
		if (actor_list[i]->stat.hp.value <= 0 && check_visual() == true && check_anim() == true)
			delete_actor(actor_list[i]);
		else
			i++;
	}
}

void				s_game_board::ending_fight(bool *play)
{
	if (account->tuto_state < TUTO_SIZE)
		account->tuto_state++;
	if (check_anim() == true && check_visual() == true)
	{
		if (enemy_list.size() == 0)
			end_game_win();
		else if (ally_list.size() == 0)
			end_game_lose();
	}
		*play = false;
}
