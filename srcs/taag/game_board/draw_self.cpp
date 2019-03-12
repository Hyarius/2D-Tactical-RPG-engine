#include "taag.h"

void				s_game_board::draw_self()
{
	static bool a = true;
	static int last_s_spell = -1;

	if (s_spell != last_s_spell)
	{
		last_s_spell = s_spell;
		a = true;
		write_in_log("================\n");
		write_in_log("== NEW ACTION ==\n");
		write_in_log("Action type : " + (s_spell == -1 ? string("Mouvement") : string("Spell")) + "\n");
		write_in_log("Statement at start : ");
		printOpenGLError();
		printSDLError();
		write_in_log("No error\n");
		write_in_log("================\n");
	}

	if (a == true)
		write_in_log("Starting draw_board function : ");
	draw_board();
	if (a == true)
	{
		printOpenGLError();
		printSDLError();
		write_in_log("No error\n");
		write_in_log("Starting draw_cell_layer function : ");
	}
	draw_cell_layer();
	if (a == true)
	{
		printOpenGLError();
		printSDLError();
		write_in_log("No error\n");
		write_in_log("Starting draw_placement function : ");
	}
	draw_placement();
	if (turn_order.size())
	{
		if (a == true)
		{
			printOpenGLError();
			printSDLError();
			write_in_log("No error\n");
			write_in_log("Starting draw_path function : ");
		}
		draw_path();
	}
	if (a == true)
	{
		printOpenGLError();
		printSDLError();
		write_in_log("No error\n");
		write_in_log("Starting draw_actor_list function : ");
	}
	draw_actor_list();
	if (a == true)
	{
		printOpenGLError();
		printSDLError();
		write_in_log("No error\n");
		write_in_log("Starting draw_cursor_layer function : ");
	}
	draw_cursor_layer();
	if (a == true)
	{
		printOpenGLError();
		printSDLError();
		write_in_log("No error\n");
		write_in_log("Starting draw_mouse_cursor function : ");
	}
	draw_mouse_cursor();
	if (a == true)
	{
		printOpenGLError();
		printSDLError();
		write_in_log("No error\n");
		write_in_log("Starting draw_animation function : ");
	}
	draw_animation();
	if (a == true)
	{
		printOpenGLError();
		printSDLError();
		write_in_log("No error\n");
		write_in_log("Starting draw_visual_info function : ");
	}
	draw_visual_info();
	if (a == true)
	{
		printOpenGLError();
		printSDLError();
		write_in_log("No error\n");
		a = false;
	}
}
