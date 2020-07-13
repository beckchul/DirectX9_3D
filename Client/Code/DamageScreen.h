/*!
* \file DamageScreen.h
* \date 2017/06/15 15:22
*
* \author Administrator
* Contact: user@company.com
*
* \brief
*
* TODO: long description
*
* \note
*/

#ifndef DamageScreen_h__
#define DamageScreen_h__

#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CDamageScreen
	: public Engine::CGameObject
{
private:
	CDamageScreen(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CDamageScreen(void);

public:
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);

public:
	static CDamageScreen* Create(LPDIRECT3DDEVICE9 pDevice);
	void RenderScreen(void) { m_fCount = 0.f, m_fTime = 0.f; }
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

#endif // DamageScreen_h__