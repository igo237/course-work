#define _USE_MATH_DEFINES
#include "Render.h"
#include <Windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "GUItextRectangle.h"
#include "MyShaders.h"
#include "Texture.h"

#include "Point.h"
#include "MyFunctions.h"
#include "VRP_Builder.h"

#include "ObjLoader.h"

#include "debout.h"

//Звук
#include "Music.h"


//внутренняя логика "движка"
#include "MyOGL.h"
extern OpenGL gl;
#include "Light.h"
Light light;
#include "Camera.h"
Camera camera;


bool texturing = true;
bool lightning = false;
bool alpha = true;

bool play = false;
bool open = false;
bool vc_sh = false;

std::wstring Track = L"";
int CoverTex;
int Length = 1;
int count = 0;

bool music_play = false;

bool opened = false;

//переключение режимов освещения, текстурирования, альфаналожения
void switchModes(OpenGL *sender, KeyEventArg arg)
{
	//конвертируем код клавиши в букву
	auto key = LOWORD(MapVirtualKeyA(arg.key, MAPVK_VK_TO_CHAR));

	std::wstring path = L"music/";

	if (!vc_sh and !music_play)
	{
		switch (key)
		{
		case '1':
			Track = path + L"[Night].wav";
			CoverTex = 1;
			Length = MusicLength(Track);
			count = 0;
			break;
		case '2':
			Track = path + L"SongOfHunts.wav";
			CoverTex = 2;
			Length = MusicLength(Track);
			count = 0;
			break;
		case '3':
			Track = path + L"World.wav";
			CoverTex = 3;
			Length = MusicLength(Track);
			count = 0;
			break;
		case '4':
			Track = path + L"Siluan.wav";
			CoverTex = 4;
			Length = MusicLength(Track);
			count = 0;
			break;
		case '5':
			Track = path + L"Synth.wav";
			CoverTex = 5;
			Length = MusicLength(Track);
			count = 0;
			break;
		}
	}



	switch (key)
	{
	case 'L':
		lightning = !lightning;
		break;
	case 'T':
		texturing = !texturing;
		break;
	case 'A':
		alpha = !alpha;
		break;
	case 'P':
		if(opened) vc_sh = !vc_sh;
		break;
	case 'O':
		open = !open;
		break;
	}

}


//умножение матриц c[M1][N1] = a[M1][N1] * b[M2][N2]
template<typename T, int M1, int N1, int M2, int N2>
void MatrixMultiply(const T* a, const T* b, T* c)
{
	for (int i = 0; i < M1; ++i)
	{
		for (int j = 0; j < N2; ++j)
		{
			c[i * N2 + j] = T(0);
			for (int k = 0; k < N1; ++k)
			{
				c[i * N2 + j] += a[i * N1 + k] * b[k * N2 + j];
			}
		}
	}
}

//Текстовый прямоугольничек в верхнем правом углу.
//OGL не предоставляет возможности для хранения текста
//внутри этого класса создается картинка с текстом (через виндовый GDI),
//в виде текстуры накладывается на прямоугольник и рисуется на экране.
//Это самый простой способ что то написать на экране
//но ооооочень не оптимальный
GuiTextRectangle text;

//айдишник для текстуры
GLuint texId;
//выполняется один раз перед первым рендером

ObjModel f;


Shader cassini_sh;
Shader phong_sh;
Shader vb_sh;
Shader simple_texture_sh;

Texture texture;

Texture FrontCovers[5] = { texture, texture, texture, texture, texture };
Texture BackCovers[5] = { texture, texture, texture, texture, texture };
Texture VinylTexture[5] = { texture, texture, texture, texture, texture };


