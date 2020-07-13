#ifndef __ResonanceBoomEffect_h__
#define __ResonanceBoomEffect_h__

#include "Engine_Include.h"
#include "Include.h"

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CTexture;
	class CVIBuffer;
};

class CResonanceBoomEffect
	: public Engine::CGameObject
{
private:
	CResonanceBoomEffect(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CResonanceBoomEffect(void);

public:
	virtual void Update(void);
	virtual void Render(void);

public:
	void SetEffectPos(D3DXVECTOR3 _vPos, D3DXVECTOR3 _vDir);
	void SetRender(bool _bRender);
private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	HRESULT SetConstantTable(LPD3DXEFFECT pEffect);
	HRESULT SetConstantTableSecond(LPD3DXEFFECT pEffect);
	void RenderFirst(void);
	void RenderSecond(void);
	void State(void);
	void Release(void);
public:
	static CResonanceBoomEffect* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CTexture*				m_pTexture[2];
	Engine::CTransform*				m_pInfo;
	Engine::CVIBuffer*				m_pBuffer;

private:
	bool			m_bRender[2];
	float			m_fTimeCount[2];
	float			m_fCount[2];
	D3DXVECTOR3		m_vEffectPos;
	D3DXVECTOR3		m_vHitBoxDir;
};

#endif	//__ExplosionEffect_h__