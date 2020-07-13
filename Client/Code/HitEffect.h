#ifndef __HitEffect_h__
#define __HitEffect_h__

#include "Engine_Include.h"
#include "Include.h"

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CResourceMgr;
	class CTexture;
	class CVIBuffer;
	class CCollision_OBB;
};

class CHitEffect
	: public Engine::CGameObject
{
private:
	CHitEffect(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CHitEffect(void);

public:
	virtual void Update(void);
	virtual void Render(void);

public:
	void SetEffectMatrix(D3DXMATRIX _Matrix);
	void SetRender(bool _bRender) { m_bRender = _bRender; }

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	HRESULT SetConstantTable(LPD3DXEFFECT pEffect);
	void State(void);
	void Release(void);
public:
	static CHitEffect* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*			m_pResourceMgr;
	Engine::CTexture*				m_pTexture;
	Engine::CTransform*				m_pInfo;
	Engine::CVIBuffer*				m_pBuffer;
	const Engine::CCollision_OBB*	m_pSwordOBB;

private:
	bool			m_bRender;
	float			m_fScale;
	D3DXMATRIX		m_matEffectMatrix;
};

#endif	//__HitEffect_h__