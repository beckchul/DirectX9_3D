/*!
 * \file LightMgr.h
 * \date 2017/05/30 15:52
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

#ifndef LightMgr_h__
#define LightMgr_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CLight;

class ENGINE_DLL CLightMgr
{
public:
	DECLARE_SINGLETON(CLightMgr)

private:
	CLightMgr(void);
	~CLightMgr(void);

public:
	const D3DLIGHT9* GetLightInfo(const UINT& iIndex = 0);

public:
	HRESULT AddLight(LPDIRECT3DDEVICE9 pDevice
		, const D3DLIGHT9* pLightInfo
		, const DWORD& dwLightIdx);

private:
	void Release(void);

private:
	typedef list<CLight*>		LIGHTLIST;
	LIGHTLIST		m_Lightlist;
};

END

#endif // LightMgr_h__