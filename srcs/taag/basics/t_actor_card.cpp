#include "taag.h"

s_actor_card::s_actor_card()
{
	button = NULL;
	actor = NULL;
	button_name = NULL;
	actor_frame = NULL;
}

s_actor_card::s_actor_card(t_button *p_button, struct s_actor **p_actor, t_button *p_button_name, t_button *p_actor_frame)
{
	button = p_button;
	actor = p_actor;
	button_name = p_button_name;
	actor_frame = p_actor_frame;
}

void			s_actor_card::draw_self()
{
	if (button != NULL)
		button->draw_self();
	if (button_name != NULL)
	{
		if (actor != NULL && *actor != NULL)
			button_name->button->text = (*actor)->name;
		else
			button_name->button->text = " ";
		button_name->draw_self();
	}
	if (actor_frame != NULL)
		actor_frame->draw_self();
	if (actor != NULL && *actor != NULL && (*actor)->tile != NULL)
	{
		int type = get_frame_state(4);
		if (type == 3)
			type = 1;
		(*actor)->tile->draw_self(actor_frame->button->coord[1], actor_frame->button->size[1], (*actor)->sprite + t_vect(type, 0));
	}
}

bool			s_actor_card::click_left(t_vect mouse)
{
	if (button != NULL && button->click_left(mouse) == true)
		return (true);
	return (false);
}

bool			s_actor_card::click_right(t_vect mouse)
{
	if (button != NULL && button->click_right(mouse) == true)
		return (true);
	return (false);
}

bool			s_actor_card::key_press(SDL_Event *event)
{
	if (button != NULL && button->key_press(event) == true)
		return (true);
	return (false);
}