void initRender()
{

	cassini_sh.VshaderFileName = "shaders/v.vert";
	cassini_sh.FshaderFileName = "shaders/cassini.frag";
	cassini_sh.LoadShaderFromFile();
	cassini_sh.Compile();

	phong_sh.VshaderFileName = "shaders/v.vert";
	phong_sh.FshaderFileName = "shaders/light.frag";
	phong_sh.LoadShaderFromFile();
	phong_sh.Compile();

	vb_sh.VshaderFileName = "shaders/v.vert";
	vb_sh.FshaderFileName = "shaders/vb.frag";
	vb_sh.LoadShaderFromFile();
	vb_sh.Compile();

	simple_texture_sh.VshaderFileName = "shaders/v.vert";
	simple_texture_sh.FshaderFileName = "shaders/textureShader.frag";
	simple_texture_sh.LoadShaderFromFile();
	simple_texture_sh.Compile();

	//ЗАгружаем текстуру винила
	VinylTexture[0].LoadTexture("textures/vinyl/[Night]_vinyl.jpg");
	VinylTexture[1].LoadTexture("textures/vinyl/SongOfHunts_vinyl.jpg");
	VinylTexture[2].LoadTexture("textures/vinyl/World_vinyl.jpg");
	VinylTexture[3].LoadTexture("textures/vinyl/Siluan_vinyl.jpg");
	VinylTexture[4].LoadTexture("textures/vinyl/Synth_vinyl.jpg");


	//Загружаем переднюю сторону обложки
	FrontCovers[0].LoadTexture("textures/front_covers/[Night]_cover.jpg");
	FrontCovers[1].LoadTexture("textures/front_covers/SongOfHunts_cover.jpg");
	FrontCovers[2].LoadTexture("textures/front_covers/World_cover.jpg");
	FrontCovers[3].LoadTexture("textures/front_covers/Siluan_cover.jpg");
	FrontCovers[4].LoadTexture("textures/front_covers/Synth_cover.jpg");

	//Загружаем заднюю сторону обложки
	BackCovers[0].LoadTexture("textures/back_covers/[Night]_back.jpg");
	BackCovers[1].LoadTexture("textures/back_covers/SongOfHunts_back.jpg");
	BackCovers[2].LoadTexture("textures/back_covers/World_back.jpg");
	BackCovers[3].LoadTexture("textures/back_covers/Siluan_back.jpg");
	BackCovers[4].LoadTexture("textures/back_covers/Synth_back.jpg");

	//==============НАСТРОЙКА ТЕКСТУР================
	//4 байта на хранение пикселя
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	

	//================НАСТРОЙКА КАМЕРЫ======================
	camera.caclulateCameraPos();

	//привязываем камеру к событиям "движка"
	gl.WheelEvent.reaction(&camera, &Camera::Zoom);
	gl.MouseMovieEvent.reaction(&camera, &Camera::MouseMovie);
	gl.MouseLeaveEvent.reaction(&camera, &Camera::MouseLeave);
	gl.MouseLdownEvent.reaction(&camera, &Camera::MouseStartDrag);
	gl.MouseLupEvent.reaction(&camera, &Camera::MouseStopDrag);
	//==============НАСТРОЙКА СВЕТА===========================
	//привязываем свет к событиям "движка"
	gl.MouseMovieEvent.reaction(&light, &Light::MoveLight);
	gl.KeyDownEvent.reaction(&light, &Light::StartDrug);
	gl.KeyUpEvent.reaction(&light, &Light::StopDrug);
	//========================================================
	//====================Прочее==============================
	gl.KeyDownEvent.reaction(switchModes);

	text.setSize(512, 180);
	//========================================================
	
	


	camera.setPosition(2, 1.5, 1.5);
	
}




float view_matrix[16];
double full_time = 0;
int location = 0;

double lever_shift_y = 0;
double lever_shift_x = 0;
double cover_open = 0;
double vinyl_rotated = 0;
double vinyl_speed = 0;
double v_angle = 0;

double vc_shift = 0;
double vc_shift_next = 0;

double v_shift = 0;
double v_shift_next = 0;

double speed_coefficient = 4;

double vc_speed_coefficient = 1;

bool shifted = false;
bool music_ended = false;
bool lever_is_back = false;

void ChangeCover(int CoverTex, bool b)
{
	FrontCovers[CoverTex - 1].Bind();
	if (b) BackCovers[CoverTex - 1].Bind();
}

