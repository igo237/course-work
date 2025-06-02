#define _USE_MATH_DEFINES
#include "Render.h"
#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "GUItextRectangle.h"
#include <cmath>
#include "Point.h"
#include "MyFunctions.h"
#include "VRP_Builder.h"

void draw_button(Point BUL, double ButtonWidth, double Radius, int location, Shader phong_sh, float light_pos_v[4], light_phong I)
{
	Point BDL = { BUL.x, BUL.y, BUL.z - 2 };
	Point BUR = { BUL.x, BUL.y + ButtonWidth, BUL.z };
	Point BDR = { BDL.x, BDL.y + ButtonWidth, BDL.z };

	Point BCenterVertical = { BUL.x, BUL.y, (BUL.z + BDL.z) / 2.0 };
	Point BCenterInside = { BUL.x - 5, BUL.y, (BUL.z + BDL.z) / 2.0 };

	Point CenterShifted = { BCenterInside.x, BCenterInside.y + ButtonWidth, BCenterInside.z };
	Point BCenterVShifted = { BCenterVertical.x, BCenterVertical.y + ButtonWidth, BCenterVertical.z };

	int dis = 1;

	for (int t = 0; t <= 360; t += dis)
	{
		double x = Radius * cos(t * M_PI / 180) + BCenterInside.x;
		double z = Radius * sin(t * M_PI / 180) + BCenterInside.z;

		Point T = { x, BCenterInside.y, z };
		Point TU = { x, CenterShifted.y, z };

		double x1 = Radius * cos((t + dis) * M_PI / 180) + BCenterInside.x;
		double z1 = Radius * sin((t + dis) * M_PI / 180) + BCenterInside.z;

		Point T1 = { x1, BCenterInside.y, z1 };
		Point T1U = { x1, CenterShifted.y, z1 };

		if (T1.x>= BUL.x)
		{

			light_phong m(184, 184, 184, 194, 194, 194, 0, 0, 0);	//свет материала
			PhongColor(location, phong_sh, light_pos_v, I, m, 0);
			ColorRGB(194, 194, 194);
			glBegin(GL_QUADS);

			draw_quad(T, T1, T1U, TU);

			glEnd();

			glBegin(GL_TRIANGLES);

			draw_triangle(BCenterVertical, T1, T);
			draw_triangle(BCenterVShifted, TU, T1U);

			glEnd();
		}



	}
}

void draw_twister(Point Center, double RadiusBig, double RadiusSmall, double LengthBig, double LengthSmall, int location, Shader phong_sh, float light_pos_v[4], light_phong I)
{
	//Центры сдвинутые на определённую длинну
	Point CenterBigShifted = { Center.x + LengthBig, Center.y, Center.z };
	Point CenterSmallShifted = { Center.x + LengthSmall, Center.y, Center.z };

	int dis = 1;

	for (int t = 0; t <= 360; t += dis)
	{
		

		double y = RadiusBig * cos(t * M_PI / 180) + Center.y;
		double z = RadiusBig * sin(t * M_PI / 180) + Center.z;

		Point TBig = { Center.x, y, z };
		Point TBigShifted = { CenterBigShifted.x, y, z };

		double y1 = RadiusBig * cos((t + dis) * M_PI / 180) + Center.y;
		double z1 = RadiusBig * sin((t + dis) * M_PI / 180) + Center.z;

		Point TBig_1 = { Center.x, y1, z1 };
		Point TBig_1_Shifted = { CenterBigShifted.x, y1, z1 };

		//Стенки крутилки
		light_phong m_wall(184, 184, 184, 194, 194, 194, 0, 0, 0);	//свет материала
		PhongColor(location, phong_sh, light_pos_v, I, m_wall, 0);
		ColorRGB(194, 194, 194);

		glBegin(GL_QUADS);

		draw_quad(TBig, TBigShifted, TBig_1_Shifted, TBig_1);

		y = RadiusSmall * cos(t * M_PI / 180) + Center.y;
		z = RadiusSmall * sin(t * M_PI / 180) + Center.z;

		Point TSmallShifted = { CenterSmallShifted.x, y, z };

		y1 = RadiusSmall * cos((t + dis) * M_PI / 180) + Center.y;
		z1 = RadiusSmall * sin((t + dis) * M_PI / 180) + Center.z;

		Point TSmall_1_Shifted = { CenterSmallShifted.x, y1, z1 };

		//Внутренние стенки до меньшего круга
		draw_quad(TSmallShifted, TSmall_1_Shifted, TBig_1_Shifted, TBigShifted);

		glEnd();

		light_phong m(12, 12, 12, 22, 22, 22, 0, 0, 0);	//свет материала
		PhongColor(location, phong_sh, light_pos_v, I, m, 0);
		ColorRGB(22, 22, 22);

		glBegin(GL_TRIANGLES);

		//Меньший круг
		draw_triangle(CenterSmallShifted, TSmall_1_Shifted, TSmallShifted);

		glEnd();

	}
}

void Front_Interface_Perch(Point O, int width, double perch_height)
{
	//Линия по середине интерфейса
	Point LP1 = { O.x, O.y - width / 2.0, O.z - perch_height / 2.0 };
	Point LP2 = { LP1.x, LP1.y, LP1.z + 0.25 };
	Point RP1 = { LP1.x, LP1.y + width, LP1.z };
	Point RP2 = { LP2.x, LP2.y + width, LP2.z };

	Point TLP1 = { LP1.x - 0.5, LP1.y, LP1.z - perch_height / 2.0 };
	Point TLP2 = { LP2.x - 0.5, LP2.y, LP2.z + perch_height / 2.0 };
	Point TRP1 = { RP1.x - 0.5, RP1.y, RP1.z - perch_height / 2.0 };
	Point TRP2 = { RP2.x - 0.5, RP2.y, RP2.z + perch_height / 2.0 };

	glBegin(GL_QUADS);
	draw_quad(LP1, LP2, RP2, RP1);
	draw_quad(TLP1, LP1, RP1, TRP1);
	draw_quad(TRP2, RP2, LP2, TLP2);
	glEnd();

}

