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
		std::make_unique<CMeshResource>();	// �C���X�^���X�̍쐬.
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
		// m_staticMeshList �� map.
		// first��map�̈�ڂ̗v�f string���o��.
		// second��map�̓�ڂ̗v�f shared_ptr<CDX9Mesh>��Ԃ�.
		if( m.first == name ) return m.second;
	}

	if( GetInstance()->m_isLoadEnd == false ) return nullptr;
	_ASSERT_EXPR( false, L"�X�^�e�B�b�N���b�V����������܂���" );
	MessageBox( nullptr, name.c_str(), "�X�^�e�B�b�N���b�V����������܂���", MB_OK );
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
		const std::string exe		= entry.path().extension().string();	// �g���q.
		const std::string filePath	= entry.path().string();				// �t�@�C���p�X.
		const std::string fileName	= entry.path().stem().string();			// �t�@�C����.

		if( exe != ".x" && exe != ".X" ) return;	// �g���q�� "X�t�@�C��" ����Ȃ���ΏI��.

		m_staticMeshList[fileName] = 
			std::make_shared<CDX9StaticMesh>( hWnd, pDevice11, pContext11, pDevice9, filePath.c_str() );
	};

	try {
		fs::recursive_directory_iterator dir_itr( FILE_PATH ), end_itr;
		std::for_each( dir_itr, end_itr, eachLoad );

	} catch( const fs::filesystem_error& e ){
		const char* errorMessage = e.path1().string().c_str();
		_ASSERT_EXPR( false, L"���b�V���t�@�C���Ǎ����s" );
		MessageBox( nullptr, errorMessage, "���b�V���t�@�C���Ǎ����s", MB_OK );
	}
}