#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include "..\Main\Global.h"
#include "..\Scene\SceneBase\SceneBase.h"

#include <unordered_map>

class CDX9SkinMesh;
class CDX9StaticMesh;

/**
 * @file ObjectBase.h
 * @brief �I�u�W�F�N�g�Ɍp������x�[�X�N���X.
 * @author OTakotiri.
 */

// �}�b�v�I�u�W�F�N�g�̏���ǂݍ��ނ��߂̍\����.
struct stObjectInfo
{
	D3DXVECTOR3	vPos;		// ���W���.
	D3DXVECTOR3	vRot;		// ��]���.
	int			StageNo;	// �X�e�[�W�i���o�[.
	std::string	ObjectName;	// �I�u�W�F�N�g��.
};
class CObjectBase
{
protected:

	CObjectBase()
		: m_vPosition		(0.0f, 0.0f, 0.0f)
		, m_vRotation		(0.0f, 0.0f, 0.0f)
		, m_vScale			(1.0f,1.0f,1.0f)
		, m_vCollisionScale	( 1.0f,1.0f,1.0f )
		, m_fAlpha			( 1.0f )
		, m_StageNo			( -1 )

	{};
	virtual ~CObjectBase() {};
public:
	// �X�V�֐�.
	virtual void Update() = 0;
	// �`��֐�.
	virtual void Render() = 0;
	// �ǂݍ��݊֐�.
	virtual void Load() = 0;
	// �X�^�e�B�b�N���b�V�����擾�֐�.
	virtual LPD3DXMESH	GetMeshData() = 0;
	// �I�u�W�F�N�g���擾�����ĂȂ���΋��Ԃ�.
	std::string GetObjName() { return m_ObjectName; }
protected:
	int			m_StageNo;		// ���̃I�u�W�F�N�g���g���Ă���X�e�[�WNo.
	std::string	m_ObjectName;	// �I�u�W�F�N�g��.
	D3DXVECTOR3	m_vPosition;	// ���W.
	D3DXVECTOR3	m_vRotation;	// ��].
	D3DXVECTOR3	m_vScale;		// ���f���̑傫��.
	D3DXVECTOR3	m_vCollisionScale;	// �����蔻��̑傫��.
	float		m_fAlpha;		// ���f���̓��ߒl.
public:
	// ���W���擾�֐�.
	inline const D3DXVECTOR3 GetPosition() const		{ return m_vPosition; }
	inline void SetPosition(const D3DXVECTOR3& pos)		{ m_vPosition = pos; }
	inline void SetPositionX(const float& posx)			{ m_vPosition.x = posx; }
	inline void SetPositionY(const float& posy)			{ m_vPosition.y = posy; }
	inline void SetPositionZ(const float& posz)			{ m_vPosition.z = posz; }
	// ��]���擾�֐�.
	inline const D3DXVECTOR3 GetRotation() const		{ return m_vRotation; }
	inline void SetRotation(const D3DXVECTOR3& rot)		{ m_vRotation = rot; }
	// �g�k���擾�֐�.
	inline const D3DXVECTOR3 GetScale() const			{ return m_vScale; }
	inline void SetScale(const D3DXVECTOR3& vScale)		{ m_vScale = vScale; }
	inline void SetScale(const float& Scale)			{ m_vScale = D3DXVECTOR3(Scale, Scale, Scale); }
	// �A���t�@�l�Z�b�g�֐�.
	inline void SetAlpha(const float& fAlpha)			{ m_fAlpha = fAlpha; }
	// �I�u�W�F�N�g�X�e�[�WNo�����蓖�Ă��Ă���ꍇ�A-1�ȏ��Ԃ��֐�.
	inline const int GetStageNo()						{ return m_StageNo; }
};

#endif // #ifndef OBJECT_BASE_H.