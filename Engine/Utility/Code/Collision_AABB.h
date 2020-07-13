/*!
* \file Collision_AABB.h
* \date 2017/06/22 15:21
*
* \author Administrator
* Contact: user@company.com
*
* \brief
*
* TODO: long description
*
* \note
*/

#ifndef Collision_AABB_h__
#define Collision_AABB_h__

#include "Collision.h"

BEGIN(Engine)

class CMesh;
class CCubeColor;

class ENGINE_DLL CCollision_AABB
	: public CCollision
{
private:
	explicit CCollision_AABB(LPDIRECT3DDEVICE9 pDevice);
	explicit CCollision_AABB(const CCollision_AABB& rhs);

public:
	virtual ~CCollision_AABB(void);

public:
	const D3DXVECTOR3* GetMin(void) const { return &m_vMin; }
	const D3DXVECTOR3* GetMax(void) const { return &m_vMax; }
	const D3DXMATRIX* GetMatrix(void) const { return &m_matColliderMatrix; }

public:
	virtual CCollision* CloneColObject(void);

public:
	void SetColInfo(const D3DXMATRIX* pWorld
		, CMesh* pMesh
		, const D3DXVECTOR3* pMin = NULL
		, const D3DXVECTOR3* pMax = NULL);

public:
	virtual void Update(void);
	void Render(const DWORD& dwColor
		, CMesh* pMesh = NULL);
	virtual DWORD Release(void);

public:
	bool CheckCollision(const CCollision_AABB* pTarget);

private:
	HRESULT InitCollision(void);

public:
	static CCollision_AABB* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	D3DXMATRIX				m_matColliderMatrix;
	const D3DXMATRIX*		m_pmatWorld;
	CCubeColor*				m_pBoundingBox;

private:
	D3DXVECTOR3				m_vMin;
	D3DXVECTOR3				m_vMax;
};

END

#endif // Collision_AABB_h__