void Front_Interface_Buttons_and_Twisters(Point LP1Int, Point LP2Int, Point RP1Int, Point RP2Int, int location, Shader phong_sh, float light_pos_v[4], light_phong I)
{
	//Точки для кнопок...

	//...для круглой
	double BRadiusBig = 0.75;
	double BRadiusSmall = 0.65;
	double BLengthBig = 0.1;
	double BLengthSmall = 0.05;
	Point BCenter = { LP2Int.x, LP2Int.y + 6, LP2Int.z - 3 };

	//...для не совсем круглой
	double ButtonWidth = 0.5;
	double ButtonRadius = 5.15;
	double ButtonDistance = 2.25;
	double ButtonDistanceLarger = 3.25;
	Point BUL = { BCenter.x, BCenter.y + 3, BCenter.z + 1 };

	//Точки для крутилок
	double TwRadiusBig = 1.7;
	double TwRadiusSmall = 1.4;
	double TwLengthBig = 1.5;
	double TwLengthSmall = 1.3;
	Point TwCenterUp = { RP2Int.x, RP2Int.y - 3.5, RP2Int.z - 2.5 };
	Point TwCenterDown = { RP1Int.x, RP1Int.y - 3.5, RP1Int.z + 2.5 };

	//Кнопки
	draw_twister(BCenter, BRadiusBig, BRadiusSmall, BLengthBig, BLengthSmall, location, phong_sh, light_pos_v, I);

	for (int i = 1; i <= 10; i++)
	{
		draw_button(BUL, ButtonWidth, ButtonRadius, location, phong_sh, light_pos_v, I);

		if (i != 3)
		{
			BUL.y += ButtonDistance;
		}
		else
		{
			BUL.y += ButtonDistanceLarger;
		}
	}

	//Крутилки
	draw_twister(TwCenterUp, TwRadiusBig, TwRadiusSmall, TwLengthBig, TwLengthSmall, location, phong_sh, light_pos_v, I);
	draw_twister(TwCenterDown, TwRadiusBig, TwRadiusSmall, TwLengthBig, TwLengthSmall, location, phong_sh, light_pos_v, I);
}

void VRP_Base(int location, Shader phong_sh, float light_pos_v[4], light_phong I)
{
	int h = 13;
	int w = 40 / 2;
	int l = 36 / 2;

	Point N;

	Point O = { 0, 0, 2 };

	Point P1 = { O.x - l, O.y - w, O.z };
	Point P2 = { O.x - l, O.y + w, O.z };
	Point P3 = { O.x + l, O.y + w, O.z };
	Point P4 = { O.x + l, O.y - w, O.z };

	Point P1U = { P1.x, P1.y, P1.z + h };
	Point P2U = { P2.x, P2.y, P2.z + h };
	Point P3U = { P3.x, P3.y, P3.z + h };
	Point P4U = { P4.x, P4.y, P4.z + h };


	//Точки для переднего интерфейса
	Point FP1 = { P3.x, P3.y - 1, P3.z + 1 };
	Point FP2 = { P3U.x, P3U.y - 1, P3U.z - 1 };
	Point FP3 = { P4U.x, P4U.y + 1, P4U.z - 1 };
	Point FP4 = { P4.x, P4.y + 1, P4.z + 1 };

	Point FP1I = { FP1.x - 0.5, FP1.y, FP1.z };
	Point FP2I = { FP2.x - 0.5, FP2.y, FP2.z };
	Point FP3I = { FP3.x - 0.5, FP3.y, FP3.z };
	Point FP4I = { FP4.x - 0.5, FP4.y, FP4.z };


	//Точки для заднего интерфейса
	Point BP1 = { P2.x, P2.y - 1, P2.z + 1 };
	Point BP2 = { P2U.x, P2U.y - 1, P2U.z - 1 };
	Point BP3 = { P1U.x, P1U.y + 1, P1U.z - 1 };
	Point BP4 = { P1.x, P1.y + 1, P1.z + 1 };

	Point BP1I = { BP1.x + 0.3, BP1.y, BP1.z };
	Point BP2I = { BP2.x + 0.3, BP2.y, BP2.z };
	Point BP3I = { BP3.x + 0.3, BP3.y, BP3.z };
	Point BP4I = { BP4.x + 0.3, BP4.y, BP4.z };

	light_phong m(20, 20, 20, 40, 40, 40, 0, 0, 0);
	PhongColor(location, phong_sh, light_pos_v, I, m, 0);
	ColorRGB(40, 40, 40);

	glBegin(GL_QUADS);


	draw_quad(P4, P3, P2, P1);
	draw_quad(P3, P3U, P2U, P2);
	draw_quad(P1, P1U, P4U, P4);
	draw_quad(P1U, P2U, P3U, P4U);

	//Вырез для переднего интерфейса
	draw_quad(P3, FP1, FP2, P3U);
	draw_quad(P4U, FP3, FP4, P4);
	draw_quad(P4, FP4, FP1, P3);
	draw_quad(P3U, FP2, FP3, P4U);

	draw_quad(FP1, FP1I, FP2I, FP2);
	draw_quad(FP2, FP2I, FP3I, FP3);
	draw_quad(FP3, FP3I, FP4I, FP4);
	draw_quad(FP4, FP4I, FP1I, FP1);

	//Вырез для заднего интерфейса
	draw_quad(P1, BP4, BP3, P1U);
	draw_quad(P2U, BP2, BP1, P2);
	draw_quad(P2, BP1, BP4, P1);
	draw_quad(P1U, BP3, BP2, P2U);

	draw_quad(BP2, BP2I, BP1I, BP1);
	draw_quad(BP3, BP3I, BP2I, BP2);
	draw_quad(BP4, BP4I, BP3I, BP3);
	draw_quad(BP1, BP1I, BP4I, BP4);


	glEnd();

}

void VRP_Platter(int location, Shader phong_sh, float light_pos_v[4], light_phong I)
{
	light_phong m(0, 0, 0, 2, 2, 2, 0, 0, 0);	//свет материала
	PhongColor(location, phong_sh, light_pos_v, I, m, 0);
	glColor3d(0, 0, 0);

	float h = 0.125;
	int Radius = 11;
	Point O = { 0, 0, 15 };
	draw_cylinder(O, Radius, h);

	O.z += h;
	Radius = 13;
	h = 1;
	draw_cylinder(O, Radius, h);
}

