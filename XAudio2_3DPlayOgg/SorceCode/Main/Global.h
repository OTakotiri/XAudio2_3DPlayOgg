#ifndef GLOBAL_H
#define GLOBAL_H

// 警告についてのコード分析を無効にする 4005:再定義.
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
//	定数.
//=================================================
constexpr int WND_W = 1280;				// ｳｨﾝﾄﾞｳの幅.
constexpr int WND_H = 720;				// ｳｨﾝﾄﾞｳの高さ.
constexpr float GAME_FPS = 60.0f;
#endif	// #ifndef GLOBAL_H.