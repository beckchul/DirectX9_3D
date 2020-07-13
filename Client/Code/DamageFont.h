#ifndef DamageFont_h__
#define DamageFont_h__

#include "GameObject.h"
#include "UIObserver.h"
namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CDamageFont
	: public Engine::CGameObject
{
private:
	CDamageFont(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CDamageFont(void);

public:
	virtual void Update(void);
	virtual void Render(void);

public:
	void RenderFont(int _iDamage, int _iIndex, D3DXVECTOR3 _vPos)
	{
		m_iNumber[_iIndex] = _iDamage;
		m_fCount[_iIndex] = 0.f;
		m_fRenderTime[_iIndex] = 0.f;
		m_vFontPos[_iIndex] = _vPos;
	}
private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

public:
	static CDamageFont* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*	m_pResourceMgr;
	CUIObserver*	m_pUIObserver;
private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
	Engine::VTXTEX*			m_pVertex;
private:
	WORD			m_wVtxCnt;
	float			m_fX, m_fY;
	float			m_fSizeX, m_fSizeY;
	D3DXMATRIX		m_matView, m_matProj;
	int				m_iNumber[8];
	D3DXVECTOR3		m_vFontPos[8];
	float			m_fCount[8], m_fRenderTime[8];
	int				m_iRenderIndex;
};

#endif // DamageFont_h__