void VRP_Legs(int location, Shader phong_sh, float light_pos_v[4], light_phong I)
{
	int Radius = 1;
	int dist = 6;
	int l = 40 - dist;
	int w = 36 - dist;
	float h = 2;
	Point O{ w / 2, l / 2, 0 };

	light_phong m(0, 0, 0,  2, 2, 2,  0, 0, 0);	//свет материала
	PhongColor(location, phong_sh, light_pos_v, I, m, 0);
	ColorRGB(0, 0, 0);

	draw_cylinder(O, Radius, h);

	O.y -= l;
	draw_cylinder(O, Radius, h);

	O.x -= w;
	draw_cylinder(O, Radius, h);

	O.y += l;
	draw_cylinder(O, Radius, h);
}

void VRP_Spindel(int location, Shader phong_sh, float light_pos_v[4], light_phong I)
{
	float h = 1;
	double Radius = 0.5;
	Point O = { 0, 0, 16.125 };
	light_phong m(121, 121, 121, 180, 180, 180, 0, 0, 0);	//свет материала
	PhongColor(location, phong_sh, light_pos_v, I, m, 0);
	ColorRGB(204, 204, 204);
	draw_cylinder(O, Radius, h);
	O.z += h;
	draw_cone(O, Radius, 0.5);

}

