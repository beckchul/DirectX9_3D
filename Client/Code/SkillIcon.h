#ifndef SkillIcon_h__
#define SkillIcon_h__

#include "GameObject.h"
#include "UIObserver.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CSkillIcon
	: public Engine::CGameObject
{
private:
	CSkillIcon(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CSkillIcon(void);

public:
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(int _iIndex);
	HRESULT AddComponent(void);
public:
	static CSkillIcon* Create(LPDIRECT3DDEVICE9 pDevice, int _iIndex);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
	CUIObserver*	m_pUIObserver;
private:
	float			m_fX, m_fY;
	float			m_fSizeX, m_fSizeY;
	D3DXMATRIX		m_matView, m_matProj;
	int				m_iIndex;
};

#endif // SkillIcon_h__