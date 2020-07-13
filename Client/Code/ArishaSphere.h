#ifndef ArishaSphere_h__
#define ArishaSphere_h__

#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CStaticMesh;
	class CCollision_OBB;
};

class CArishaSphere
	: public Engine::CGameObject
{
private:
	CArishaSphere(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CArishaSphere(void);

public:
	void SetMatrixInfo(const D3DXMATRIX* pArishaInfo, const D3DXMATRIX* pSphereRef);
	const bool*  GetCheckCollision(void)
	{ return &m_bCheckCollision; }
public:
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	HRESULT SetConstantTable(LPD3DXEFFECT pEffect);
	bool	CollisionOBB(void);
public:
	static CArishaSphere* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CCollision_OBB*	m_pCollisionOBB;
private:
	Engine::CStaticMesh*	m_pMesh;
	Engine::CTransform*		m_pInfo;

private:
	const D3DXMATRIX*		m_pmatSphereRef;
	const D3DXMATRIX*		m_pmatArishaInfo;
	D3DXMATRIX				m_CollisionMatrix;
	bool					m_bRender;
	bool					m_bCheckCollision;
};


#endif // ArishaSphere_h__