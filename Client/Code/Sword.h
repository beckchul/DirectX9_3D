#ifndef Sword_h__
#define Sword_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CStaticMesh;
	class CCollision_OBB;

	class CTexture;
	class CVIBuffer;
};

class CSword
	: public Engine::CGameObject
{
private:
	CSword(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CSword(void);

private:
	enum TRAIL_POINT 
		{ CURCUR_MIN_POINT
		, CURCUR_MAX_POINT
		, CUR_MIN_POINT
		, CUR_MAX_POINT
		, NEW_MIN_POINT
		, NEW_MAX_POINT
		, POINT_END };
public:
	void SetMatrixInfo(const D3DXMATRIX* pArishaInfo, const D3DXMATRIX* pWeaponRef);
	void SetPlayerState(const PLAYERSTATE* pPlayerSteate);
public:
	virtual void Update(void);
	virtual void Render(void);
	void TrailRender(void);
private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	HRESULT SetConstantTable(LPD3DXEFFECT pEffect);
	void StateCheck(void);
	void TrailRenderCheck(void);
public:
	static CSword* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CStaticMesh*	m_pMesh;
	Engine::CTransform*		m_pInfo;
	Engine::CCollision_OBB* m_pCollisionOBB;
	Engine::CTexture*		m_pTexture;
	Engine::CVIBuffer*		m_pBuffer;

private:
	const D3DXMATRIX*		m_pmatWeaponRef;
	const D3DXMATRIX*		m_pmatArishaInfo;
	D3DXMATRIX				m_CollisionMatrix;
	const PLAYERSTATE*		m_pPlayerState;
	PLAYERSTATE				m_ePrePlayerState;
	D3DXVECTOR3				m_vTrailPoint[POINT_END];
	float					m_fColCount;
	bool					m_bTrailRender[2];
};


#endif // Sword_h__