#ifndef AnimationMainApp_h__
#define AnimationMainApp_h__

#include "Include.h"
#include "Engine_Function.h"
#include "Engine_Include.h"

namespace Engine
{
	class CGraphicDev;
	class CManagement;
	class CResourceMgr;
	class CTimeMgr;
	class CGameObject;
}

class CAnimationMainApp
{
public:
	DECLARE_SINGLETON(CAnimationMainApp)
private:
	CAnimationMainApp(void);

public:
	~CAnimationMainApp(void);

public:
	HRESULT InitApp(void);
	void Update(void);
	void Render(void);

private:
	void Release(void);

private:
	Engine::CGraphicDev*	m_pGraphicDev;
	Engine::CManagement*	m_pManagement;
	Engine::CResourceMgr*	m_pResourceMgr;
	Engine::CTimeMgr*		m_pTimeMgr;

private:
	LPDIRECT3DDEVICE9		m_pDevice;
};

#endif // AnimationMainApp_h__