void ChangeVinyl(int Tex)
{
	VinylTexture[Tex - 1].Bind();
}

Animation cover_animation(0, -45);

bool angle_f = false;

double angle = 0;

void Render(double delta_time)
{    
	

	full_time += delta_time;
	
	//натройка камеры и света
	//в этих функциях находятся OGLные функции
	//которые устанавливают параметры источника света
	//и моделвью матрицу, связанные с камерой.

	if (gl.isKeyPressed('F')) //если нажата F - свет из камеры
	{
		light.SetPosition(camera.x(), camera.y(), camera.z());
	}
	camera.SetUpCamera();
	//забираем моделвью матрицу сразу после установки камера
	//так как в ней отсуствуют трансформации glRotate...
	//она, фактически, является видовой.
	glGetFloatv(GL_MODELVIEW_MATRIX,view_matrix);

	

	light.SetUpLight();

	//рисуем оси
	gl.DrawAxes();

	

	glBindTexture(GL_TEXTURE_2D, 0);

	//включаем нормализацию нормалей
	//чтобв glScaled не влияли на них.

	glEnable(GL_NORMALIZE);  
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	
	//включаем режимы, в зависимости от нажания клавиш. см void switchModes(OpenGL *sender, KeyEventArg arg)
	if (lightning)
		glEnable(GL_LIGHTING);
	if (texturing)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0); //сбрасываем текущую текстуру
	}
		
	if (alpha)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
		
	//=============НАСТРОЙКА МАТЕРИАЛА==============


	//настройка материала, все что рисуется ниже будет иметь этот метериал.
	//массивы с настройками материала
	float  amb[] = { 0.2, 0.2, 0.1, 1. };
	float dif[] = { 0.4, 0.65, 0.5, 1. };
	float spec[] = { 0.9, 0.8, 0.3, 1. };
	float sh = 0.2f * 256;

	//фоновая
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//дифузная
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//зеркальная
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec); 
	//размер блика
	glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//чтоб было красиво, без квадратиков (сглаживание освещения)
	glShadeModel(GL_SMOOTH); //закраска по Гуро      
			   //(GL_SMOOTH - плоская закраска)

	//============ РИСОВАТЬ ТУТ ==============

	double lever_speed_y = 0;
	double lever_speed_x = 0;

	double cover_speed = 0;



	double vc_speed = 0;

	Point COV_CUR;
	Point COV_NEXT;

	Point VIN_CUR;
	Point VIN_NEXT;

	//СВЕТ
	light_phong I ( 255, 255, 255,							//фоновый
					255, 255, 255,							//рассеянный
					255, 255, 255 );						//бликовый


	

	float light_pos[4] = { light.x(),light.y(), light.z(), 1 };
	float light_pos_v[4];


	if(lightning)
	{
		phong_sh.UseShader();
		//переносим координаты света в видовые координаты
		MatrixMultiply<float, 1, 4, 4, 4>(light_pos, view_matrix, light_pos_v);
	}
	

	if (Track != L"" and music_play and count == 0)
	{
		PlaySomeGoodMusic(Track);
		count++;
	}

	VRP_Base(location, phong_sh, light_pos_v, I);
	VRP_Legs(location, phong_sh, light_pos_v, I);
	VRP_Spindel(location, phong_sh, light_pos_v, I);
	VRP_Lever_Base(location, phong_sh, light_pos_v, I);
	VRP_Back_Interface(location, phong_sh, light_pos_v, I);
	VRP_Front_Interface(location, phong_sh, light_pos_v, I);



	//Рисуется обложка и винил
	if(opened and !open)
	{
		Point Center = { 0, 0, 0 };

		Point Center1 = { 40, 0, 20 };

		double Radius = 14;
		double RadiusIn = 0.5;

		//кол-во точек
		int samples = 50;
		//шаг между точками
		double step = 1. / (samples - 1);

		glPushMatrix();

		Point Q = { 40, 0, 20 };
		Point C = { 40, 50, 20 };
		Point V = { 0, 0, 16.21 };
		Point S = { 10, 50, 40 };
		
		vc_speed = 10 * delta_time * vc_sh * step * vc_speed_coefficient;
		
		v_shift += vc_speed;
		v_shift_next = v_shift + vc_speed;

		VIN_CUR = BezierCurve3(Q, C, S, V, v_shift);
		VIN_NEXT = BezierCurve3(Q, C, S, V, v_shift_next);

		if (lever_is_back)
		{
			vc_speed_coefficient = -1;
		}

		if (!lever_is_back)
		{
			vc_speed_coefficient = 1;
		}

		if (VIN_CUR.x < 30 and VIN_CUR.y > 20) v_angle -= 45 * delta_time * vc_sh * vc_speed_coefficient;

		if (v_angle < -90) v_angle = -90;
		if (v_angle > 0) v_angle = 0;

		if (VIN_CUR.x <= V.x + 0.1 and VIN_CUR.y < V.y and !lever_is_back)
		{
			VIN_CUR = {V.x, V.y, V.z};
			vc_sh = false;
			angle_f = true;
			play = true;
		}


		if (VIN_CUR.x > Q.x - 0.1 and VIN_CUR.y <= Q.y and lever_is_back)
		{
			VIN_CUR = { Q.x, Q.y, Q.z };
			vc_sh = false;
			angle_f = false;
			lever_is_back = false;
			play = false;
		}

		vinyl_speed -= 360 * 0.55 * music_play * delta_time;

		glTranslated(VIN_CUR.x, VIN_CUR.y, VIN_CUR.z);
		glRotated(v_angle, 0, 1, 0);
		glRotated(vinyl_speed, 1, 0, 0);

		//Винил
		glActiveTexture(GL_TEXTURE0);
		ChangeVinyl(CoverTex);
		Vinyl(Center, Radius, RadiusIn);
		glBindTexture(GL_TEXTURE_2D, 0);

		glPopMatrix();

		glPushMatrix();

		Point D = { 40, 0, 20 };
		Point E = { 40, -63, 20 };
		Point G = { 7, -33, 15 };
		Point F = { 46, -10, 20 };

		vc_shift += vc_speed;
		vc_shift_next = vc_shift + vc_speed + angle_f / 100.0;

		COV_CUR = BezierCurve3(D, E, F, G, vc_shift);
		COV_NEXT = BezierCurve3(D, E, F, G, vc_shift_next);

		angle = atan2f(COV_CUR.x - COV_NEXT.x, COV_CUR.y - COV_NEXT.y) * 180 / M_PI;

		glTranslated(COV_CUR.x, COV_CUR.y, COV_CUR.z);
		glRotated(-angle, 0, 0, 1);

		//Обложка
		glActiveTexture(GL_TEXTURE0);
		ChangeCover(CoverTex, false);
		Cover(Center, false);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE0);
		ChangeCover(CoverTex, true);
		Cover(Center, true);
		glBindTexture(GL_TEXTURE_2D, 0);

		glPopMatrix();
	}



	//Анимация поворота иглы
	glPushMatrix();
	
	glTranslated(-11, 15, 16.425);

	


	if (!music_ended and lever_shift_y < -5)
	{
		play = false;
		shifted = true;
	}

	if(!music_ended and lever_shift_x > 1.5)
	{
		shifted = false;
		speed_coefficient = 23.0 / Length;
		play = true;
		music_play = true;
	}
	if(music_ended and lever_shift_x < 0)
	{
		shifted = false;
		play = true;
	}
	if(music_ended and lever_shift_y > 0)
	{
		play = false;
		music_ended = false;
		speed_coefficient = 6;
		lever_shift_y = 0;
		vc_sh = true;
		lever_is_back = true;
		count = 0;
	}
	if (!music_ended and lever_shift_y < -28)
	{
		play = false;
		music_ended = true;
		music_play = false;
		speed_coefficient = -5;
		shifted = true;
	}

	lever_speed_y = speed_coefficient * play;
	lever_speed_x = speed_coefficient * shifted;

	glRotated(lever_shift_y -= lever_speed_y * delta_time, 0, 0, 1);
	glRotated(lever_shift_x += lever_speed_x * delta_time, 0, 1, 0);

	VRP_Lever_Needle(location, phong_sh, light_pos_v, I);


	glPopMatrix();

	//Винил
	glPushMatrix();

	glRotated(vinyl_speed, 0, 0, 1);

	VRP_Platter(location, phong_sh, light_pos_v, I);

	glDisable(GL_LIGHTING);

	glPopMatrix();

	//Поднятие крышки
	glPushMatrix();

	cover_speed = -20 * delta_time * open;

	cover_animation.change_speed(cover_speed);

	glTranslated(-18, 0, 15);

	if(open)
	{
		cover_animation.start();
		opened = true;
		open = cover_animation.forward();
	}

	glRotated(cover_animation.cur_value(), 0, 1, 0);

	VRP_Cover(location, phong_sh, light_pos_v, I);


	glPopMatrix();

	glEnable(GL_LIGHTING);

	//===============================================
	
	
	//сбрасываем все трансформации
	glLoadIdentity();
	camera.SetUpCamera();	
	Shader::DontUseShaders();
	//рисуем источник света
	light.DrawLightGizmo();

	//================Сообщение в верхнем левом углу=======================
	glActiveTexture(GL_TEXTURE0);
	//переключаемся на матрицу проекции
	glMatrixMode(GL_PROJECTION);
	//сохраняем текущую матрицу проекции с перспективным преобразованием
	glPushMatrix();
	//загружаем единичную матрицу в матрицу проекции
	glLoadIdentity();

	//устанавливаем матрицу паралельной проекции
	glOrtho(0, gl.getWidth() - 1, 0, gl.getHeight() - 1, 0, 1);

	//переключаемся на моделвью матрицу
	glMatrixMode(GL_MODELVIEW);
	//сохраняем матрицу
	glPushMatrix();
    //сбразываем все трансформации и настройки камеры загрузкой единичной матрицы
	glLoadIdentity();

	//отрисованное тут будет визуалзироватся в 2д системе координат
	//нижний левый угол окна - точка (0,0)
	//верхний правый угол (ширина_окна - 1, высота_окна - 1)

	
	std::wstringstream ss;
	ss << std::fixed << std::setprecision(3);
	ss << "T - " << (texturing ? L"[вкл]выкл  " : L" вкл[выкл] ") << L"текстур" << std::endl;
	ss << "L - " << (lightning ? L"[вкл]выкл  " : L" вкл[выкл] ") << L"освещение" << std::endl;
	ss << "A - " << (alpha ? L"[вкл]выкл  " : L" вкл[выкл] ") << L"альфа-наложение" << std::endl;
	ss << L"F - Свет из камеры" << std::endl;
	ss << L"G - двигать свет по горизонтали" << std::endl;
	ss << L"G+ЛКМ двигать свет по вертекали" << std::endl;
	ss << L"Коорд. света: (" << std::setw(7) <<  light.x() << "," << std::setw(7) << light.y() << "," << std::setw(7) << light.z() << ")" << std::endl;
	ss << L"Коорд. камеры: (" << std::setw(7) << camera.x() << "," << std::setw(7) << camera.y() << "," << std::setw(7) << camera.z() << ")" << std::endl;
	ss << L"Параметры камеры: R=" << std::setw(7) << camera.distance() << ",fi1=" << std::setw(7) << camera.fi1() << ",fi2=" << std::setw(7) << camera.fi2() << std::endl;
	ss << L"delta_time: " << std::setprecision(5)<< delta_time << std::endl;
	ss << L"full_time: " << std::setprecision(2) << full_time << std::endl;

	
	text.setPosition(10, gl.getHeight() - 10 - 180);
	text.setText(ss.str().c_str());
	
	text.Draw();

	//восстанавливаем матрицу проекции на перспективу, которую сохраняли ранее.
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	
}   



