#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include "..\Main\Global.h"
#include "..\Scene\SceneBase\SceneBase.h"

#include <unordered_map>

class CDX9SkinMesh;
class CDX9StaticMesh;

/**
 * @file ObjectBase.h
 * @brief オブジェクトに継承するベースクラス.
 * @author OTakotiri.
 */

// マップオブジェクトの情報を読み込むための構造体.
struct stObjectInfo
{
	D3DXVECTOR3	vPos;		// 座標情報.
	D3DXVECTOR3	vRot;		// 回転情報.
	int			StageNo;	// ステージナンバー.
	std::string	ObjectName;	// オブジェクト名.
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
	// 更新関数.
	virtual void Update() = 0;
	// 描画関数.
	virtual void Render() = 0;
	// 読み込み関数.
	virtual void Load() = 0;
	// スタティックメッシュ情報取得関数.
	virtual LPD3DXMESH	GetMeshData() = 0;
	// オブジェクト名取得入ってなければ空を返す.
	std::string GetObjName() { return m_ObjectName; }
protected:
	int			m_StageNo;		// このオブジェクトが使われているステージNo.
	std::string	m_ObjectName;	// オブジェクト名.
	D3DXVECTOR3	m_vPosition;	// 座標.
	D3DXVECTOR3	m_vRotation;	// 回転.
	D3DXVECTOR3	m_vScale;		// モデルの大きさ.
	D3DXVECTOR3	m_vCollisionScale;	// 当たり判定の大きさ.
	float		m_fAlpha;		// モデルの透過値.
public:
	// 座標情報取得関数.
	inline const D3DXVECTOR3 GetPosition() const		{ return m_vPosition; }
	inline void SetPosition(const D3DXVECTOR3& pos)		{ m_vPosition = pos; }
	inline void SetPositionX(const float& posx)			{ m_vPosition.x = posx; }
	inline void SetPositionY(const float& posy)			{ m_vPosition.y = posy; }
	inline void SetPositionZ(const float& posz)			{ m_vPosition.z = posz; }
	// 回転情報取得関数.
	inline const D3DXVECTOR3 GetRotation() const		{ return m_vRotation; }
	inline void SetRotation(const D3DXVECTOR3& rot)		{ m_vRotation = rot; }
	// 拡縮情報取得関数.
	inline const D3DXVECTOR3 GetScale() const			{ return m_vScale; }
	inline void SetScale(const D3DXVECTOR3& vScale)		{ m_vScale = vScale; }
	inline void SetScale(const float& Scale)			{ m_vScale = D3DXVECTOR3(Scale, Scale, Scale); }
	// アルファ値セット関数.
	inline void SetAlpha(const float& fAlpha)			{ m_fAlpha = fAlpha; }
	// オブジェクトステージNoが割り当てられている場合、-1以上を返す関数.
	inline const int GetStageNo()						{ return m_StageNo; }
};

#endif // #ifndef OBJECT_BASE_H.