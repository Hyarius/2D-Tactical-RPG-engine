# include "template.h"

				matrice::matrice(double a0, double a1, double a2, double a3, double b0, double b1, double b2, double b3, double c0, double c1, double c2, double c3,
				double d0, double d1, double d2, double d3)
{
	this->value[0][0] = a0;			this->value[0][1] = a1;				this->value[0][2] = a2;				this->value[0][3] = a3;
	this->value[1][0] = b0;			this->value[1][1] = b1;				this->value[1][2] = b2;				this->value[1][3] = b3;
	this->value[2][0] = c0;			this->value[2][1] = c1;				this->value[2][2] = c2;				this->value[2][3] = c3;
	this->value[2][0] = d0;			this->value[2][1] = d1;				this->value[2][2] = d2;				this->value[3][3] = d3;
}

				matrice::matrice(X_ROTATE, double angle)
{
	double		radian = degree_to_radius(angle);
	this->value[0][0] = 1.0f; 		this->value[0][1] = 0.0f;       	this->value[0][2] = 0.0f;			this->value[0][3] = 0;
	this->value[1][0] = 0.0f; 		this->value[1][1] = cos(radian); 	this->value[1][2] = -sin(radian);	this->value[1][3] = 0;
	this->value[2][0] = 0.0f; 		this->value[2][1] = sin(radian); 	this->value[2][2] = cos(radian);	this->value[2][3] = 0;
	this->value[3][0] = 0;			this->value[3][1] = 0;				this->value[3][2] = 0;				this->value[3][3] = 1;
}

				matrice::matrice(Y_ROTATE, double angle)
{
	double		radian = degree_to_radius(angle);

	this->value[0][0] = cos(radian); this->value[0][1] = 0.0f; 			this->value[0][2] = sin(radian);	this->value[0][3] = 0;
	this->value[1][0] = 0.0f;       this->value[1][1] = 1.0f; 			this->value[1][2] = 0.0f;			this->value[1][3] = 0;
	this->value[2][0] = -sin(radian);this->value[2][1] = 0.0f; 			this->value[2][2] = cos(radian);	this->value[2][3] = 0;
	this->value[3][0] = 0;			this->value[3][1] = 0;				this->value[3][2] = 0;				this->value[3][3] = 1;
}

				matrice::matrice(Z_ROTATE, double angle)
{
	double		radian = degree_to_radius(angle);

	this->value[0][0] = cos(radian);this->value[0][1] = -sin(radian);	this->value[0][2] = 0.0f;			this->value[0][3] = 0;
	this->value[1][0] = sin(radian);this->value[1][1] = cos(radian);	this->value[1][2] = 0.0f;			this->value[1][3] = 0;
	this->value[2][0] = 0.0f; 		this->value[2][1] = 0.0f; 			this->value[2][2] = 1.0f;			this->value[2][3] = 0;
	this->value[3][0] = 0;			this->value[3][1] = 0;				this->value[3][2] = 0;				this->value[3][3] = 1;
}

				matrice::matrice(ROTATION, double x, double y, double z)
{
	matrice rotx = matrice(X, x);
	matrice roty = matrice(Y, y);
	matrice rotz = matrice(Z, z);

	matrice result = (rotx * roty * rotz);

	int i;
	int j;

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			this->value[i][j] = result.value[i][j];
			j++;
		}
		i++;
	}
}

				matrice::matrice(TRANSLATION, double t_x, double t_y, double t_z)
{
	this->value[0][0] = 1.0f;		this->value[0][1] = 0.0f; 			this->value[0][2] = 0.0f;			this->value[0][3] = t_x;
	this->value[1][0] = 0.0f;		this->value[1][1] = 1.0f; 			this->value[1][2] = 0.0f;			this->value[1][3] = t_y;
	this->value[2][0] = 0.0f;		this->value[2][1] = 0.0f; 			this->value[2][2] = 1.0f;			this->value[2][3] = t_z;
	this->value[3][0] = 0;			this->value[3][1] = 0;				this->value[3][2] = 0;				this->value[3][3] = 1;
}

				matrice::matrice(SCALE, double t_x, double t_y, double t_z)
{
	this->value[0][0] = t_x;		this->value[0][1] = 0.0f; 			this->value[0][2] = 0.0f;			this->value[0][3] = 0.0f;
	this->value[1][0] = 0.0f;		this->value[1][1] = t_y; 			this->value[1][2] = 0.0f;			this->value[1][3] = 0.0f;
	this->value[2][0] = 0.0f;		this->value[2][1] = 0.0f; 			this->value[2][2] = t_z;			this->value[2][3] = 0.0f;
	this->value[3][0] = 0;			this->value[3][1] = 0;				this->value[3][2] = 0;				this->value[3][3] = 1;
}

matrice			matrice::operator * (matrice p_matrice)
{
	int		i;
	int		j;
	matrice	m = matrice(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	i = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			m.value[i][j] = (this->value[i][0] * p_matrice.value[0][j]) +
						(this->value[i][1] * p_matrice.value[1][j]) +
						(this->value[i][2] * p_matrice.value[2][j]) +
						(this->value[i][3] * p_matrice.value[3][j]);
			j++;
		}
		i++;
	}
	return (m);
}

t_vertex		matrice::operator * (t_vertex vertex)
{
	double		result[3];

	result[0] = this->value[0][0] * vertex.x + this->value[0][1] * vertex.y + this->value[0][2] * vertex.z + this->value[0][3];
	result[1] = this->value[1][0] * vertex.x + this->value[1][1] * vertex.y + this->value[1][2] * vertex.z + this->value[1][3];
	result[2] = this->value[2][0] * vertex.x + this->value[2][1] * vertex.y + this->value[2][2] * vertex.z + this->value[2][3];

	return t_vertex(result[0], result[1], result[2]);
}
