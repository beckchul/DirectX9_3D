#include "stdafx.h"
#include "Stone.h"
#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Stage.h"
#include "Mesh.h"
#include "CollisionMgr.h"
#include "Shader.h"
#include "StaticMesh.h"

CStone::CStone(LPDIRECT3DDEVICE9 pDevice)
: CLandObject(pDevice)
, m_bCollision(false)
, m_vEffectPos(300.f, -0.f, 300.f)
, m_fSize(7.f)
{

}

CStone::~CStone(void)
{

}

HRESULT CStone::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_pInfo->m_vScale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);

	return S_OK;
}

void CStone::Update(void)
{
	Engine::CGameObject::Update();
	Engine::CGameObject::Compute_ViewZ(&m_pInfo->m_vPos);
}

void CStone::Render(void)
{
	LPD3DXEFFECT	pEffect = m_pShader->GetEffectHandle();
	HRESULT hr = SetConstantTable(pEffect);
	FAILED_CHECK_RETURN(hr, );

	m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(125, 255, 255, 255));

	m_pMesh->RenderMesh_ForSDK(pEffect, &m_pInfo->m_matWorld);
}

void CStone::SetEffexct(D3DXVECTOR3 _vEffectPos, float _fSize)
{
	m_vEffectPos = _vEffectPos;
	m_fSize = _fSize;
}

HRESULT CStone::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = Engine::Get_ResourceMgr()->CloneResource(RESOURCE_STAGE, L"StaticMesh_MapModel");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->CloneShader(L"Shader_Map");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));


	pComponent = Engine::Get_ResourceMgr()->CloneResource(RESOURCE_STAGE, L"LightningAura");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));
	return S_OK;
}

HRESULT CStone::SetConstantTable(LPD3DXEFFECT pEffect)
{
	NULL_CHECK_RETURN(pEffect, E_FAIL);

	const D3DLIGHT9* pLightInfo = Engine::Get_LightMgr()->GetLightInfo();
	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (D3DXVECTOR4*)(&pLightInfo->Diffuse));
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)(&pLightInfo->Ambient));
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)(&pLightInfo->Specular));
	
	pEffect->SetVector("g_vBrushPos", &D3DXVECTOR4(m_vEffectPos.x, m_vEffectPos.y, m_vEffectPos.z, 1.f));
	pEffect->SetFloat("g_fBrushSize", m_fSize);
	
	pEffect->SetTexture("g_BrushTexture", m_pTexture->GetTexture(0));

	D3DXMATRIX		matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPos", (D3DXVECTOR4*)&matView.m[3][0]);

	if (g_bBlackScreen)
	{
		pEffect->SetFloat("g_fBlack", 0.f);
		pEffect->SetFloat("g_fColor", g_fWorldColor);
	}
	else
		pEffect->SetFloat("g_fBlack", 1.f);

	return S_OK;
}

CStone* CStone::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStone*		pGameObject = new CStone(pDevice);
	if(FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}
