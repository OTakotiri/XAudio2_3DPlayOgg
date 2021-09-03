#ifndef MESH_RESORCE_H
#define MESH_RESORCE_H

//警告についてのｺｰﾄﾞ分析を無効にする. 4005:再定義.
#pragma warning( disable:4005)

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <thread>
#include <filesystem>	// C++17 必須.

#include <D3DX11.h>
#include <D3D11.h>
#include <d3dx9.h>

#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )

#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"

class CMeshResource
{
	const char* FILE_PATH = "Data\\Model";

	typedef std::unordered_map<std::string, std::shared_ptr<CDX9StaticMesh>>		static_mesh_map;
public:
	CMeshResource();
	~CMeshResource();

	// インスタンスの取得.
	static CMeshResource* GetInstance();

	// モデルの読み込み.
	static void Load( 
		HWND hWnd, 
		ID3D11Device* pDevice11, 
		ID3D11DeviceContext* pContext11,
		LPDIRECT3DDEVICE9 pDevice9 );
	// スレッドの解放.
	static bool ThreadRelease();

	// スタティックメッシュの読み込み.
	static std::shared_ptr<CDX9StaticMesh> GetStatic( const std::string& name );
	static bool GetStatic( std::shared_ptr<CDX9StaticMesh>& model, const std::string& name );

private:
	// モデルの読み込み.
	void ModelLoad( 
		HWND hWnd, 
		ID3D11Device* pDevice11, 
		ID3D11DeviceContext* pContext11,
		LPDIRECT3DDEVICE9 pDevice9 );

private:
	static_mesh_map m_staticMeshList;	// スタティックメッシュのリスト.

	std::thread		m_thread;	// スレッド.

	bool	m_isLoadEnd;		// ロードが終了したか.
	bool	m_isThreadJoined;	// スレッドが解放されたか.

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CMeshResource( const CMeshResource & )				= delete;
	CMeshResource& operator = ( const CMeshResource & )	= delete;
	CMeshResource(CMeshResource && )						= delete;
	CMeshResource& operator = (CMeshResource && )		= delete;
};

#endif	// #ifndef MESH_RESORCE_H.