#ifndef AniCromcruach_h__
#define AniCromcruach__

#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CDynamicMesh;
};

class CAniCromcruach
	: public Engine::CGameObject
{
private:
	CAniCromcruach(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CAniCromcruach(void);

public:
	Engine::ANI_INFO* GetAniInfo(void)
	{	return &m_tAniInfo;}

public:
	void AnimationStart(void)
	{	m_bAnimationStart = true;}
public:
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void KeyCheck(void);
	void SetConstantTable(void);
	void Release(void);
public:
	static CAniCromcruach* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CDynamicMesh*	m_pMesh;
	Engine::CTransform*		m_pInfo;
private:
	bool				m_bPush;
	Engine::ANI_INFO	m_tAniInfo;
	float				m_fSpeed;
	bool				m_bAnimationStart;
};


#endif // AniCromcruach_h__