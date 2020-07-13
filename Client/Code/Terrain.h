/*!
 * \file Terrain.h
 * \date 2017/05/31 15:17
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

#ifndef Terrain_h__
#define Terrain_h__

#include "GameObject.h"

namespace Engine
{
	class CResourceMgr;

	class CTransform;
	class CVIBuffer;
	class CTexture;
};

class CTerrain
	: public Engine::CGameObject
{
private:
	CTerrain(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CTerrain(void);

public:
	virtual void Update(void);
	virtual void Render(void);

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	void Release(void);

private:
	Engine::CResourceMgr*	m_pResourceMgr;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture[2];

private:
	D3DMATERIAL9			m_pMtrl;
	LPDIRECT3DTEXTURE9		m_pCreateTexture;

private:
	Engine::VTXTEX*			m_pTerrainVtx;
	Engine::INDEX32*		m_pIndex;
	DWORD					m_dwTriCnt;
	float					m_fDetail;
};

#endif // Terrain_h__