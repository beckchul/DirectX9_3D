#include "stdafx.h"
#include "StaticCamera.h"
#include "Include.h"
#include "Export_Function.h"
#include "Transform.h"

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pDevice)
: Engine::CCamera(pDevice)
, m_pTargetInfo(NULL)
, m_fTargetDistance(0.f)
, m_fCamSpeed(0.f)
{
}

CStaticCamera::~CStaticCamera(void)
{

}

void CStaticCamera::Update(void)
{

	KeyCheck();
	if (g_iCameraOption == 0)
	{
		if (!m_bRenderCharacter)
			TargetRenewal();
		else
			CharacterRender();
	}
	else if (g_iCameraOption == 1)
	{
		ResonanceRender();
	}
	else if (g_iCameraOption == 2)
	{
		RordOfManaOneRender();
	}
	else if (g_iCameraOption == 3)
	{
		RordOfManaTwoRender();
	}
	else if (g_iCameraOption == 4)
	{
		RordOfManaThreeRender();
	}
	else if (g_iCameraOption == 5)
	{
		RordOfManaFourRender();
	}

	Engine::CCamera::Update();
}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pDevice, const Engine::CTransform* pTargetInfo
	, const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	CStaticCamera*		pCamera = new CStaticCamera(pDevice);
	if (FAILED(pCamera->InitCamera(pEye, pAt)))
		Safe_Delete(pCamera);

	pCamera->SetCameraTarget(pTargetInfo);
	return pCamera;
}

HRESULT CStaticCamera::InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	m_fCamSpeed = 10.f;

	m_vEye = *pEye;
	m_vAt = *pAt;
	CCamera::Invalidate_ViewMatrix();

	m_fFovY = D3DXToRadian(30.f);
	m_fAspect = float(WINCX) / WINCY;
	m_fNear = 1.f;
	m_fFar = 2500.f;
	CCamera::Invalidate_ProjMatrix();
	return S_OK;
}

void CStaticCamera::KeyCheck(void)
{
	float		fTime = Engine::Get_TimeMgr()->GetTime();

	//if (Engine::Get_Input()->GetDIKeyState(DIK_A))
	//	m_fAngle += D3DXToRadian(90.f) * fTime * 2.f;

	//if (Engine::Get_Input()->GetDIKeyState(DIK_D))
	//	m_fAngle -= D3DXToRadian(90.f) * fTime * 2.f;


	if (Engine::Get_Input()->GetDIKeyState(DIK_Q))
		m_fTargetDistance += 500.f * fTime;

	if (Engine::Get_Input()->GetDIKeyState(DIK_E))
		m_fTargetDistance -= 500.f * fTime;

	if (Engine::Get_Input()->GetDIKeyState(DIK_P))
		m_bRenderCharacter = true;
	else
		m_bRenderCharacter = false;
}

void CStaticCamera::TargetRenewal(void)
{
	m_vEye = m_pTargetInfo->m_vDir * -1.f * m_fTargetDistance;

	D3DXVECTOR3		vRight;
	memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(D3DXVECTOR3));

	D3DXMATRIX		matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(30.f));
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);

	m_vAt = m_pTargetInfo->m_vPos + D3DXVECTOR3(0.f, 2.f, 0.f);
	m_vEye += m_pTargetInfo->m_vPos;
}

void CStaticCamera::CharacterRender(void)
{
	m_vEye = m_pTargetInfo->m_vDir * 150;

	D3DXVECTOR3		vRight;
	memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(D3DXVECTOR3));

	D3DXMATRIX		matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(-20.f));
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);

	m_vAt = m_pTargetInfo->m_vPos + D3DXVECTOR3(0.f, 1.2f, 0.f);
	m_vEye += m_pTargetInfo->m_vPos;
}

void CStaticCamera::ResonanceRender(void)
{
	m_vEye = m_pTargetInfo->m_vDir * 400.f * -1.f;

	D3DXVECTOR3		vRight;
	memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(D3DXVECTOR3));

	D3DXMATRIX		matRotAxis, matRotUp;
	D3DXMatrixRotationAxis(&matRotUp, &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(-35.f));
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(10.f));
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotUp);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);

	m_vAt = m_pTargetInfo->m_vPos + D3DXVECTOR3(0.f, 1.2f, 0.f);
	m_vEye += m_pTargetInfo->m_vPos;
}

void CStaticCamera::RordOfManaOneRender(void)
{
	m_vEye = m_pTargetInfo->m_vDir * 200;

	D3DXVECTOR3		vRight;
	memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(D3DXVECTOR3));

	D3DXMATRIX		matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(-20.f));
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);

	m_vAt = m_pTargetInfo->m_vPos + D3DXVECTOR3(0.f, 2.f, 0.f);
	m_vEye += m_pTargetInfo->m_vPos + D3DXVECTOR3(0.f, -2.f, 0.f);
}

void CStaticCamera::RordOfManaTwoRender(void)
{
	m_vEye = m_pTargetInfo->m_vDir * 300;

	D3DXVECTOR3		vRight;
	memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(D3DXVECTOR3));

	D3DXMATRIX		matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(-20.f));
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);

	m_vAt = m_pTargetInfo->m_vPos;
	m_vEye += m_pTargetInfo->m_vPos;
}

void CStaticCamera::RordOfManaThreeRender(void)
{
	m_vEye = m_pTargetInfo->m_vDir * -1.f * 150;

	D3DXVECTOR3		vRight;
	memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(D3DXVECTOR3));

	D3DXMATRIX		matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(20.f));
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);

	m_vAt = m_pTargetInfo->m_vPos + D3DXVECTOR3(0.f, 2.f, 0.f);
	m_vEye += m_pTargetInfo->m_vPos;
}

void CStaticCamera::RordOfManaFourRender(void)
{
	m_vEye = m_pTargetInfo->m_vDir * -1.f * 600;

	D3DXVECTOR3		vRight;
	memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(D3DXVECTOR3));

	D3DXMATRIX		matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, D3DXToRadian(20.f));
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);

	m_vAt = m_pTargetInfo->m_vPos + D3DXVECTOR3(0.f, 3.f, 0.f);
	m_vEye += m_pTargetInfo->m_vPos;
}

void CStaticCamera::SetCameraTarget(const Engine::CTransform* pTargetInfo)
{
	m_pTargetInfo = pTargetInfo;
	m_fTargetDistance = 600.f;
	m_fCamSpeed = 10.f;
}
