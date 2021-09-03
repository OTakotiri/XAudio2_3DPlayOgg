#include "MeshResource.h"
#include <sstream>
#include <stdio.h>

namespace fs = std::filesystem;

CMeshResource::CMeshResource()
	: m_staticMeshList	()
	, m_isLoadEnd		( false	)
	, m_isThreadJoined	( false )
{
}

CMeshResource::~CMeshResource()
{
	m_staticMeshList.clear();
}

CMeshResource* CMeshResource::GetInstance()
{
	static std::unique_ptr<CMeshResource> pInstance =
		std::make_unique<CMeshResource>();	// インスタンスの作成.
	return pInstance.get();
}

void CMeshResource::Load(
	HWND hWnd,
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	LPDIRECT3DDEVICE9 pDevice9 )
{
	auto modelLoad = [&]( HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11, LPDIRECT3DDEVICE9 pDevice9 )
	{
		GetInstance()->ModelLoad( hWnd, pDevice11, pContext11, pDevice9 );
		GetInstance()->m_isLoadEnd = true;
	};


	GetInstance()->m_thread = std::thread( modelLoad, hWnd, pDevice11, pContext11, pDevice9 );
}

bool CMeshResource::ThreadRelease()
{
	if( GetInstance()->m_isThreadJoined == true ) return true;
	if( GetInstance()->m_isLoadEnd == false ) return false;
	GetInstance()->m_thread.join();
	GetInstance()->m_isThreadJoined = true;
	return true;
}

std::shared_ptr<CDX9StaticMesh> CMeshResource::GetStatic( const std::string& name )
{
	for( auto& m : GetInstance()->m_staticMeshList ){
		// m_staticMeshList は map.
		// firstでmapの一つ目の要素 stringを出し.
		// secondでmapの二つ目の要素 shared_ptr<CDX9Mesh>を返す.
		if( m.first == name ) return m.second;
	}

	if( GetInstance()->m_isLoadEnd == false ) return nullptr;
	_ASSERT_EXPR( false, L"スタティックメッシュが見つかりません" );
	MessageBox( nullptr, name.c_str(), "スタティックメッシュが見つかりません", MB_OK );
	return nullptr;
}

bool CMeshResource::GetStatic( std::shared_ptr<CDX9StaticMesh>& model, const std::string& name )
{
	model = GetStatic( name );
	if( model == nullptr ) return false; 
	return true;
}

void CMeshResource::ModelLoad(
	HWND hWnd,
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	LPDIRECT3DDEVICE9 pDevice9 )
{
	auto eachLoad = [&]( const fs::directory_entry& entry )
	{
		const std::string exe		= entry.path().extension().string();	// 拡張子.
		const std::string filePath	= entry.path().string();				// ファイルパス.
		const std::string fileName	= entry.path().stem().string();			// ファイル名.

		if( exe != ".x" && exe != ".X" ) return;	// 拡張子が "Xファイル" じゃなければ終了.

		m_staticMeshList[fileName] = 
			std::make_shared<CDX9StaticMesh>( hWnd, pDevice11, pContext11, pDevice9, filePath.c_str() );
	};

	try {
		fs::recursive_directory_iterator dir_itr( FILE_PATH ), end_itr;
		std::for_each( dir_itr, end_itr, eachLoad );

	} catch( const fs::filesystem_error& e ){
		const char* errorMessage = e.path1().string().c_str();
		_ASSERT_EXPR( false, L"メッシュファイル読込失敗" );
		MessageBox( nullptr, errorMessage, "メッシュファイル読込失敗", MB_OK );
	}
}