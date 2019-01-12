#include "gui.h"

s_data::s_data()
{

}

s_data::s_data(void *p_data)
{
	this->data.push_back(p_data);
}

s_data::s_data(int nb_param, void *p_ptr, ...)
{
	va_list list_param;
    va_start(list_param, p_ptr);
	int i = 1;

	this->data.push_back(p_ptr);
	while (i < nb_param)
	{
		this->data.push_back(va_arg(list_param, void *));
		i++;
	}
	va_end(list_param);
}

void		s_data::operator = (void *p_data)
{
	this->data.clear();
	this->data.push_back(p_data);
}

void		s_data::operator + (void *p_ptr)
{
	this->data.push_back(p_ptr);
}
