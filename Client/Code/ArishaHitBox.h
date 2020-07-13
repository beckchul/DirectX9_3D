#ifndef ArishaHitBox_h__
#define ArishaHitBox_h__

#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CStaticMesh;
	class CCollision_OBB;
};

class CArishaHitBox
	: public Engine::CGameObject
{
private:
	CArishaHitBox(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CArishaHitBox(void);

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
	static CArishaHitBox* Create(LPDIRECT3DDEVICE9 pDevice);

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


#endif // ArishaHitBox_h__