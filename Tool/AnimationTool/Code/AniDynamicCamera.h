#ifndef AniDynamicCamera_h__
#define AniDynamicCamera_h__

#include "Camera.h"

class CAniDynamicCamera
	: public Engine::CCamera
{
private:
	explicit CAniDynamicCamera(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CAniDynamicCamera(void);

public:
	virtual void Update(void);

public:
	static CAniDynamicCamera* Create(LPDIRECT3DDEVICE9 pDevice
		, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);

private:
	HRESULT InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);
	void KeyCheck(void);
private:
	float		m_fCamSpeed;
	bool		m_bMouseFix;
	bool		m_bClick;
};

#endif