void VRP_Lever_Base(int location, Shader phong_sh, float light_pos_v[4], light_phong I)
{
	//Часть 1
	float h = 0.5;
	float Radius = 14;
	int c = 0;
	int c1 = 0;

	Point P1 = { -15, 17, 15 };
	Point P2 = { P1.x + 13, P1.y, P1.z };
	Point P3 = { P2.x, P2.y - 3.125, P1.z };
	Point P4 = { P1.x, P1.y - 9.5, P1.z };

	Point P1U = { P1.x, P1.y, P1.z + h };
	Point P2U = { P2.x, P2.y, P1U.z };
	Point P3U = { P3.x, P3.y, P1U.z };
	Point P4U = { P4.x, P4.y, P1U.z };

	Point O = { 0, 0, P1.z };
	Point O1 = { O.x, O.y, O.z + h };

	Point TP = { (P1.x - P2.x) / 2, P2.y, P2.z + h };

	light_phong m(5.3, 5.3, 5.3, 15.3, 15.3, 15.3, 0, 0, 0);	//свет материала
	PhongColor(location, phong_sh, light_pos_v, I, m, 0);
	glColor3d(0.06, 0.06, 0.06);

	for (int t = 0; t <= 360; t++)
	{
		double x = Radius * cos(t * M_PI / 180) + O.x;
		double y = Radius * sin(t * M_PI / 180) + O.y;

		Point T = { x, y, O.z };
		Point TU = { x, y, O1.z };

		double x1 = Radius * cos((t + 1) * M_PI / 180) + O.x;
		double y1 = Radius * sin((t + 1) * M_PI / 180) + O.y;

		Point T1 = { x1, y1, O.z };
		Point T1U = { x1, y1, O1.z };



		if (TP.x > P1.x and T.x <= P2.x and T.y > P4.y - 1)
		{
			glBegin(GL_TRIANGLES);
			draw_triangle(TU, T1U, TP);
			glEnd();

			glBegin(GL_QUADS);
			draw_quad(T, T1, T1U, TU);
			glEnd();

			if (c1 == 0)
			{
				glBegin(GL_QUADS);
				draw_quad(TU, P3U, P3, T);
				draw_quad(TP, P2U, P3U, TU);
				glEnd();
				c1++;
			}
		}

		if (T.y < P4.y - 1 and T.x <= P2.x)
		{
			if (c == 0)
			{
				glBegin(GL_QUADS);
				draw_quad(T, P4, P4U, TU);
				draw_quad(TU, P4U, P1U, TP);
				glEnd();
				c++;
			}
		}

	}

	glBegin(GL_QUADS);
	draw_quad(P2, P2U, P1U, P1);
	draw_quad(P3, P3U, P2U, P2);
	draw_quad(P1, P1U, P4U, P4);
	glEnd();

	light_phong m_p1(12.95, 12.95, 12.95, 22.95, 22.95, 22.95, 0, 0, 0);	//свет материала
	PhongColor(location, phong_sh, light_pos_v, I, m_p1, 0);
	glColor3d(0.09, 0.09, 0.09);
	Radius = 1.125;
	h = 0.125;
	O = { P1.x + 4,P1.y - 2, P1U.z };
	draw_cylinder(O, Radius, h);

	O.z += h;
	Radius = 1;
	h = 0.8;
	draw_cylinder(O, Radius, h);


	//Часть 2
	O = { P1.x + 6.125,P1.y - 2, P1U.z };
	h = 1.2;
	Radius = 0.125;
	draw_cylinder(O, Radius, h);

	O.x += 0.5;
	draw_cylinder(O, Radius, h);


	O.z += h;
	Point P6 = { O.x + 0.5, O.y - 0.25, O.z };
	Point P7 = { P6.x, P6.y + 0.5, P6.z };
	Point P8 = { P7.x - 0.75, P7.y, P7.z };
	Point P9 = { P8.x, P8.y - 0.5, P8.z };

	h = 0.2;

	Point P6U = { P6.x, P6.y, P6.z + h };
	Point P7U = { P7.x, P7.y, P6U.z };
	Point P8U = { P8.x, P8.y, P6U.z };
	Point P9U = { P9.x, P9.y, P6U.z };
	glBegin(GL_QUADS);
	draw_quad(P6, P7, P8, P9);
	draw_quad(P9U, P8U, P7U, P6U);
	draw_quad(P6, P6U, P7U, P7);
	draw_quad(P7, P7U, P8U, P8);
	draw_quad(P9, P9U, P6U, P6);
	glEnd();

	O = { P1.x + 4, P1.y - 2, O.z };
	O1 = { O.x, O.y, O.z + h };
	Radius = 1.5;
	h = 0.4;
	c = 0;
	c1 = 0;
	for (int t = 0; t <= 360; t++)
	{
		double x = Radius * cos(t * M_PI / 180) + O.x;
		double y = Radius * sin(t * M_PI / 180) + O.y;

		Point T = { x, y, O.z };
		Point TU = { x, y, O1.z };

		double x1 = Radius * cos((t + 1) * M_PI / 180) + O.x;
		double y1 = Radius * sin((t + 1) * M_PI / 180) + O.y;

		Point T1 = { x1, y1, O.z };
		Point T1U = { x1, y1, O1.z };

		float R = Radius + 0.35;

		double xR = R * cos(t * M_PI / 180) + O.x;
		double yR = R * sin(t * M_PI / 180) + O.y;

		Point TR = { xR, yR, O.z };
		Point TRU = { xR, yR, O1.z };

		double xR1 = R * cos((t + 1) * M_PI / 180) + O.x;
		double yR1 = R * sin((t + 1) * M_PI / 180) + O.y;

		Point TR1 = { xR1, yR1, O.z };
		Point TR1U = { xR1, yR1, O1.z };

		if (y > P6.y - 0.6 and y < P7.y + 0.3 and x > O.x)
		{
			glBegin(GL_QUADS);
			draw_quad(T1, T1U, TU, T);
			draw_quad(TR, TRU, TR1U, TR1);
			draw_quad(T, TR, TR1, T1);
			draw_quad(T1U, TR1U, TRU, TU);
			if (y > P6.y - 0.6 and y < P6.y - 0.5 and x > O.x and c == 1)
			{
				draw_quad(TU, TRU, TR, T);
				c++;
			}
			if (y < P7.y + 0.3 and y > P7.y + 0.28 and x > O.x and c == 0)
			{
				draw_quad(T, TR, TRU, TU);
				c++;
			}
			glEnd();

			Point P9R = { P9.x, TR.y, P9.z };
			Point P9R1 = { P9.x, TR1.y, P9.z };

			Point P9RU = { P9R.x, P9R.y, TRU.z };
			Point P9R1U = { P9R1.x, P9R1.y, TRU.z };
			if (yR >= P9.y and yR1 <= P8.y)
			{
				glBegin(GL_QUADS);
				draw_quad(P9R, P9R1, TR1, TR);
				draw_quad(P9R1U, P9RU, TRU, TR1U);
				glEnd();
			}
			else
			{

				if (yR1 < P8.y + 0.1 and c1 == 0)
				{
					glBegin(GL_TRIANGLES);
					draw_triangle(P9R, P9R1, TR);
					draw_triangle(P9R1U, P9RU, TRU);
					glEnd();

					glBegin(GL_QUADS);
					draw_quad(P9R1, P9R1U, TRU, TR);
					glEnd();
					c1++;
				}

				if (yR > P9.y - 0.01 and yR <= P9.y and c1 == 1)
				{
					glBegin(GL_QUADS);
					draw_quad(P9R1U, P9R1, TR, TRU);
					glEnd();

					glBegin(GL_TRIANGLES);
					draw_triangle(P9R, P9R1, TR);
					draw_triangle(P9R1U, P9RU, TRU);
					glEnd();
					c1++;
				}


			}
		}


	}


	//Часть 3
	h = 0.8;
	Radius = 0.65;
	O = { P2.x - 1.25, P1.y - 2, P1U.z };
	draw_cylinder(O, Radius, h);

	O.z += h;
	h = 0.5;
	Radius = 0.5;
	draw_cylinder(O, Radius, h);


	light_phong m_p2(10, 10, 10, 20, 20, 20, 0, 0, 0);	//свет материала
	PhongColor(location, phong_sh, light_pos_v, I, m_p2, 0);
	ColorRGB(20, 20, 20);
	O.z += h;
	h = 0.4;
	Point P10 = { O.x + Radius / 2 + 0.25 / 2, O.y + Radius, O.z };
	Point P11 = { P10.x - Radius - 0.25, P10.y, P10.z };
	Point P12 = { P11.x, P11.y - Radius * 2, P11.z };
	Point P13 = { P12.x + Radius + 0.25, P12.y, P12.z };

	Point P10U = { P10.x, P10.y, P10.z + h * 4 };
	Point P11U = { P11.x, P11.y, P10U.z };

	Point P14 = { P10.x, P10.y - 0.0625, P10.z };
	Point P15 = { P11.x, P14.y, P10.z };
	Point P14U = { P10U.x, P10U.y - 0.0625, P10U.z };
	Point P15U = { P11U.x, P14U.y, P10U.z };

	Point P12U = { P12.x, P12.y, P12.z + h };
	Point P13U = { P13.x, P13.y, P12U.z };

	Point P16 = { P12.x, P12.y + 0.0625, P12.z };
	Point P17 = { P13.x, P16.y, P12.z };
	Point P16U = { P12U.x, P12U.y + 0.0625, P12U.z };
	Point P17U = { P13U.x, P16U.y, P12U.z };

	Point P14UD = { P14U.x, P14U.y, P17U.z };
	Point P15UD = { P15U.x, P15U.y, P14UD.z };

	glBegin(GL_QUADS);
	draw_quad(P10, P11, P12, P13);
	draw_quad(P11, P10, P10U, P11U);
	draw_quad(P13, P12, P12U, P13U);
	draw_quad(P10, P14, P14U, P10U);
	draw_quad(P17, P13, P13U, P17U);
	draw_quad(P15, P11, P11U, P15U);
	draw_quad(P12, P16, P16U, P12U);
	draw_quad(P14U, P15U, P11U, P10U);
	draw_quad(P13U, P12U, P16U, P17U);
	draw_quad(P15U, P14U, P14UD, P15UD);

	glEnd();

	//Количество точек
	int samples = 25;
	//шаг между точками
	double step = 1. / (samples - 1);

	Point PT1;
	Point PT2;
	Point PT3;
	Point PT4;

	Point PT1D;
	Point PT2D;
	Point PT3D;
	Point PT4D;

	P14U.z = P17U.z;
	P15U.z = P14U.z;



	for (int i = 0; i < samples - 1; ++i)
	{
		double t = 0 + i * step;
		double t1 = 0 + (i + 1) * step;

		PT1 = BezierCurve3(P14U, P14, P17, P17U, t);
		PT2 = BezierCurve3(P14U, P14, P17, P17U, t1);
		PT3 = BezierCurve3(P15U, P15, P16, P16U, t);
		PT4 = BezierCurve3(P15U, P15, P16, P16U, t1);

		PT1D = { PT1.x, PT1.y, P10.z };
		PT2D = { PT2.x, PT2.y, PT1D.z };
		PT3D = { PT3.x, PT3.y, PT2D.z };
		PT4D = { PT4.x, PT4.y, PT3D.z };

		glBegin(GL_QUADS);
		draw_quad(PT1, PT2, PT4, PT3);
		draw_quad(PT2, PT1, PT1D, PT2D);
		draw_quad(PT3, PT4, PT4D, PT3D);
		glEnd();

	}

}

