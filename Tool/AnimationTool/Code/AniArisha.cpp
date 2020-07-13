#include "stdafx.h"
#include "AniArisha.h"
#include "Export_Function.h"
#include "Transform.h"
#include "DynamicMesh.h"
#include "Shader.h"

CAniArisha::CAniArisha(LPDIRECT3DDEVICE9 pDevice)
: CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_fSpeed(0.f)
, m_bAnimationStart(false)
{
}

CAniArisha::~CAniArisha(void)
{
	Release();
}

HRESULT CAniArisha::Initialize(void)
{
	FAILED_CHECK(AddComponent());
	m_tAniInfo.iAniIndex = 1;
	m_tAniInfo.fAniSpeedFront = 1.f;
	m_tAniInfo.fAniSpeedEnd = 1.f;
	m_tAniInfo.fAniWeightFront = 0.8f;
	m_tAniInfo.fAniWeightEnd = 0.2f;
	m_tAniInfo.dMagicNumber = 0.25;
	m_tAniInfo.dEndAniCount = 0.4;

	m_pInfo->m_vPos = D3DXVECTOR3(-1.f, 0.f, 1.f);
	m_pInfo->m_vScale = D3DXVECTOR3(0.03f, 0.03f, 0.03f);

	m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(150.f);
	return S_OK;
}

void CAniArisha::Update(void)
{
	if (g_AniSelect != 0)
		return;

	if (m_bAnimationStart)
	{
		m_bAnimationStart = false;
		m_pMesh->SetAnimationSet(m_tAniInfo);
	}

	if (m_pMesh->GetPeriod() - m_tAniInfo.dEndAniCount <= m_pMesh->GetTrackPos())
	{
		m_tAniInfo.iAniIndex = 0;
		m_pMesh->SetAnimationSet(m_tAniInfo);
		m_pMesh->SetTrackPos(0.0);
	}

	Engine::CGameObject::Update();
}

void CAniArisha::Render(void)
{
	if (g_AniSelect != 0)
		return;

	SetConstantTable();

	m_pMesh->FrameMove(Engine::Get_TimeMgr()->GetTime());
	m_pMesh->RenderMesh_ForSDK(m_pShader->GetEffectHandle()
		, &m_pInfo->m_matWorld);
}

void CAniArisha::Release(void)
{

}

HRESULT CAniArisha::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"DynamicMesh_Arisha");
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"DynamicMesh", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->CloneShader(L"Shader_DynamicMesh");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));

	return S_OK;
}

CAniArisha * CAniArisha::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CAniArisha*		pGameObject = new CAniArisha(pDevice);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

const D3DXMATRIX * CAniArisha::GetArishaInfo(void)
{
	return m_pArishaInfo;
}

void CAniArisha::SetConstantTable(void)
{
	LPD3DXEFFECT	pEffect = m_pShader->GetEffectHandle();
	NULL_CHECK(pEffect);

	pEffect->SetVector("gLightColor", &D3DXVECTOR4(1, 1, 1, 1));
	pEffect->SetVector("gWorldLightPosition", &D3DXVECTOR4(500, 500, 0, 1));
	D3DXMATRIX		matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("gWorldCameraPosition", (D3DXVECTOR4*)&matView.m[3][0]);

}