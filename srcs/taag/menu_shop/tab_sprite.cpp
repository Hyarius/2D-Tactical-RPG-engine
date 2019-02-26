#include "taag.h"

void create_tab_sprite(t_data data)
{
	t_gui *gui = (t_gui *)(data.data[0]);

	*gui = t_gui();
	gui->add(new t_button(new s_text_button(
			"SPRITE TAB", DARK_GREY,
			t_vect(4, 4) * gui->unit, t_vect(3, 1) * gui->unit, 8,
			t_color(0.3, 0.3, 0.3), t_color(0.5, 0.5, 0.5)),
			NULL, NULL));
}
