#ifndef __LightningEffect_h__
#define __LightningEffect_h__

#include "Engine_Include.h"
#include "Include.h"

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CTexture;
	class CVIBuffer;
};

class CLightningEffect
	: public Engine::CGameObject
{
private:
	CLightningEffect(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CLightningEffect(void);

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
	static CLightningEffect* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CTexture*				m_pTexture;
	Engine::CTransform*				m_pInfo;
	Engine::CVIBuffer*				m_pBuffer;

private:
	bool			m_bRender;
	float			m_fTimeCount;
	float			m_fCount;
	D3DXVECTOR3		m_vEffectPos;
	int				m_iIndex;
};

#endif	//__ExplosionEffect_h__