/*!
 * \file DynamicCamera.h
 * \date 2017/05/31 15:37
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

#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Camera.h"

class CDynamicCamera
	: public Engine::CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CDynamicCamera(void);

public:
	virtual void Update(void);

public:
	static CDynamicCamera* Create(LPDIRECT3DDEVICE9 pDevice
		, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);

private:
	HRESULT InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt);
	void KeyCheck(void);
	void FixMouse(void);
	void MouseMove(void);

private:
	float		m_fCamSpeed;
	bool		m_bMouseFix;
	bool		m_bClick;
};

#endif // DynamicCamera_h__