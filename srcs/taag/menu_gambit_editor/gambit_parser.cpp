#include "taag.h"
/*
#define CHARGE			0	//run to the closest enemy at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define CHARGE_WEAK		1	//run to the enemy with less hp at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define CHARGE_PERCENT	2	//run to the enemy with less % hp at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define RETREAT			3	//retreat to the safest tile if stat hp < value[1] % with max mouvement value[2]
#define SUPPORT			4	//run to closest ally at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define SUPPORT_PERCENT	5	//run to closest ally with less % hp at range value[1], delta value[2] and range type value[3] with max mouvement value[4]
#define ATTACK			6	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on the closest enemy availible in range if possible
#define ATTACK_WEAK		7	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on the enemy with the less hp in range if possible
#define ATTACK_PERCENT	8	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on the enemy with the less % hp in range if possible
#define HELP			9	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on an ally
#define HELP_WEAK		10	//cast the spell num value[1] (if value[2] PA and value[3] pm on caster) on the ally with less % HP
#define HELP_PERC		11	//cast the spell num value[2] (if value[2] PA and value[3] pm on caster) on the ally if HP % < value[1]
#define TURN			12	//if turn == value[1] --> execute command num value[2] with value stocked after
*/

static string parse_charge(t_ai_helper data)
{
	string text = "approach the closest enemy at " + (data.value[2] != 0 ? to_string(data.value[1] - data.value[2]) + "-": "") + to_string(data.value[1]) + " tile of distance";
	if (data.value[4] != -1)
		text += " with max " + to_string(data.value[4]) + " PM used";
	return (text);
}

static string parse_charge_weak(t_ai_helper data)
{
	string text = "approach the enemy with less hp at " + (data.value[2] != 0 ? to_string(data.value[1] - data.value[2]) + "-" : "") + to_string(data.value[1]) + " tile of distance";
	if (data.value[4] != -1)
		text += " with max " + to_string(data.value[4]) + " PM used";
	return (text);
}

static string parse_charge_percent(t_ai_helper data)
{
	string text = "approach the enemy with less % hp at " + (data.value[2] != 0 ? to_string(data.value[1] - data.value[2]) + "-" : "") + to_string(data.value[1]) + " tile of distance";
	if (data.value[4] != -1)
		text += " with max " + to_string(data.value[4]) + " PM used";
	return (text);
}

static string parse_retreat(t_ai_helper data)
{
	string text = "flee the enemy if hp < " + to_string(data.value[1]) + " %";
	if (data.value[2] != -1)
		text += " with max " + to_string(data.value[2]) + " PM used";
	return (text);
}

static string parse_support(t_ai_helper data)
{
	string text = "approach the closest ally at " + (data.value[2] != 0 ? to_string(data.value[1] - data.value[2]) + "-" : "") + to_string(data.value[1]) + " tile of distance";
	if (data.value[4] != -1)
		text += " with max " + to_string(data.value[4]) + " PM used";
	return (text);
}

static string parse_support_percent(t_ai_helper data)
{
	string text = "approach the ally with less % hp at " + (data.value[2] != 0 ? to_string(data.value[1] - data.value[2]) + "-" : "") + to_string(data.value[1]) + " tile of distance";
	if (data.value[4] != -1)
		text += " with max " + to_string(data.value[4]) + " PM used";
	return (text);
}

static string parse_support_damaged(t_ai_helper data)
{
	string text = "approach the ally with < " + to_string(data.value[1]) + " % hp at " + (data.value[3] != 0 ? to_string(data.value[2] - data.value[3]) + "-" : "") + to_string(data.value[2]) + " tile of distance";
	if (data.value[5] != -1)
		text += " with max " + to_string(data.value[5]) + " PM used";
	return (text);
}

static string parse_attack(t_ai_helper data)
{
	string text = "cast the spell num " + to_string(data.value[1]) + " on the closest enemy on range";
	if (data.value[2] != -1)
		text += " if PA == " + to_string(data.value[2]);
	if (data.value[3] != -1)
		text += " if PM == " + to_string(data.value[2]);
	return (text);
}

static string parse_attack_weak(t_ai_helper data)
{
	string text = "cast the spell num " + to_string(data.value[1]) + " on the enemy with lower hp value on range";
	if (data.value[2] != -1)
		text += " if PA == " + to_string(data.value[2]);
	if (data.value[3] != -1)
		text += " if PM == " + to_string(data.value[2]);
	return (text);
}

static string parse_attack_percent(t_ai_helper data)
{
	string text = "cast the spell num " + to_string(data.value[1]) + " on the enemy with lower hp % on range";
	if (data.value[2] != -1)
		text += " if PA == " + to_string(data.value[2]);
	if (data.value[3] != -1)
		text += " if PM == " + to_string(data.value[2]);
	return (text);
}


static string parse_help(t_ai_helper data)
{
	string text = "cast the spell num " + to_string(data.value[1]) + " on the closest ally on range";
	if (data.value[2] != -1)
		text += " if PA == " + to_string(data.value[2]);
	if (data.value[3] != -1)
		text += " if PM == " + to_string(data.value[2]);
	return (text);
}

static string parse_help_weak(t_ai_helper data)
{
	string text = "cast the spell num " + to_string(data.value[1]) + " on the ally with the lowest HP value on range";
	if (data.value[2] != -1)
		text += " if PA == " + to_string(data.value[2]);
	if (data.value[3] != -1)
		text += " if PM == " + to_string(data.value[2]);
	return (text);
}

static string parse_help_percent(t_ai_helper data)
{
	string text = "cast the spell num " + to_string(data.value[1]) + " on the closest ally on range if him hp < " + to_string(data.value[2]) + " %";
	if (data.value[3] != -1)
		text += " if PA == " + to_string(data.value[3]);
	if (data.value[4] != -1)
		text += " if PM == " + to_string(data.value[4]);
	return (text);
}


static string parse_turn(t_ai_helper data)
{
	if (data.value.size() < 3)
		return ("ERROR");
	return ("NULL");
}

static string parse_attack_caster_hp(t_ai_helper data)
{
	string text = "cast the spell num " + to_string(data.value[1]) + " on the closest enemy on range if caster hp < " + to_string(data.value[2]) + " %";
	if (data.value[3] != -1)
		text += " if PA == " + to_string(data.value[3]);
	if (data.value[4] != -1)
		text += " if PM == " + to_string(data.value[4]);
	return (text);
}

static string parse_heal_caster_hp(t_ai_helper data)
{
	string text = "cast the spell num " + to_string(data.value[1]) + " on the closest ally on range if caster hp < " + to_string(data.value[2]) + " %";
	if (data.value[3] != -1)
		text += " if PA == " + to_string(data.value[3]);
	if (data.value[4] != -1)
		text += " if PM == " + to_string(data.value[4]);
	return (text);
}


typedef string (*f_gambit_parsor)(t_ai_helper);

f_gambit_parsor f_gambit_list[] = {
	&parse_charge,
	&parse_charge_weak,
	&parse_charge_percent,
	&parse_retreat,
	&parse_support,
	&parse_support_percent,
	&parse_support_damaged,
	&parse_attack,
	&parse_attack_weak,
	&parse_attack_percent,
	&parse_attack_caster_hp,
	&parse_help,
	&parse_help_weak,
	&parse_help_percent,
	&parse_heal_caster_hp,
	&parse_turn,
};

string parse_gambit(t_ai_helper data)
{
	if (data.value.size() == 0 || data.value[0] == -1)
		return ("");
	return (f_gambit_list[data.value[0]](data));
}
