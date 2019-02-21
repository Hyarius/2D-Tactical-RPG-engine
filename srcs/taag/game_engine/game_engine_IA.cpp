#include "taag.h"
/*
#define CHARGE			0	//run to the closest enemy at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define CHARGE_WEAK		1	//run to the enemy with less hp at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define CHARGE_PERCENT	2	//run to the enemy with less % hp at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define RETREAT			3	//retreat to the safest tile if stat hp < value[1] %
#define SUPPORT			4	//run to closest ally at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define SUPPORT_PERCENT	5	//run to closest ally with less % hp at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define ATTACK			6	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on the closest enemy availible in range if possible
#define ATTACK_WEAK		7	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on the enemy with the less hp in range if possible
#define ATTACK_PERCENT	8	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on the enemy with the less % hp in range if possible
#define HELP			9	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on an ally
#define HELP_WEAK		10	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on the ally with less % HP
#define HELP_PERC		11	//cast the spell num value[2] (if value[2] PA and value[3] pm on caster) on the ally if HP % < value[1]
#define TURN			12	//if turn == value[1] --> execute command num value[2] with verification helped by value[3]
*/

gambit_command command[13] = {
	&s_game_engine::get_close_enemy,
	&s_game_engine::get_close_enemy_weak,
	&s_game_engine::get_close_enemy_percent,
	&s_game_engine::flee_enemy,
	&s_game_engine::get_close_ally,
	&s_game_engine::get_close_ally_percent,
	&s_game_engine::attack,
	&s_game_engine::attack_weak,
	&s_game_engine::attack_percent,
	&s_game_engine::help,
	&s_game_engine::help_weak,
	&s_game_engine::help_percent,
	&s_game_engine::action_on_turn,
};

bool					s_game_engine::execute_gambit(t_actor *source)
{
	if (board.check_anim() == false || source->destination.size() != 0 ||
		board.enemy_list.size() == 0 || board.ally_list.size() == 0)
		return (false);
	size_t i = 0;
	while (i < source->gambit.size())
	{
		int index = source->gambit[i].value[0];
		printf("[%zu] - %d", i, index);
		t_ai_helper data = source->gambit[i];
		if (index >= 0)
		{
			if ((this->*(command[index]))(data) == true)
				return (false);
		}
		printf("\n");
		i++;
	}
	return (true);
}

void				s_game_engine::enemy_turn()
{
	board.reset_board();

	bool play = true;
	SDL_Event	event;

	printf("--- actor = %s ---\n", turn_order[turn_index % turn_order.size()]->name.c_str());
	while (execute_gambit(turn_order[turn_index % turn_order.size()]) == false && play == true)
	{
		prepare_screen();

		update_board();
		draw_board();
		draw_gui();

		if (board.enemy_list.size() == 0 || board.ally_list.size() == 0)
			ending_fight(&play);

		if (SDL_PollEvent(&event) == 1)
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
				exit(0);
			handle_control_camera(&event);
		}
		render_screen(true);
	}
	if (play == true)
		next_turn();
}

void				s_game_engine::next_turn()
{
	if (turn_order.size())
	{
		turn_order[turn_index % turn_order.size()]->selected = false;
		turn_order[turn_index % turn_order.size()]->reset_value();
	}
	turn_index = (turn_index + 1) % turn_order.size();
	if (turn_order.size())
	{
		turn_order[turn_index % turn_order.size()]->selected = true;
	}
	calculated = false;
	s_spell = -1;
	if (turn_order[turn_index % turn_order.size()]->team != 1)
	{
		enemy_turn();
	}
}
