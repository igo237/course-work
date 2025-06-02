#include "Point.h"
#include "debout.h"
#include "MyShaders.h"
#pragma once

//������� ����� �� 256 � 1
void ColorRGB(int R, int G, int B);

//������� ����� �� 256 � 1 c �����
void ColorRGBA(int R, int G, int B, int A);


//����
struct light_phong 
{
	Point ambient = { 0, 0, 0 };
	Point diffusion = { 0, 0, 0 };
	Point specular = { 0, 0, 0 };

	// ( ������� / �������� / �������� ) 
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

//��������
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

//��������� �������
Point calculate_normal(Point A, Point B, Point C);

//������ �������
void draw_normal(Point N, Point A, Point B, Point C, Point D);

//��������� ���������� � ��������
Point texture_coordinates(Point point);

//������ ������� � ���������
void draw_quad(Point A, Point B, Point C, Point D);

//������ ����������� � ���������
void draw_triangle(Point A, Point B, Point C);

//������ ������� � ���������
void draw_cylinder(Point Center, double Radius, double Height);

//������ �������������� �������
void draw_cylinder_horizontal(Point Center, double Radius, double Length);

//������ ����� � ���������
void draw_cone(Point O, double Radius, double height);

//��������� ������
Point CalculateVector(Point A, Point B);

//������ ������
Point ErmitCurve(Point P0, Point P1, Point R0, Point R1, double t);

//������ ����� 2 �������
Point BezierCurve2(Point P0, Point P1, Point P2, double t);

//������ ����� 3 �������
Point BezierCurve3(Point P0, Point P1, Point P2, Point P3, double t);

//��������� U
double calculate_u(int i, int k, int n);

//��������� N
double calculate_N(int i, int k, double* U, double t);

//�-������
Point B_Spline(int k, int n, Point* P, double* U, double t);

//������� ����� ������ ���� � ����� �� �����
void PhongColor(int location, Shader phong_sh, float light_pos_v[4], light_phong I, light_phong m, int alpha);