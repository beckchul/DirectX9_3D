/*!
 * \file SkyBox.h
 * \date 2017/06/15 15:39
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

#ifndef SkyBox_h__
#define SkyBox_h__

#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CSkyBox
	: public Engine::CGameObject
{
private:
	CSkyBox(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CSkyBox(void);

public:
	virtual void Update(void);
	virtual void Render(void);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);

public:
	static CSkyBox* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
};


#endif // SkyBox_h__