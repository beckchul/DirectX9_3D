/*!
 * \file Light.h
 * \date 2017/05/30 15:46
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

#ifndef Light_h__
#define Light_h__

#include "Engine_Include.h"

BEGIN(Engine)

class ENGINE_DLL CLight
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pDevice);

public:
	~CLight(void);

public:
	const D3DLIGHT9* GetLightInfo(void) const;

public:
	HRESULT InitLight(const D3DLIGHT9* pLightInfo, const DWORD& dwLightIdx);

public:
	static CLight* Create(LPDIRECT3DDEVICE9 pDevice
		, const D3DLIGHT9* pLightInfo, const DWORD& dwLightIdx);

private:
	LPDIRECT3DDEVICE9		m_pDevice;
	D3DLIGHT9				m_LightInfo;
};

END

#endif // Light_h__