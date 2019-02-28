#include "template.h"

s_color::s_color()
{
	this->r = 0.0;
	this->g = 0.0;
	this->b = 0.0;
	this->a = 1.0;
}

s_color::s_color(double p_r, double p_g, double p_b)
{
	this->r = p_r;
	this->g = p_g;
	this->b = p_b;
	this->a = 1.0;
}

s_color::s_color(double p_r, double p_g, double p_b, double p_a)
{
	this->r = p_r;
	this->g = p_g;
	this->b = p_b;
	this->a = p_a;
}

s_color::s_color(int p_r, int p_g, int p_b)
{
	this->r = (double)(p_r) / 255.0;
	this->g = (double)(p_g) / 255.0;
	this->b = (double)(p_b) / 255.0;
	this->a = 1.0;
}

s_color::s_color(int p_r, int p_g, int p_b, int p_a)
{
	this->r = (double)(p_r) / 255.0;
	this->g = (double)(p_g) / 255.0;
	this->b = (double)(p_b) / 255.0;
	this->a = (double)(p_a) / 255.0;
}

s_color			s_color:: operator + (double add)
{
	s_color result = s_color(this->r + add, this->g + add, this->b + add);
	return (result);
}

s_color			s_color:: operator - (double add)
{
	s_color result = s_color(this->r - add, this->g - add, this->b - add);
	return (result);
}
