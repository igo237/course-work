#pragma once
//Рисуем крутилку (перенести в VRP_DRAW)
void draw_twister(Point Center, double RadiusBig, double RadiusSmall, double LengthBig, double LengthSmall, int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//Рисуем кнопку
void draw_button(Point BUL, double ButtonWidth, double Radius, int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//Жёрдочка по середине переднего интерфейса
void Front_Interface_Perch(Point O, int width, double perch_height);

//Кнопки и крутилки у переднего интерфейса
void Front_Interface_Buttons_and_Twisters(Point LP1Int, Point LP2Int, Point RP1Int, Point RP2Int);

//Основа проигрывателя
void VRP_Base(int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//Блин проигрывателя
void VRP_Platter(int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//Ножки проигрывателя
void VRP_Legs(int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//Шпиндель проигрывателя
void VRP_Spindel(int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//Основа рычага проигрывателя
void VRP_Lever_Base(int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//Игла на рычаге
void VRP_Lever_Needle(int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//Крышка проигрывателя
void VRP_Cover(int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//Передний интерфейс проигрывателя
void VRP_Front_Interface(int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//Задний интерфейс проигрывателя
void VRP_Back_Interface(int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//Винил
void Vinyl(Point Center, double Radius, double RadiusIn);

//Обложка
void Cover(Point Center, bool flag);

//Проигрыватель
//void VRP();