void VRP_Lever_Needle(int location, Shader phong_sh, float light_pos_v[4], light_phong I)
{
	
	//Противовес
	float h = 2;
	float Radius = 14;

	

	//Point O = { -11, 15, 16.425 };

	Point O = { 0, 0, 0 };



	Point P1 = { O.x + 1, O.y - 2, O.z };
	Point P2 = { P1.x, P1.y + 4, P1.z };
	Point P3 = { P2.x - 3, P2.y, P1.z };
	Point P4 = { P3.x, P3.y - 4, P1.z };
	Point P5 = { P4.x - 2, P4.y + 1, P4.z };

	Point P1U = { P1.x, P1.y, P1.z + h };
	Point P2U = { P2.x, P2.y, P1U.z };
	Point P3U = { P3.x, P3.y, P1U.z };
	Point P4U = { P4.x, P4.y, P1U.z };
	Point P5U = { P5.x, P5.y, P1U.z };

	
	light_phong m(32, 32, 32, 42, 42, 42, 0, 0, 0);	//свет материала
	PhongColor(location, phong_sh, light_pos_v, I, m, 0);
	ColorRGB(42, 42, 42);
	glBegin(GL_QUADS);
	draw_quad(P1, P2, P3, P4);
	draw_quad(P1, P1U, P2U, P2);
	draw_quad(P2, P2U, P3U, P3);
	draw_quad(P4, P4U, P1U, P1);
	draw_quad(P3, P3U, P5U, P5);
	draw_quad(P5, P5U, P4U, P4);
	draw_quad(P4U, P3U, P2U, P1U);
	glEnd();

	glBegin(GL_TRIANGLES);
	draw_triangle(P3, P5, P4);
	draw_triangle(P3U, P4U, P5U);
	glEnd();

	O = { P1.x, P1.y + 2, P1.z + h / 2 };

	int length = 12;
	Radius = 0.45;

	//Цилиндрическая часть рычага
	light_phong m_cylinder(200, 200, 200, 220, 220, 220, 0, 0, 0);	//свет материала
	PhongColor(location, phong_sh, light_pos_v, I, m_cylinder, 0);
	ColorRGB(220, 220, 220);
	draw_cylinder_horizontal(O, Radius, length);

	O.x += length;

	//Игла
	Point P6 = { O.x, O.y + 0.5, O.z - 0.5 };
	Point P7 = { P6.x + 1, P6.y + 0.5, P6.z };
	Point P8 = { P7.x + 0.5, P7.y - 0.5, P7.z };
	Point P9 = { P8.x, P8.y - 1, P8.z };
	Point P10 = { P9.x - 1.5, P9.y, P9.z };

	Point PN11 = { P8.x + 5, P8.y - 2.25, P8.z };
	Point PN12 = { P9.x + 4.7, P9.y - 2, P9.z };
	Point PN13 = { PN11.x - 1.67, PN11.y + 0.75, PN11.z };
	Point PN14 = { PN12.x - 1.57, PN12.y + 0.67, PN12.z };

	Point PN15 = { PN11.x - 0.334, PN11.y + 0.15, PN11.z };
	Point PN16 = { PN12.x - 0.314, PN12.y + 0.134, PN12.z };
	Point PN17 = { PN13.x + 0.334, PN13.y - 0.15, PN13.z };
	Point PN18 = { PN14.x + 0.314, PN14.y - 0.134, PN14.z };

	Point PN15D = { (PN15.x + PN16.x) / 2, (PN15.y + PN16.y) / 2, PN15.z - 0.2 };
	Point PN16D = { (PN15.x + PN16.x) / 2, (PN15.y + PN16.y) / 2, PN15.z - 0.15 };
	Point PN17D = { (PN17.x + PN18.x) / 2, (PN17.y + PN18.y) / 2, PN16.z };
	Point PN18D = { (PN17.x + PN18.x) / 2, (PN17.y + PN18.y) / 2, PN16.z - 0.01 };

	Point PN11U = { PN11.x, PN11.y, PN11.z + 0.2 };
	Point PN12U = { PN12.x, PN12.y, PN12.z + 0.2 };
	Point PN13U = { PN13.x, PN13.y, PN13.z + 0.2 };
	Point PN14U = { PN14.x, PN14.y, PN14.z + 0.2 };


	Point P6U = { P6.x, P6.y, P6.z + 1 };
	Point P7U = { P7.x, P7.y, P6U.z };
	Point P8U = { P8.x, P8.y, P6U.z };
	Point P9U = { P9.x, P9.y, P6U.z };
	Point P10U = { P10.x, P10.y, P6U.z };
	Point P11U = { PN11.x, PN11.y, P6U.z };
	Point P12U = { PN12.x, PN12.y, P6U.z };
	Point P13U = { PN13.x, PN13.y, P6U.z };
	Point P14U = { PN14.x, PN14.y, P6U.z };

	//Темная часть иглы
	light_phong m_dark(21, 21, 21, 31, 31, 31, 0, 0, 0);	//свет материала
	PhongColor(location, phong_sh, light_pos_v, I, m_dark, 0);
	ColorRGB(31, 31, 31);
	glBegin(GL_QUADS);
	draw_quad(P8, P6, P10, P9);
	draw_quad(P10, P6, P6U, P10U);
	draw_quad(P9, P10, P10U, P9U);
	draw_quad(P8, P9, PN14, PN13);
	draw_quad(P9U, P14U, PN14, P9);
	draw_quad(P8, PN13, P13U, P8U);
	draw_quad(P6U, P8U, P9U, P10U);
	draw_quad(P9U, P8U, P11U, P12U);

	draw_quad(P6, P7, P7U, P6U);
	draw_quad(P7, P8, P8U, P7U);

	draw_quad(PN13U, PN11U, P11U, P13U);
	draw_quad(PN12U, PN14U, P14U, P12U);
	draw_quad(PN11U, PN12U, P12U, P11U);
	glEnd();

	//Красная часть иглы
	light_phong m_red(209, 0, 0, 219, 0, 0, 0, 0, 0);	//свет материала
	PhongColor(location, phong_sh, light_pos_v, I, m_red, 0);
	ColorRGB(219, 0, 0);
	glBegin(GL_QUADS);
	draw_quad(PN11, PN13, PN14, PN12);
	draw_quad(PN12, PN14, PN14U, PN12U);
	draw_quad(PN13, PN11, PN11U, PN13U);
	draw_quad(PN11, PN12, PN12U, PN11U);
	glEnd();

	//Сама игла
	light_phong m_needle(137, 137, 137, 147, 147, 147, 0, 0, 0);	//свет материала
	PhongColor(location, phong_sh, light_pos_v, I, m_needle, 0);
	ColorRGB(147, 147, 147);
	glBegin(GL_QUADS);
	draw_quad(PN15D, PN16D, PN17D, PN18D);
	glEnd();

	light_phong m_part(21, 21, 21, 31, 31, 31, 0, 0, 0);	//свет материала
	PhongColor(location, phong_sh, light_pos_v, I, m_part, 0);
	ColorRGB(31, 31, 31);
	glBegin(GL_TRIANGLES);
	draw_triangle(P6, P8, P7);
	draw_triangle(P6U, P7U, P8U);
	glEnd();
}

