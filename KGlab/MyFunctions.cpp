#define _USE_MATH_DEFINES
#include "MyFunctions.h"
#include "Render.h"
#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "GUItextRectangle.h"
#include "MyShaders.h"
#include "Point.h"
#include "Texture.h"
#include "ObjLoader.h"
#include "debout.h"
#include <cmath>

void ColorRGB(int R, int G, int B)
{
	const double maxColor = 255.0;
	glColor3d(R / maxColor, G / maxColor, B / maxColor);
}

void ColorRGBA(int R, int G, int B, int A)
{
	const double maxColor = 255.0;
	glColor4d(R / maxColor, G / maxColor, B / maxColor, A / 100.0);
}



Animation::Animation(double st, double fin)
{
	start_v = st;
	cur_v = st;
	end_v = fin;
	speed = 1;
}
bool Animation::forward()
{
	if(cur_v <= end_v)
	{
		end();
	}
	return speed;
}
bool Animation::backward()
{
	if (cur_v >= end_v)
	{
		end();
	}
	return speed;
}
void Animation::change_speed(double sp)
{
	speed = sp;
}
double Animation::cur_value()
{
	return cur_v;
}
void Animation::start()
{
	cur_v += speed;
}
void Animation::set_cur_to_zero()
{
	cur_v = start_v;
}
void Animation::end()
{
	speed = 0;
}


Point calculate_normal(Point A, Point B, Point C)
{
	//вычисляем стороны-вектора
	Point BA{ A.x - B.x, A.y - B.y, A.z - B.z };
	Point BC{ C.x - B.x, C.y - B.y, C.z - B.z };

	//считаем векторное произведение BA x BC
	Point N{ BA.y * BC.z - BA.z * BC.y, -BA.x * BC.z + BA.z * BC.x, BA.x * BC.y - BA.y * BC.x };

	//Нормализуем нормаль
	double l = sqrt(N.x * N.x + N.y * N.y + N.z * N.z);
	N.x /= l;
	N.y /= l;
	N.z /= l;

	return N;
}

void draw_normal(Point N, Point A, Point B, Point C, Point D)
{
	//вычисляем центр
	Point center{ (A.x + B.x + C.x + D.x) / 4.0, (A.y + B.y + C.y + D.y) / 4.0, (A.z + B.z + C.z + D.z) / 4.0 };

	//тут вектор нормали будет заканчиваться
	Point N_end{ center.x + N.x, center.y + N.y, center.z + N.z };

	//рисуем вектор нормали
	//запоминаем, включено ли освещение
	//чтобы востановить его если нужно
	bool b_light = glIsEnabled(GL_LIGHTING);
	//отключаем освещение чтобы раскрасить glColor
	if (b_light) {
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
		glColor3d(1, 0, 0);
		glVertex3dv((double*)&center);
		glVertex3dv((double*)&N_end);
		glEnd();
		//восстанавливаем освещение, если нужно
		glEnable(GL_LIGHTING);
	}

}

Point texture_coordinates(Point point)
{
	Point TexCord;
	//выставляем значения точек на плоскости 28х28
	TexCord.x = point.y + 15;
	TexCord.y = point.z + 15;

	//делаем значения точек в плоскости 1х1
	TexCord.x /= 30.0;
	TexCord.y /= 30.0;

	return TexCord;
}

void draw_quad(Point A, Point B, Point C, Point D)
{
	//Нормаль
	Point N;
	N = calculate_normal(A, B, C);
	glNormal3dv((double*)&N);

	//Координаты точки в текстуре
	Point TexCord;

	TexCord = texture_coordinates(A);
	glTexCoord2dv((double*)&TexCord);
	glVertex3dv((double*)&A);

	TexCord = texture_coordinates(B);
	glTexCoord2dv((double*)&TexCord);
	glVertex3dv((double*)&B);


	TexCord = texture_coordinates(C);
	glTexCoord2dv((double*)&TexCord);
	glVertex3dv((double*)&C);

	TexCord = texture_coordinates(D);
	glTexCoord2dv((double*)&TexCord);
	glVertex3dv((double*)&D);
}

