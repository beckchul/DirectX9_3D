#ifndef MonsterHpBar_h__
#define MonsterHpBar_h__

#include "GameObject.h"
#include "UIObserver.h"
namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CMonsterHpBar
	: public Engine::CGameObject
{
private:
	CMonsterHpBar(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CMonsterHpBar(void);

public:
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);
	void SettingHpBar(void);
public:
	static CMonsterHpBar* Create(LPDIRECT3DDEVICE9 pDevice);

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

#endif // MonsterHpBar_h__