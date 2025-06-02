#include "Point.h"
#include "debout.h"
#include "MyShaders.h"
#pragma once

//Перевод цвета из 256 в 1
void ColorRGB(int R, int G, int B);

//Перевод цвета из 256 в 1 c альфа
void ColorRGBA(int R, int G, int B, int A);


//Свет
struct light_phong 
{
	Point ambient = { 0, 0, 0 };
	Point diffusion = { 0, 0, 0 };
	Point specular = { 0, 0, 0 };

	// ( фоновый / дифузный / бликовый ) 
	light_phong(double ambient_red, double ambient_green, double ambient_blue, double diffusion_red, double diffusion_green, double diffusion_blue, double specular_red, double specular_green, double specular_blue)
	{
		double coefficient = 255.0;

		ambient.x = ambient_red / coefficient;
		ambient.y = ambient_green / coefficient;
		ambient.z = ambient_blue / coefficient;

		diffusion.x = diffusion_red / coefficient;
		diffusion.y = diffusion_green / coefficient;
		diffusion.z = diffusion_blue / coefficient;

		specular.x = specular_red / coefficient;
		specular.y = specular_green / coefficient;
		specular.z = specular_blue / coefficient;
	}

};

//Анимация
class Animation
{
	double start_v;
	double cur_v;
	double end_v;
	double speed;

public:

	Animation(double st, double fin);
	bool forward();
	bool backward();
	void change_speed(double sp);
	void set_cur_to_zero();
	double cur_value();
	void start();
	void end();

};

//Вычисляем нормаль
Point calculate_normal(Point A, Point B, Point C);

//Рисуем нормаль
void draw_normal(Point N, Point A, Point B, Point C, Point D);

//Вычисляем координаты в текстуре
Point texture_coordinates(Point point);

//Рисуем квадрат с текстурой
void draw_quad(Point A, Point B, Point C, Point D);

//Рисуем треугольник с текстурой
void draw_triangle(Point A, Point B, Point C);

//Рисуем цилиндр с текстурой
void draw_cylinder(Point Center, double Radius, double Height);

//Рисуем горизонтальный цилиндр
void draw_cylinder_horizontal(Point Center, double Radius, double Length);

//Рисуем конус с текстурой
void draw_cone(Point O, double Radius, double height);

//Вычисляем вектор
Point CalculateVector(Point A, Point B);

//Кривая Эрмита
Point ErmitCurve(Point P0, Point P1, Point R0, Point R1, double t);

//Кривая Безье 2 порядка
Point BezierCurve2(Point P0, Point P1, Point P2, double t);

//Кривая Безье 3 порядка
Point BezierCurve3(Point P0, Point P1, Point P2, Point P3, double t);

//Вычисляем U
double calculate_u(int i, int k, int n);

//Вычисляем N
double calculate_N(int i, int k, double* U, double t);

//Б-сплайн
Point B_Spline(int k, int n, Point* P, double* U, double t);

//Функция чтобы менять цвет у света по фонгу
void PhongColor(int location, Shader phong_sh, float light_pos_v[4], light_phong I, light_phong m, int alpha);