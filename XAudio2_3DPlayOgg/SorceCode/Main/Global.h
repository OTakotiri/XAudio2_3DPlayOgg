#ifndef GLOBAL_H
#define GLOBAL_H

// �x���ɂ��ẴR�[�h���͂𖳌��ɂ��� 4005:�Ē�`.
#pragma warning( disable:4005 )

#include <Windows.h>
#include <memory>
#include <mmsystem.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "MyMacro.h"


//=================================================
//	�萔.
//=================================================
constexpr int WND_W = 1280;				// ����޳�̕�.
constexpr int WND_H = 720;				// ����޳�̍���.
constexpr float GAME_FPS = 60.0f;
#endif	// #ifndef GLOBAL_H.