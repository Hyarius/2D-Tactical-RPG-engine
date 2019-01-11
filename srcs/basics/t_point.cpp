#include "template.h"

s_point::s_point()
{
	this->x = 0;
	this->y = 0;
	this->r = 0.0;
	this->g = 0.0;
	this->b = 0.0;
	this->a = 0.0;
}

s_point::s_point(double p_x, double p_y, double p_a, double p_b, double p_c, double p_d)
{
	this->x = p_x;
	this->y = p_y;
	this->r = p_a;
	this->g = p_b;
	this->b = p_c;
	this->a = p_d;
}

s_point::s_point(double p_x, double p_y, double p_a, double p_b, double p_c)
{
	this->x = p_x;
	this->y = p_y;
	this->r = p_a;
	this->g = p_b;
	this->b = p_c;
	this->a = 1.0;
}

s_point::s_point(double p_x, double p_y, double p_a, double p_b)
{
	this->x = p_x;
	this->y = p_y;
	this->r = p_a;
	this->g = p_b;
	this->b = 0.0;
	this->a = 1.0;
}

s_point::s_point(double p_x, double p_y)
{
	this->x = p_x;
	this->y = p_y;
	this->r = 0.0;
	this->g = 0.0;
	this->b = 0.0;
	this->a = 0.0;
}
