#ifndef ManaScreen_h__
#define ManaScreen_h__

#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CManaScreen
	: public Engine::CGameObject
{
private:
	CManaScreen(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CManaScreen(void);

public:
	void RenderScreen(void)
	{
		m_fTime = 0.f;
		m_fCount = 0.f;
	}
public:
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	
public:
	static CManaScreen* Create(LPDIRECT3DDEVICE9 pDevice);

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
	float			m_fTime, m_fCount;
};

#endif // ManaScreen_h__