#include "taag.h"

bool		bool_checker[] = {true, true , true , true , true , true , true , true , true };
bool		draw_self_bool = true;

void				s_game_board::draw_self()
{
	static int last_s_spell = -1;

	if (s_spell != last_s_spell || draw_self_bool == true)
	{
		last_s_spell = s_spell;
		draw_self_bool = true;
		write_in_log("\n");
		write_in_log("\n");
		write_in_log("================\n");
		write_in_log("== NEW ACTION ==\n");
		write_in_log("Action type : " + (s_spell == -1 ? string("Mouvement") : string("Spell")) + "\n");
		write_in_log("Statement at start : ");
		printOpenGLError();
		printSDLError();
		write_in_log("No error\n");
		write_in_log("================\n");
	}

	if (draw_self_bool == true)
		write_in_log("Starting draw_board function : ");
	if (bool_checker[0] == true)
		draw_board();
	if (draw_self_bool == true)
	{
		printOpenGLError();
		printSDLError();
		if (bool_checker[0] == true)
			write_in_log("No error\n");
		else
			write_in_log("ABORT\n");
		write_in_log("Starting draw_cell_layer function : ");
	}
	if (bool_checker[1] == true)
		draw_cell_layer();
	if (draw_self_bool == true)
	{
		printOpenGLError();
		printSDLError();
		if (bool_checker[1] == true)
			write_in_log("No error\n");
		else
			write_in_log("ABORT\n");
		write_in_log("Starting draw_placement function : ");
	}
	if (bool_checker[2] == true)
		draw_placement();
	if (draw_self_bool == true)
	{
		printOpenGLError();
		printSDLError();
		if (bool_checker[2] == true)
			write_in_log("No error\n");
		else
			write_in_log("ABORT\n");
	}
	if (turn_order.size())
	{
		if (draw_self_bool == true)
		{
			write_in_log("Starting draw_path function : ");
		}
		draw_path();
		if (draw_self_bool == true)
		{
			printOpenGLError();
			printSDLError();
			if (bool_checker[3] == true)
				write_in_log("No error\n");
			else
				write_in_log("ABORT\n");
		}
	}
	if (draw_self_bool == true)
		write_in_log("Starting draw_actor_list function : ");
	if (bool_checker[4] == true)
		draw_actor_list();
	if (draw_self_bool == true)
	{
		printOpenGLError();
		printSDLError();
		if (bool_checker[4] == true)
			write_in_log("No error\n");
		else
			write_in_log("ABORT\n");
		write_in_log("Starting draw_cursor_layer function : ");
	}
	if (bool_checker[5] == true)
		draw_cursor_layer();
	if (draw_self_bool == true)
	{
		printOpenGLError();
		printSDLError();
		if (bool_checker[5] == true)
			write_in_log("No error\n");
		else
			write_in_log("ABORT\n");
		write_in_log("Starting draw_mouse_cursor function : ");
	}
	if (bool_checker[6] == true)
		draw_mouse_cursor();
	if (draw_self_bool == true)
	{
		printOpenGLError();
		printSDLError();
		if (bool_checker[6] == true)
			write_in_log("No error\n");
		else
			write_in_log("ABORT\n");
		write_in_log("Starting draw_animation function : ");
	}
	if (bool_checker[7] == true)
		draw_animation();
	if (draw_self_bool == true)
	{
		printOpenGLError();
		printSDLError();
		if (bool_checker[7] == true)
			write_in_log("No error\n");
		else
			write_in_log("ABORT\n");
		write_in_log("Starting draw_visual_info function : ");
	}
	if (bool_checker[8] == true)
		draw_visual_info();
	if (draw_self_bool == true)
	{
		printOpenGLError();
		printSDLError();
		if (bool_checker[8] == true)
			write_in_log("No error\n");
		else
			write_in_log("ABORT\n");
		draw_self_bool = false;
	}
}
