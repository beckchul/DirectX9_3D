#ifndef __ManaHandEffect_h__
#define __ManaHandEffect_h__

#include "Engine_Include.h"
#include "Include.h"

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CResourceMgr;
	class CTexture;
	class CVIBuffer;
};

class CManaHandEffect
	: public Engine::CGameObject
{
private:
	CManaHandEffect(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CManaHandEffect(void);

public:
	virtual void Update(void);
	virtual void Render(void);

public:
	void SetEffectPos(D3DXVECTOR3 _vPos);
	void SetRender(bool _bRender);
private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	HRESULT SetConstantTable(LPD3DXEFFECT pEffect);
	void State(void);
	void Release(void);
public:
	static CManaHandEffect* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*			m_pResourceMgr;
	Engine::CTexture*				m_pTexture;
	Engine::CTransform*				m_pInfo;
	Engine::CVIBuffer*				m_pBuffer;

private:
	bool			m_bRender;
	float			m_fTimeCount;
	float			m_fCount;
	D3DXVECTOR3		m_vEffectPos;
};

#endif	//__ExplosionEffect_h__