void VRP_Cover(int location, Shader phong_sh, float light_pos_v[4], light_phong I)
{
	int width = 37;
	int length = 34;
	double height1 = 4.5;
	int height2 = 6;

	//Середина
	Point O = { 0, 0, 0 };

	//Левая сторона крышки
	Point LP1 = { O.x + 1, O.y - width / 2., O.z };
	Point LP2 = { LP1.x, LP1.y + 0.5, LP1.z + height1 };
	Point LP3 = { LP2.x + 1.5, LP2.y, LP2.z + 1.5 };
	Point LP4 = { LP3.x + 18, LP3.y, LP3.z };
	Point LP5 = { LP4.x + 13.5, LP4.y, LP4.z - 3 };
	Point LP6 = { O.x + length + 1, O.y - width / 2., O.z };
	Point LP7 = { O.x, O.y - width / 2., O.z };

	Point TLP1 = { LP1.x + 1.5, LP1.y, LP1.z };
	Point TLP3 = { LP3.x, LP3.y - 0.5, LP3.z - 1.5 };
	Point TLP4 = { LP4.x, LP4.y - 0.5, LP4.z - 1.5 };
	Point TLP5 = { LP5.x, LP5.y - 0.5, LP5.z - 3 };
	Point TLPP5 = { LP5.x + 0.57, LP5.y - 0.5, LP5.z - 1.5 };
	Point TLP6 = { LP6.x - 14.5, LP6.y, LP6.z };


	//Правая сторона крышки
	Point RP1 = { LP1.x, LP1.y + width, LP1.z };
	Point RP2 = { LP2.x, LP2.y + width - 1, LP2.z };
	Point RP3 = { LP3.x, LP3.y + width - 1, LP3.z };
	Point RP4 = { LP4.x, LP4.y + width - 1, LP4.z };
	Point RP5 = { LP5.x, LP5.y + width - 1, LP5.z };
	Point RP6 = { LP6.x, LP6.y + width, LP6.z };
	Point RP7 = { LP7.x, LP7.y + width, LP7.z };

	Point TRP1 = { TLP1.x, TLP1.y + width, TLP1.z };
	Point TRP3 = { TLP3.x, TLP3.y + width, TLP3.z };
	Point TRP4 = { TLP4.x, TLP4.y + width, TLP4.z };
	Point TRP5 = { TLP5.x, TLP5.y + width, TLP5.z };
	Point TRPP5 = { TLPP5.x, TLPP5.y + width, TLPP5.z };
	Point TRP6 = { TLP6.x, TLP6.y + width, TLP6.z };


	light_phong m(90, 90, 90, 100, 100, 100, 0, 0, 0);	//свет материала
	PhongColor(location, phong_sh, light_pos_v, I, m, 0);
	ColorRGBA(100, 100, 100, 55);

	//Левая сторона крышки
	glBegin(GL_QUADS);

	draw_quad(LP1, LP2, TLP3, TLP1);
	draw_quad(LP3, LP4, TLP4, TLP3);
	draw_quad(TLP3, TLP4, TLP6, TLP1);
	draw_quad(TLPP5, TLP5, TLP6, TLP4);
	draw_quad(LP5, TLPP5, TLP4, LP4);

	glEnd();

	glBegin(GL_TRIANGLES);

	draw_triangle(LP2, LP3, TLP3);
	draw_triangle(LP2, LP1, LP7);
	draw_triangle(TLP5, TLPP5, LP6);

	glEnd();

	//Правая сторона крышки
	glBegin(GL_QUADS);

	draw_quad(RP2, RP1, TRP1, TRP3);
	draw_quad(RP4, RP3, TRP3, TRP4);
	draw_quad(TRP4, TRP3, TRP1, TRP6);
	draw_quad(TRP5, TRPP5, TRP4, TRP6);
	draw_quad(RP4, TRP4, TRPP5, RP5);

	glEnd();

	glBegin(GL_TRIANGLES);

	draw_triangle(RP3, RP2, TRP3);
	draw_triangle(RP7, RP1, RP2);
	draw_triangle(RP6, TRPP5, TRP5);

	glEnd();

	glBegin(GL_QUADS);

	//Задняя сторона крышки
	draw_quad(RP7, RP2, LP2, LP7);
	draw_quad(RP2, RP3, LP3, LP2);

	//Передняя сторона крышки
	draw_quad(TRPP5, TLPP5, LP5, RP5);
	draw_quad(LP6, TLPP5, TRPP5, RP6);

	//Верхняя часть крышки
	draw_quad(LP3, RP3, RP4, LP4);
	draw_quad(RP5, LP5, LP4, RP4);

	glEnd();
}

