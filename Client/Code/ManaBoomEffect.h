#ifndef __ManaBoomEffect_h__
#define __ManaBoomEffect_h__

#include "Engine_Include.h"
#include "Include.h"

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CTexture;
	class CVIBuffer;
};

class CManaBoomEffect
	: public Engine::CGameObject
{
private:
	CManaBoomEffect(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CManaBoomEffect(void);

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
	void State(void);
	void Release(void);
public:
	static CManaBoomEffect* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CTexture*				m_pTexture;
	Engine::CTransform*				m_pInfo;
	Engine::CVIBuffer*				m_pBuffer;

private:
	bool			m_bRender;
	float			m_fTimeCount;
	float			m_fCount;
	D3DXVECTOR3		m_vEffectPos;
	D3DXVECTOR3		m_vHitBoxDir;
};

#endif	//__ExplosionEffect_h__