#ifndef PlayerHpBar_h__
#define PlayerHpBar_h__

#include "GameObject.h"
#include "UIObserver.h"
namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CPlayerHpBar
	: public Engine::CGameObject
{
private:
	CPlayerHpBar(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPlayerHpBar(void);

public:
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);
	void SettingHpBar(void);
public:
	static CPlayerHpBar* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*	m_pResourceMgr;
	CUIObserver*	m_pUIObserver;
private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::VTXTEX*			m_pVertex;
	Engine::CTexture*		m_pTexture;
	
private:
	WORD			m_wVtxCnt;
	float			m_fX, m_fY;
	float			m_fSizeX, m_fSizeY;
	D3DXMATRIX		m_matView, m_matProj;
};

#endif // PlayerHpBar_h__