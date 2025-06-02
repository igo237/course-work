#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#include "Music.h"
#pragma comment(lib, "winmm.lib") 

void PlaySomeGoodMusic(std::wstring music_name)
{
    PlaySound((LPCTSTR) music_name.c_str(), NULL, SND_FILENAME | SND_ASYNC);
}

int MusicLength(std::wstring music_name)
{
    std::wstring command = L"open \"" + music_name + L"\" type waveaudio alias music";

    mciSendString(command.c_str(), NULL, 0, NULL);

    wchar_t buffer[128];
    mciSendString(L"status music length", buffer, sizeof(buffer), NULL);

    int time = _wtoi(buffer) / 1000.0;

    mciSendString(L"close music", NULL, 0, NULL);

    return time;
}