#include "taag.h"

event			g_effects[NB_EFFECTS];

void			init_effects()
{
	g_effects[0] = deal_mag_dmg;
	g_effects[1] = deal_phy_dmg;
	g_effects[2] = heal;
	g_effects[3] = change_pm;
	g_effects[4] = change_pa;
}

void deal_mag_dmg(t_actor *source, t_actor *target, t_effect_stat effect_stat)
{
	string source_name = (source != NULL ? source->name : "empty");
	string target_name = (target != NULL ? target->name : "empty");
	printf("%s deal %.2f magical damage to %s\n", source_name.c_str(), effect_stat.value[0], target_name.c_str());

	int attack = (source != NULL ? 1 : 0);
	int defense = (target != NULL ? 1 : 0);

}

void deal_phy_dmg(t_actor *source, t_actor *target, t_effect_stat effect_stat)
{
	string source_name = (source != NULL ? source->name : "empty");
	string target_name = (target != NULL ? target->name : "empty");
	printf("%s deal %.2f magical damage to %s\n", source_name.c_str(), effect_stat.value[0], target_name.c_str());
}

void heal(t_actor *source, t_actor *target, t_effect_stat effect_stat)
{
	string source_name = (source != NULL ? source->name : "empty");
	string target_name = (target != NULL ? target->name : "empty");
	printf("%s heal %.2f hp to %s\n", source_name.c_str(), effect_stat.value[0], target_name.c_str());
}

void change_pm(t_actor *source, t_actor *target, t_effect_stat effect_stat)
{
	string source_name = (source != NULL ? source->name : "empty");
	string target_name = (target != NULL ? target->name : "empty");
	printf("%s give %.2f pm to %s\n", source_name.c_str(), effect_stat.value[0], target_name.c_str());
}

void change_pa(t_actor *source, t_actor *target, t_effect_stat effect_stat)
{
	string source_name = (source != NULL ? source->name : "empty");
	string target_name = (target != NULL ? target->name : "empty");
	printf("%s give %.2f pa to %s\n", source_name.c_str(), effect_stat.value[0], target_name.c_str());
}
