#ifndef C_XAUDIO2_H
#define C_XAUDIO2_H

#include "XAudio2Vector.h"
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <iostream>
#include <fstream>
#include <sstream>
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"xaudio2.lib")

//--------------------------
// セーフ開放マクロ.

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=nullptr; } }
#endif  
#ifndef SAVE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=nullptr; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=nullptr; } }
#endif

namespace XAudio2File
{
	// バイナリデータを読み込む.
	template <class In>
	static HRESULT LoadBinary(const std::string Path, In& InData)
	{
		std::ifstream fin(Path.c_str(), std::ios::in | std::ios::binary);

		if (!fin) {
			return E_FAIL;
		}
		// ファイルを読み込む.
		fin.read(reinterpret_cast<char*>(std::addressof(InData)), sizeof(In));

		fin.close();  //ファイルを閉じる
		return S_OK;
	};
	// バイナリデータを作成する.
	template <class Out>
	static HRESULT CreateBinary(const std::string Path, Out OutData)
	{
		std::ofstream fout;

		fout.open(Path.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);

		if (!fout) {
			return E_FAIL;
		}

		fout.write(reinterpret_cast<char*>(std::addressof(OutData)), sizeof(Out));
		fout.close();  //ファイルを閉じる

		return S_OK;
	};
}

namespace XAudio2Reverb
{
	enum class enReverbPreset
	{
		en_PRESET_FOREST,
		en_PRESET_DEFAULT,
		en_PRESET_GENERIC,
		en_PRESET_PADDEDCELL,
		en_PRESET_ROOM,
		en_PRESET_BATHROOM,
		en_PRESET_LIVINGROOM,
		en_PRESET_STONEROOM,
		en_PRESET_AUDITORIUM,
		en_PRESET_CONCERTHALL,
		en_PRESET_CAVE,
		en_PRESET_ARENA,
		en_PRESET_HANGAR,
		en_PRESET_CARPETEDHALLWAY,
		en_PRESET_HALLWAY,
		en_PRESET_STONECORRIDOR,
		en_PRESET_ALLEY,
		en_PRESET_CITY,
		en_PRESET_MOUNTAINS,
		en_PRESET_QUARRY,
		en_PRESET_PLAIN,
		en_PRESET_PARKINGLOT,
		en_PRESET_SEWERPIPE,
		en_PRESET_UNDERWATER,
		en_PRESET_SMALLROOM,
		en_PRESET_MEDIUMROOM,
		en_PRESET_LARGEROOM,
		en_PRESET_MEDIUMHALL,
		en_PRESET_LARGEHALL,
		en_PRESET_PLATE,

		Max,
	};

	const static int NUM_PRESETS = 30;
	static XAUDIO2FX_REVERB_I3DL2_PARAMETERS PRESET_PARAMS[NUM_PRESETS] =
	{
		XAUDIO2FX_I3DL2_PRESET_FOREST,
		XAUDIO2FX_I3DL2_PRESET_DEFAULT,
		XAUDIO2FX_I3DL2_PRESET_GENERIC,
		XAUDIO2FX_I3DL2_PRESET_PADDEDCELL,
		XAUDIO2FX_I3DL2_PRESET_ROOM,
		XAUDIO2FX_I3DL2_PRESET_BATHROOM,
		XAUDIO2FX_I3DL2_PRESET_LIVINGROOM,
		XAUDIO2FX_I3DL2_PRESET_STONEROOM,
		XAUDIO2FX_I3DL2_PRESET_AUDITORIUM,
		XAUDIO2FX_I3DL2_PRESET_CONCERTHALL,
		XAUDIO2FX_I3DL2_PRESET_CAVE,
		XAUDIO2FX_I3DL2_PRESET_ARENA,
		XAUDIO2FX_I3DL2_PRESET_HANGAR,
		XAUDIO2FX_I3DL2_PRESET_CARPETEDHALLWAY,
		XAUDIO2FX_I3DL2_PRESET_HALLWAY,
		XAUDIO2FX_I3DL2_PRESET_STONECORRIDOR,
		XAUDIO2FX_I3DL2_PRESET_ALLEY,
		XAUDIO2FX_I3DL2_PRESET_CITY,
		XAUDIO2FX_I3DL2_PRESET_MOUNTAINS,
		XAUDIO2FX_I3DL2_PRESET_QUARRY,
		XAUDIO2FX_I3DL2_PRESET_PLAIN,
		XAUDIO2FX_I3DL2_PRESET_PARKINGLOT,
		XAUDIO2FX_I3DL2_PRESET_SEWERPIPE,
		XAUDIO2FX_I3DL2_PRESET_UNDERWATER,
		XAUDIO2FX_I3DL2_PRESET_SMALLROOM,
		XAUDIO2FX_I3DL2_PRESET_MEDIUMROOM,
		XAUDIO2FX_I3DL2_PRESET_LARGEROOM,
		XAUDIO2FX_I3DL2_PRESET_MEDIUMHALL,
		XAUDIO2FX_I3DL2_PRESET_LARGEHALL,
		XAUDIO2FX_I3DL2_PRESET_PLATE,
	};



}

#endif//#ifndef C_XAUDIO2_H.