void draw_triangle(Point A, Point B, Point C)
{
	//Нормаль
	Point N;
	N = calculate_normal(A, B, C);
	glNormal3dv((double*)&N);

	//Координаты точки в текстуре
	Point TexCord;

	TexCord = texture_coordinates(A);
	glTexCoord2dv((double*)&TexCord);
	glVertex3dv((double*)&A);

	TexCord = texture_coordinates(B);
	glTexCoord2dv((double*)&TexCord);
	glVertex3dv((double*)&B);

	TexCord = texture_coordinates(C);
	glTexCoord2dv((double*)&TexCord);
	glVertex3dv((double*)&C);
}

void draw_cylinder(Point Center, double Radius, double Height)
{
	Point CenterShifted = { Center.x, Center.y, Center.z + Height };

	int dis = 1;

	for (int t = 0; t <= 360; t += dis)
	{
		double x = Radius * cos(t * M_PI / 180) + Center.x;
		double y = Radius * sin(t * M_PI / 180) + Center.y;

		Point T = { x, y, Center.z };
		Point TU = { x, y, CenterShifted.z };

		double x1 = Radius * cos((t + dis) * M_PI / 180) + Center.x;
		double y1 = Radius * sin((t + dis) * M_PI / 180) + Center.y;

		Point T1 = { x1, y1, Center.z };
		Point T1U = { x1, y1, CenterShifted.z };

		glBegin(GL_TRIANGLES);

		draw_triangle(Center, T, T1);
		draw_triangle(CenterShifted, T1U, TU);

		glEnd();

		glBegin(GL_QUADS);

		draw_quad(TU, T1U, T1, T);

		glEnd();
	}
}

void draw_cylinder_horizontal(Point Center, double Radius, double Length)
{
	Point CenterShifted = { Center.x + Length, Center.y, Center.z };

	int dis = 1;

	for (int t = 0; t <= 360; t += dis)
	{
		double y = Radius * cos(t * M_PI / 180) + Center.y;
		double z = Radius * sin(t * M_PI / 180) + Center.z;

		Point T = { Center.x, y, z };
		Point TU = { CenterShifted.x, y, z };

		double y1 = Radius * cos((t + dis) * M_PI / 180) + Center.y;
		double z1 = Radius * sin((t + dis) * M_PI / 180) + Center.z;

		Point T1 = { Center.x, y1, z1 };
		Point T1U = { CenterShifted.x, y1, z1 };


		glBegin(GL_TRIANGLES);

		draw_triangle(Center, T, T1);
		draw_triangle(CenterShifted, T1U, TU);

		glEnd();

		glBegin(GL_QUADS);

		draw_quad(TU, T1U, T1, T);

		glEnd();
	}
}

void draw_cone(Point O, double Radius, double h)
{
	Point O1 = { O.x, O.y, O.z + h };
	for (int t = 0; t <= 360; t++)
	{
		double x = Radius * cos(t * M_PI / 180) + O.x;
		double y = Radius * sin(t * M_PI / 180) + O.y;

		Point T = { x, y, O.z };

		double x1 = Radius * cos((t + 1) * M_PI / 180) + O.x;
		double y1 = Radius * sin((t + 1) * M_PI / 180) + O.y;

		Point T1 = { x1, y1, O.z };

		glBegin(GL_TRIANGLES);

		draw_triangle(O1, T1, T);

		glEnd();
	}
}

Point CalculateVector(Point A, Point B)
{
	Point C;
	C.x = B.x - A.x;
	C.y = B.y - A.y;
	C.z = B.z - A.z;
	return C;
}

Point ErmitCurve(Point P0, Point P1, Point R0, Point R1, double t)
{
	double x = ((2 * pow(t, 3) - 3 * pow(t, 2) + 1) * P0.x + (-2 * pow(t, 3) + 3 * pow(t, 2)) * P1.x + (pow(t, 3) - 2 * pow(t, 2) + t) * R0.x + (pow(t, 3) - pow(t, 2)) * R1.x);
	double y = ((2 * pow(t, 3) - 3 * pow(t, 2) + 1) * P0.y + (-2 * pow(t, 3) + 3 * pow(t, 2)) * P1.y + (pow(t, 3) - 2 * pow(t, 2) + t) * R0.y + (pow(t, 3) - pow(t, 2)) * R1.y);
	double z = ((2 * pow(t, 3) - 3 * pow(t, 2) + 1) * P0.z + (-2 * pow(t, 3) + 3 * pow(t, 2)) * P1.z + (pow(t, 3) - 2 * pow(t, 2) + t) * R0.z + (pow(t, 3) - pow(t, 2)) * R1.z);
	Point C = { x, y, z };
	return C;
}

