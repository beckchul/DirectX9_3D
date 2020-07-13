/*!
 * \file RcTex.h
 * \date 2015/04/04 23:00
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
#ifndef RcTex_h__
#define RcTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CRcTex
	: public CVIBuffer
{
private:
	explicit CRcTex(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CRcTex(void);

public:
	virtual CResources* CloneResource(void);
	void SetEffectBuffer(const D3DXVECTOR3* pMin, const D3DXVECTOR3* pMax);
	void ReSetBuffer(void);
	void SetEffectUV(float fSizeX, float fSizeY, float fPosX, float fPosY);
	void ReSetUV(void);
public:
	virtual HRESULT CreateBuffer(void);

public:
	static CRcTex* Create(LPDIRECT3DDEVICE9 pDevice);
};

END

#endif // RcTex_h__