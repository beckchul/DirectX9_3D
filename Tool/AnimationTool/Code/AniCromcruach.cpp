#include "stdafx.h"
#include "AniCromcruach.h"
#include "Export_Function.h"
#include "Transform.h"
#include "DynamicMesh.h"
#include "Shader.h"

CAniCromcruach::CAniCromcruach(LPDIRECT3DDEVICE9 pDevice)
: CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_fSpeed(0.f)
, m_bAnimationStart(false) 
{
}

CAniCromcruach::~CAniCromcruach(void)
{
	Release();
}

HRESULT CAniCromcruach::Initialize(void)
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
	m_pInfo->m_vScale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);

	m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(-45.f);

	return S_OK;
}

void CAniCromcruach::Update(void)
{
	if (g_AniSelect != 1)
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

void CAniCromcruach::Render(void)
{
	if (g_AniSelect != 1)
		return;

	SetConstantTable();

	m_pMesh->FrameMove(Engine::Get_TimeMgr()->GetTime());
	m_pMesh->RenderMesh_ForSDK(m_pShader->GetEffectHandle()
		, &m_pInfo->m_matWorld);
}

HRESULT CAniCromcruach::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"DynamicMesh_Cromcruach");
	m_pMesh = dynamic_cast<Engine::CDynamicMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"DynamicMesh", pComponent));

	pComponent = Engine::Get_ShaderMgr()->CloneShader(L"Shader_DynamicMesh");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));

	return S_OK;
}

void CAniCromcruach::KeyCheck(void)
{
}

void CAniCromcruach::SetConstantTable(void)
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

void CAniCromcruach::Release(void)
{
}

CAniCromcruach* CAniCromcruach::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CAniCromcruach*		pGameObject = new CAniCromcruach(pDevice);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}
