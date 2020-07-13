#ifndef Staff_h__
#define Staff_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CStaticMesh;
	class CCollision_OBB;
};

class CStaff
	: public Engine::CGameObject
{
private:
	CStaff(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CStaff(void);

public:
	void SetMatrixInfo(const D3DXMATRIX* pCromcruachInfo, const D3DXMATRIX* pWeaponRef);
	void SetMonsterState(const MONSTERSTATE* pMonsterState)
	{ m_pMonsterState = pMonsterState; }
public:
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	HRESULT SetConstantTable(LPD3DXEFFECT pEffect);
	void StateCheck(void);
public:
	static CStaff* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CStaticMesh*	m_pMesh;
	Engine::CTransform*		m_pInfo;
	Engine::CCollision_OBB* m_pCollisionOBB;

private:
	const D3DXMATRIX*		m_pmatWeaponRef;
	const D3DXMATRIX*		m_pmatCromcruachInfo;
	const MONSTERSTATE*		m_pMonsterState;
	D3DXMATRIX				m_CollisionMatrix;
};


#endif // Staff_h__