#pragma once
//������ �������� (��������� � VRP_DRAW)
void draw_twister(Point Center, double RadiusBig, double RadiusSmall, double LengthBig, double LengthSmall, int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//������ ������
void draw_button(Point BUL, double ButtonWidth, double Radius, int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//Ƹ������ �� �������� ��������� ����������
void Front_Interface_Perch(Point O, int width, double perch_height);

//������ � �������� � ��������� ����������
void Front_Interface_Buttons_and_Twisters(Point LP1Int, Point LP2Int, Point RP1Int, Point RP2Int);

//������ �������������
void VRP_Base(int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//���� �������������
void VRP_Platter(int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//����� �������������
void VRP_Legs(int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//�������� �������������
void VRP_Spindel(int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//������ ������ �������������
void VRP_Lever_Base(int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//���� �� ������
void VRP_Lever_Needle(int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//������ �������������
void VRP_Cover(int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//�������� ��������� �������������
void VRP_Front_Interface(int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//������ ��������� �������������
void VRP_Back_Interface(int location, Shader phong_sh, float light_pos_v[4], light_phong I);

//�����
void Vinyl(Point Center, double Radius, double RadiusIn);

//�������
void Cover(Point Center, bool flag);

//�������������
//void VRP();