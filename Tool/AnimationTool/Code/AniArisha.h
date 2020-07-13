#ifndef AniArisha_h__
#define AniArisha_h__

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CDynamicMesh;
};

class CAniArisha
	: public Engine::CGameObject
{
private:
	CAniArisha(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CAniArisha(void);

public:
	const D3DXMATRIX* GetArishaInfo(void);

public:
	virtual void Update(void);
	virtual void Render(void);

public:
	Engine::ANI_INFO* GetAniInfo(void)
	{ return &m_tAniInfo; }

public:
	void AnimationStart(void)
	{ m_bAnimationStart = true; }

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void SetConstantTable(void);
	void Release(void);

public:
	static CAniArisha* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CDynamicMesh*	m_pMesh;
	Engine::CTransform*		m_pInfo;
private:
	bool				m_bPush;
	Engine::ANI_INFO	m_tAniInfo;
	float				m_fSpeed;
private:
	const D3DXMATRIX*	m_pArishaInfo;
	bool				m_bAnimationStart;
};


#endif//AniArisha_h__