/*!
* \file CubeColor.h
* \date 2016/01/11 9:52
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
#ifndef CubeColor_h__
#define CubeColor_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeColor
	: public CVIBuffer
{
private:
	explicit CCubeColor(LPDIRECT3DDEVICE9 pDevice);
	explicit CCubeColor(const CCubeColor& rhs);

public:
	virtual ~CCubeColor(void);

public:
	void SetBoundingBox(const D3DXVECTOR3* pMin
		, const D3DXVECTOR3* pMax
		, const DWORD& dwColor);

public:
	virtual HRESULT CreateBuffer(const D3DXVECTOR3& vMin
		, const D3DXVECTOR3& vMax, const DWORD& dwColor);
	virtual CResources* CloneResource(void);
	virtual void Render(const D3DXMATRIX* pmatWorld);

public:
	static CCubeColor* Create(LPDIRECT3DDEVICE9 pDevice
		, const D3DXVECTOR3& vMin, const D3DXVECTOR3& vMax
		, const DWORD& dwColor);
};

END

#endif // CubeColor_h__
