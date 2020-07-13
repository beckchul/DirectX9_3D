#include "stdafx.h"
#include "AniDynamicCamera.h"

#include "Include.h"
#include "Export_Function.h"

CAniDynamicCamera::CAniDynamicCamera(LPDIRECT3DDEVICE9 pDevice)
: Engine::CCamera(pDevice)
, m_fCamSpeed(0.f)
, m_bMouseFix(true)
, m_bClick(false)
{

}

CAniDynamicCamera::~CAniDynamicCamera(void)
{

}

void CAniDynamicCamera::Update(void)
{
	KeyCheck();
	Engine::CCamera::Update();
}

CAniDynamicCamera* CAniDynamicCamera::Create(LPDIRECT3DDEVICE9 pDevice, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	CAniDynamicCamera*		pCamera = new CAniDynamicCamera(pDevice);
	if (FAILED(pCamera->InitCamera(pEye, pAt)))
		Engine::Safe_Delete(pCamera);

	return pCamera;
}

HRESULT CAniDynamicCamera::InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	m_fCamSpeed = 10.f;

	m_vEye = *pEye;
	m_vAt = *pAt;
	CCamera::Invalidate_ViewMatrix();

	m_fFovY = D3DXToRadian(40.f);
	m_fAspect = float(WINCX) / WINCY;
	m_fNear = 1.f;
	m_fFar = 1000.f;
	CCamera::Invalidate_ProjMatrix();

	return S_OK;
}

void CAniDynamicCamera::KeyCheck(void)
{
	float fTime = Engine::Get_TimeMgr()->GetTime();

	if (GetAsyncKeyState(VK_UP))
	{
		D3DXVECTOR3		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye += vLook * m_fCamSpeed * fTime;
		m_vAt += vLook * m_fCamSpeed * fTime;
	}

	if (GetAsyncKeyState(VK_DOWN))
	{
		D3DXVECTOR3		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		m_vEye -= vLook * m_fCamSpeed * fTime;
		m_vAt -= vLook * m_fCamSpeed * fTime;
	}
}