Point BezierCurve2(Point P0, Point P1, Point P2, double t)
{
	double x = pow(1 - t, 2) * P0.x + 2 * t * pow(1 - t, 1) * P1.x + pow(t, 2) * P2.x;
	double y = pow(1 - t, 2) * P0.y + 2 * t * pow(1 - t, 1) * P1.y + pow(t, 2) * P2.y;
	double z = pow(1 - t, 2) * P0.z + 2 * t * pow(1 - t, 1) * P1.z + pow(t, 2) * P2.z;
	Point C = { x, y, z };
	return C;
}

Point BezierCurve3(Point P0, Point P1, Point P2, Point P3, double t)
{
	double x = pow(1 - t, 3) * P0.x + 3 * t * pow(1 - t, 2) * P1.x + 3 * pow(t, 2) * (1 - t) * P2.x + pow(t, 3) * P3.x;
	double y = pow(1 - t, 3) * P0.y + 3 * t * pow(1 - t, 2) * P1.y + 3 * pow(t, 2) * (1 - t) * P2.y + pow(t, 3) * P3.y;
	double z = pow(1 - t, 3) * P0.z + 3 * t * pow(1 - t, 2) * P1.z + 3 * pow(t, 2) * (1 - t) * P2.z + pow(t, 3) * P3.z;
	Point C = { x, y, z };
	return C;
}

double calculate_u(int i, int k, int n)
{
	if (0 <= i and i <= k)
	{
		return 0;
	}
	if (k < i and i <= n)
	{
		return  ((double)(i - k)) / (n - k + 1);
	}
	if (n < i and i <= (n + k + 1))
	{
		return 1;
	}
}

double calculate_N(int i, int k, double* U, double t)
{
	int n = 7;
	int _k = 3;
	if (k == 0)
	{
		if (U[i] <= t and t < U[i + 1])
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		double STT1 = 0, STT2 = 0;
		double cond1 = U[i + k] - U[i];
		if (cond1 != 0)
		{
			STT1 = ((t - U[i]) / cond1) * calculate_N(i, k - 1, U, t);
		}

		double cond2 = U[i + k + 1] - U[i + 1];
		if (cond2 != 0)
		{
			STT2 = ((U[i + k + 1] - t) / cond2) * calculate_N(i + 1, k - 1, U, t);
		}

		return STT1 + STT2;

	}
}

Point B_Spline(int k, int n, Point* P, double* U, double t)
{
	Point C = { 0, 0, 0 };
	for (int i = 0; i <= n; i++) {
		double N = calculate_N(i, k, U, t);
		C.x += N * P[i].x;
		C.y += N * P[i].y;
		C.z += N * P[i].z;
	}
	return C;
}

void PhongColor(int location, Shader phong_sh, float light_pos_v[4], light_phong I, light_phong m, int alpha)
{
	location = glGetUniformLocationARB(phong_sh.program, "Ia");
	glUniform3fARB(location, I.ambient.x, I.ambient.y, I.ambient.z);
	location = glGetUniformLocationARB(phong_sh.program, "Id");
	glUniform3fARB(location, I.diffusion.x, I.diffusion.y, I.ambient.z);
	location = glGetUniformLocationARB(phong_sh.program, "Is");
	glUniform3fARB(location, I.specular.x, I.specular.y, I.specular.z);

	location = glGetUniformLocationARB(phong_sh.program, "ma");
	glUniform3fARB(location, m.ambient.x, m.ambient.y, m.ambient.z);
	location = glGetUniformLocationARB(phong_sh.program, "md");
	glUniform3fARB(location, m.diffusion.x, m.diffusion.y, m.diffusion.z);
	location = glGetUniformLocationARB(phong_sh.program, "ms");
	glUniform4fARB(location, m.specular.x, m.specular.y, m.specular.z, 500);


	location = glGetUniformLocationARB(phong_sh.program, "light_pos_v");
	glUniform3fvARB(location, 1, light_pos_v);
}