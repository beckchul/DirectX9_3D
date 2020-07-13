#ifndef __PunchEffect_h__
#define __PunchEffect_h__

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

class CPunchEffect
	: public Engine::CGameObject
{
private:
	CPunchEffect(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPunchEffect(void);

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
	static CPunchEffect* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*			m_pResourceMgr;
	Engine::CTexture*				m_pTexture;
	Engine::CTransform*				m_pInfo;
	Engine::CVIBuffer*				m_pBuffer;

private:
	bool			m_bRender;
	float			m_fTimeCount;
	float			m_fCount;
	float			m_fRenderCount;
	D3DXVECTOR3		m_vEffectPos;
};

#endif	//__PunchEffect_h__