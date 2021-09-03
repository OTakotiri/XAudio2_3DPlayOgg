#ifndef MESH_RESORCE_H
#define MESH_RESORCE_H

//�x���ɂ��Ă̺��ޕ��͂𖳌��ɂ���. 4005:�Ē�`.
#pragma warning( disable:4005)

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <thread>
#include <filesystem>	// C++17 �K�{.

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

	// �C���X�^���X�̎擾.
	static CMeshResource* GetInstance();

	// ���f���̓ǂݍ���.
	static void Load( 
		HWND hWnd, 
		ID3D11Device* pDevice11, 
		ID3D11DeviceContext* pContext11,
		LPDIRECT3DDEVICE9 pDevice9 );
	// �X���b�h�̉��.
	static bool ThreadRelease();

	// �X�^�e�B�b�N���b�V���̓ǂݍ���.
	static std::shared_ptr<CDX9StaticMesh> GetStatic( const std::string& name );
	static bool GetStatic( std::shared_ptr<CDX9StaticMesh>& model, const std::string& name );

private:
	// ���f���̓ǂݍ���.
	void ModelLoad( 
		HWND hWnd, 
		ID3D11Device* pDevice11, 
		ID3D11DeviceContext* pContext11,
		LPDIRECT3DDEVICE9 pDevice9 );

private:
	static_mesh_map m_staticMeshList;	// �X�^�e�B�b�N���b�V���̃��X�g.

	std::thread		m_thread;	// �X���b�h.

	bool	m_isLoadEnd;		// ���[�h���I��������.
	bool	m_isThreadJoined;	// �X���b�h��������ꂽ��.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CMeshResource( const CMeshResource & )				= delete;
	CMeshResource& operator = ( const CMeshResource & )	= delete;
	CMeshResource(CMeshResource && )						= delete;
	CMeshResource& operator = (CMeshResource && )		= delete;
};

#endif	// #ifndef MESH_RESORCE_H.