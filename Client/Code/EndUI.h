#ifndef EndUI_h__
#define EndUI_h__

#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CEndUI
	: public Engine::CGameObject
{
private:
	CEndUI(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CEndUI(void);

public:
	virtual void Update(void);
	virtual void Render(void);

	void RenderEnd() { m_bRender = true; }
private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);

public:
	static CEndUI* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;

private:
	float			m_fX, m_fY;
	float			m_fSizeX, m_fSizeY;
	D3DXMATRIX		m_matView, m_matProj;
	bool			m_bRender;
};

#endif // EndUI_h__