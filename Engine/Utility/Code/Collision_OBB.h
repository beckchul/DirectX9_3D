/*!
* \file Collision_OBB.h
* \date 2017/06/20 16:40
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

#ifndef Collision_OBB_h__
#define Collision_OBB_h__

#include "Collision.h"

BEGIN(Engine)

class CMesh;
class CCubeColor;

class ENGINE_DLL CCollision_OBB
	: public CCollision
{
public:
	enum AXISID { AXIS_X, AXIS_Y, AXIS_Z, AXIS_END };

private:
	typedef struct tagOBB_Info
	{
		D3DXVECTOR3		vCenter;
		D3DXVECTOR3		vProj[AXIS_END];
		D3DXVECTOR3		vParallel[AXIS_END];
	}OBB;

private:
	explicit CCollision_OBB(LPDIRECT3DDEVICE9 pDevice);
	explicit CCollision_OBB(const CCollision_OBB& rhs);

public:
	virtual ~CCollision_OBB(void);

public:
	const OBB* GetObbInfo(void) const { return &m_tOBB; }
	bool GetCollisionState(void) const { return m_bCollision; }
	void SetCollisionState(bool bCollision)
	{ m_bCollision = bCollision; }
	virtual CCollision* CloneColObject(void);

public:
	D3DXVECTOR3 GetMinPoint(void) { return m_vMin; }
	D3DXVECTOR3 GetMaxPoint(void) { return m_vMax; }

	bool GetCheck(void) { return m_bCheck; }
	void CompleteCheck(void) { m_bCheck = true; }
	void ClearCheck(void) { m_bCheck = false; }
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
	bool CheckCollision(const CCollision_OBB* pTarget);

private:
	HRESULT InitCollision(void);
	void ComputePoint(void);
	void ComputeAxis(void);

public:
	static CCollision_OBB* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	const D3DXMATRIX*		m_pmatWorld;
	CCubeColor*				m_pBoundingBox;

private:
	D3DXVECTOR3				m_vPoint[8];
	D3DXVECTOR3				m_vMin;
	D3DXVECTOR3				m_vMax;
	OBB						m_tOBB;
	bool					m_bCollision;
	bool					m_bCheck;
};

END

#endif // Collision_OBB_h__