#ifndef CromcruachSphere_h__
#define CromcruachSphere_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CStaticMesh;
	class CCollision_OBB;
};

class CCromcruachSphere
	: public Engine::CGameObject
{
private:
	CCromcruachSphere(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCromcruachSphere(void);

public:
	void SetMatrixInfo(const D3DXMATRIX* pmatCromcruachInfo, const D3DXMATRIX* pSphereRef);
	void SetMonsterState(const MONSTERSTATE* pMonsterState)
	{ m_pMonsterState = pMonsterState; }
	const bool* GetCheckCollision(void) { return &m_bHit; }

public:
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	HRESULT SetConstantTable(LPD3DXEFFECT pEffect);
	bool Collision_OBB(void);
	void StateCheck(void);
public:
	static CCromcruachSphere* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CCollision_OBB*	m_pCollisionOBB;
private:
	Engine::CStaticMesh*	m_pMesh;
	Engine::CTransform*		m_pInfo;

private:
	const D3DXMATRIX*		m_pmatSphereRef;
	const D3DXMATRIX*		m_pmatCromcruachInfo;
	const MONSTERSTATE*		m_pMonsterState;
	D3DXMATRIX				m_CollisionMatrix;
	bool					m_bRender;
	bool					m_bHit;
};


#endif // CromcruachSphere_h__