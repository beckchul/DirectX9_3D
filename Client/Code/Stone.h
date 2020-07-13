/*!
 * \file Stone.h
 * \date 2017/06/05 16:02
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

#ifndef Stone_h__
#define Stone_h__

#include "LandObject.h"

namespace Engine
{
	class CVIBuffer;
	class CTransform;
	class CStaticMesh;
	class CTexture;
};

class CStone
	: public CLandObject
{
private:
	CStone(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CStone(void);

public:
	virtual void Update(void);
	virtual void Render(void);

public:
	void SetEffexct(D3DXVECTOR3 _vEffectPos, float _fSize);
private:
	HRESULT Initialize(void);
	HRESULT AddComponent(void);
	HRESULT SetConstantTable(LPD3DXEFFECT pEffect);
public:
	static CStone* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	Engine::CStaticMesh*	m_pMesh;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTexture*		m_pTexture;
private:
	bool		m_bCollision;
	LPD3DXMESH	m_pInfoMesh;

private:
	D3DXVECTOR3	m_vEffectPos;
	float		m_fSize;
};


#endif // Stone_h__