void VRP_Front_Interface(int location, Shader phong_sh, float light_pos_v[4], light_phong I)
{
	int width = 38;
	int height = 11;

	Point O = { 18, 0, 3 + height / 2.0 };

	//Точки для фона интерфейса
	Point LP1Int = { O.x - 0.5, O.y - width / 2.0, O.z - height / 2.0 };
	Point LP2Int = { LP1Int.x, LP1Int.y, LP1Int.z + height };
	Point RP1Int = { LP1Int.x, LP1Int.y + width, LP1Int.z };
	Point RP2Int = { LP2Int.x, LP2Int.y + width, LP2Int.z };

	//Точки чтобы вырезать часы
	Point LP1IntClock = { LP1Int.x, LP1Int.y + 4 - 0.3, LP1Int.z + 1 - 0.3 };
	Point LP2IntClock = { LP1IntClock.x, LP1IntClock.y, LP1IntClock.z + 3 + 0.6 };

	Point RP1IntClock = { LP1IntClock.x, LP1IntClock.y + 13.5, LP1IntClock.z };
	Point RP2IntClock = { LP1IntClock.x, LP1IntClock.y + 13.5, LP2IntClock.z };

	//Точки чтобы вырезать дисковод
	Point LP1IntDisk = { RP2IntClock.x, RP2IntClock.y + 1.75, RP2IntClock.z - 0.5 };
	Point LP2IntDisk = { LP1IntDisk.x, LP1IntDisk.y, LP1IntDisk.z - 1.75 };
	Point RP1IntDisk = { LP1IntDisk.x, LP1IntDisk.y + 13, LP1IntDisk.z };
	Point RP2IntDisk = { LP2IntDisk.x, LP2IntDisk.y + 13, LP2IntDisk.z };

	//Точки для дисковода
	double dist = 0.1;
	Point LP1Disk = { LP1IntDisk.x - dist, LP1IntDisk.y + dist, LP1IntDisk.z - dist };
	Point LP2Disk = { LP2IntDisk.x - dist, LP2IntDisk.y + dist, LP2IntDisk.z + dist };
	Point RP1Disk = { RP1IntDisk.x - dist, RP1IntDisk.y - dist, RP1IntDisk.z - dist };
	Point RP2Disk = { RP2IntDisk.x - dist, RP2IntDisk.y - dist, RP2IntDisk.z + dist };

	Point LP1DiskShifted = { LP1Disk.x + dist, LP1Disk.y, LP1Disk.z };
	Point LP2DiskShifted = { LP2Disk.x + dist, LP2Disk.y, LP2Disk.z };
	Point RP1DiskShifted = { RP1Disk.x + dist, RP1Disk.y, RP1Disk.z };
	Point RP2DiskShifted = { RP2Disk.x + dist, RP2Disk.y, RP2Disk.z };

	//Точки для часов
	Point LP1Clock = { LP1Int.x - 0.25, LP1Int.y + 4, LP1Int.z + 1 };
	Point LP2Clock = { LP1Clock.x, LP1Clock.y, LP1Clock.z + 3 };
	Point RP1Clock = { LP1Clock.x, LP1Clock.y + 12.5, LP1Clock.z };
	Point RP2Clock = { LP2Clock.x, LP2Clock.y + 12.5, LP2Clock.z };

	//Кнопки и крутилки
	Front_Interface_Buttons_and_Twisters(LP1Int, LP2Int, RP1Int, RP2Int, location, phong_sh, light_pos_v, I);

	//Жёрдочка по середине
	light_phong m(150, 150, 150, 194, 194, 194, 0, 0, 0);	//свет материала
	PhongColor(location, phong_sh, light_pos_v, I, m, 0);
	ColorRGB(194, 194, 194);
	Front_Interface_Perch(O, width, 0.25);

	light_phong m_disk(53, 77, 99, 63, 87, 109, 0, 0, 0);	//свет материала
	PhongColor(location, phong_sh, light_pos_v, I, m_disk, 0);
	glBegin(GL_QUADS);

	//Фон с вырезом под часы и дисковод
	ColorRGB(63, 87, 109);
	//Верхняя часть
	draw_quad(RP2Int, RP2IntClock, LP2IntClock, LP2Int);
	//Нижняя часть
	draw_quad(LP1Int, LP1IntClock, RP1IntClock, RP1Int);
	//Левая часть
	draw_quad(LP2Int, LP2IntClock, LP1IntClock, LP1Int);
	//Правая часть
	draw_quad(RP1IntClock, RP2IntClock, LP1IntDisk, LP2IntDisk);
	draw_quad(RP1IntClock, LP2IntDisk, RP2IntDisk, RP1Int);
	draw_quad(RP2Int, RP1Int, RP2IntDisk, RP1IntDisk);
	draw_quad(RP2Int, RP1IntDisk, LP1IntDisk, RP2IntClock);

	//Фон для часов
	draw_quad(LP1Clock, LP1IntClock, LP2IntClock, LP2Clock);
	draw_quad(LP2Clock, LP2IntClock, RP2IntClock, RP2Clock);
	draw_quad(RP2Clock, RP2IntClock, RP1IntClock, RP1Clock);
	draw_quad(RP1Clock, RP1IntClock, LP1IntClock, LP1Clock);
	//Фон для дисковода
	draw_quad(LP1Disk, LP1IntDisk, RP1IntDisk, RP1Disk);
	draw_quad(RP2Disk, RP2IntDisk, LP2IntDisk, LP2Disk);
	draw_quad(LP2Disk, LP2IntDisk, LP1IntDisk, LP1Disk);
	draw_quad(RP1Disk, RP1IntDisk, RP2IntDisk, RP2Disk);

	//Дисковод
	ColorRGB(63, 87, 109);
	draw_quad(RP1Disk, RP1DiskShifted, LP1DiskShifted, LP1Disk);
	draw_quad(LP2Disk, LP2DiskShifted, RP2DiskShifted, RP2Disk);
	draw_quad(LP1Disk, LP1DiskShifted, LP2DiskShifted, LP2Disk);
	draw_quad(RP2Disk, RP2DiskShifted, RP1DiskShifted, RP1Disk);
	draw_quad(RP1DiskShifted, RP2DiskShifted, LP2DiskShifted, LP1DiskShifted);

	

	glEnd();


	light_phong m_clock(15, 15, 15, 25, 25, 25, 125, 125, 125);	//свет материала
	PhongColor(location, phong_sh, light_pos_v, I, m_clock, 0);
	glBegin(GL_QUADS);
	//Часы
	ColorRGB(25, 25, 25);
	draw_quad(LP1Clock, LP2Clock, RP2Clock, RP1Clock);
	glEnd();

}

