#ifndef CromcruachHitBox_h__
#define CromcruachHitBox_h__

#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CStaticMesh;
	class CCollision_OBB;
};

class CCromcruachHitBox
	: public Engine::CGameObject
{
private:
	CCromcruachHitBox(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCromcruachHitBox(void);

public:
	void SetCollision(bool _bCollision) { m_bCheckCollision = _bCollision; }
	void SetColPos(D3DXVECTOR3 _vPos) { m_vCollisionPos = _vPos; }
	void SetColScale(D3DXVECTOR3 _vScale);
public:
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
public:
	static CCromcruachHitBox* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CCollision_OBB*	m_pCollisionOBB;
private:
	Engine::CStaticMesh*	m_pMesh;
	Engine::CTransform*		m_pInfo;

private:
	bool					m_bCheckCollision;
	D3DXVECTOR3				m_vCollisionPos;
};


#endif // CromcruachHitBox_h__