void VRP_Back_Interface(int location, Shader phong_sh, float light_pos_v[4], light_phong I)
{
	int width = 38;
	int height = 11;

	Point O = { -18, 0, 3 + height / 2.0 };

	Point LP1Int = { O.x + 0.3, O.y + width / 2.0, O.z - height / 2.0 };
	Point LP2Int = { LP1Int.x, LP1Int.y, LP1Int.z + height };
	Point RP1Int = { LP1Int.x, LP1Int.y - width, LP1Int.z };
	Point RP2Int = { LP2Int.x, LP2Int.y - width, LP2Int.z };

	light_phong m(20, 20, 20, 28, 28, 28, 0, 0, 0);	//свет материала
	PhongColor(location, phong_sh, light_pos_v, I, m, 0);
	ColorRGB(34, 34, 34);
	glBegin(GL_QUADS);

	draw_quad(LP1Int, LP2Int, RP2Int, RP1Int);

	glEnd();
}

void Vinyl(Point Center, double Radius, double RadiusIn)
{
	Point CenterShifted = { Center.x + 0.1, Center.y, Center.z };

	int dis = 1;

	for (int t = 0; t <= 360; t += dis)
	{
		//для самой пластинки
		double yBig = Radius * cos(t * M_PI / 180) + Center.y;
		double zBig = Radius * sin(t * M_PI / 180) + Center.z;

		Point TBig = { Center.x, yBig, zBig };
		Point TUBig = { CenterShifted.x, yBig, zBig };

		double y1Big = Radius * cos((t + dis) * M_PI / 180) + Center.y;
		double z1Big = Radius * sin((t + dis) * M_PI / 180) + Center.z;

		Point T1Big = { Center.x, y1Big, z1Big };
		Point T1UBig = { CenterShifted.x, y1Big, z1Big };


		//внутреннее отверстие
		double ySmall = RadiusIn * cos(t * M_PI / 180) + Center.y;
		double zSmall = RadiusIn * sin(t * M_PI / 180) + Center.z;

		Point TSmall = { Center.x, ySmall, zSmall };
		Point TUSmall = { CenterShifted.x, ySmall, zSmall };

		double y1Small = RadiusIn * cos((t + dis) * M_PI / 180) + Center.y;
		double z1Small = RadiusIn * sin((t + dis) * M_PI / 180) + Center.z;

		Point T1Small = { Center.x, y1Small, z1Small };
		Point T1USmall = { CenterShifted.x, y1Small, z1Small };

		ColorRGB(255, 255, 255);
		glBegin(GL_QUADS);

		draw_quad(TSmall, TBig, T1Big, T1Small);
		draw_quad(T1USmall, T1UBig, TUBig, TUSmall);


		draw_quad(TSmall, T1Small, T1USmall, TUSmall);
		draw_quad(TUBig, T1UBig, T1Big, TBig);

		glEnd();
	}
}

void Cover(Point Center, bool flag)
{
	double cover_width = 29;

	int coefficient = 0;

	flag ? coefficient = -1 : coefficient = 1;

	Point PUL = { Center.x + 0.12 * coefficient, Center.y - cover_width / 2.0, Center.z + cover_width / 2.0 };
	Point PDL = { PUL.x, Center.y - cover_width / 2.0, Center.z - cover_width / 2.0 };
	Point PUR = { PUL.x, Center.y + cover_width / 2.0, Center.z + cover_width / 2.0 };
	Point PDR = { PUL.x, Center.y + cover_width / 2.0, Center.z - cover_width / 2.0 };


	ColorRGB(255, 255, 255);

	glBegin(GL_QUADS);
	//Нормаль
	Point N;
	!flag ? N = calculate_normal(PDL, PUL, PUR) : N = calculate_normal(PUL, PDL, PUR);
	glNormal3dv((double*)&N);

	glTexCoord2d(flag, 1);
	glVertex3dv((double*)&PUL);

	glTexCoord2d(flag, 0);
	glVertex3dv((double*)&PDL);

	glTexCoord2d(!flag, 0);
	glVertex3dv((double*)&PDR);

	glTexCoord2d(!flag, 1);
	glVertex3dv((double*)&PUR);

	//Соединение передней и задней стороны
	if(!flag)
	{
		Point BUL = { Center.x + 0.12 * -coefficient, Center.y - cover_width / 2.0, Center.z + cover_width / 2.0 };
		Point BDL = { BUL.x, Center.y - cover_width / 2.0, Center.z - cover_width / 2.0 };
		Point BUR = { BUL.x, Center.y + cover_width / 2.0, Center.z + cover_width / 2.0 };
		Point BDR = { BUL.x, Center.y + cover_width / 2.0, Center.z - cover_width / 2.0 };

		draw_quad(BUR, PUR, PUL, BUL);
		draw_quad(BUL, PUL, PDL, BDL);
		draw_quad(BDL, PDL, PDR, BDR);
	}

	glEnd();

}

