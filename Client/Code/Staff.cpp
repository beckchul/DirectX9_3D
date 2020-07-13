#include "stdafx.h"
#include "Staff.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Shader.h"
#include "Collision_OBB.h"

CStaff::CStaff(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pMesh(NULL)
, m_pInfo(NULL)
, m_pmatWeaponRef(NULL)
, m_pmatCromcruachInfo(NULL)
{
	D3DXMatrixIdentity(&m_CollisionMatrix);
}

CStaff::~CStaff(void)
{

}

HRESULT CStaff::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	return S_OK;
}

void CStaff::Update(void)
{
	m_CollisionMatrix = m_pInfo->m_matWorld;
	Engine::CGameObject::Update();
	m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pmatWeaponRef) * (*m_pmatCromcruachInfo);
	StateCheck();
}

void CStaff::Render(void)
{
	SetConstantTable(m_pShader->GetEffectHandle());
	m_pMesh->RenderMesh_ForSDK(m_pShader->GetEffectHandle()
		, &m_pInfo->m_matWorld);


	//if (((Engine::CCollision_OBB*)m_pCollisionOBB)->GetCollisionState())
	//	m_pCollisionOBB->Render(D3DCOLOR_ARGB(255, 255, 0, 0));
	//else
	//	m_pCollisionOBB->Render(D3DCOLOR_ARGB(255, 0, 255, 0));
}

HRESULT CStaff::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"StaticMesh_Staff");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	//Shader
	pComponent = Engine::Get_ShaderMgr()->CloneShader(L"Shader_Mesh");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));

	//CollisionOBB
	pComponent = Engine::Get_CollisionMgr()->CloneColObject(Engine::CCollisionMgr::COLLISION_OBB);
	m_pCollisionOBB = dynamic_cast<Engine::CCollision_OBB*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Collision_OBB", pComponent));
	m_pCollisionOBB->SetColInfo(&m_CollisionMatrix, m_pMesh);
	return S_OK;
}

CStaff* CStaff::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStaff*		pGameObject = new CStaff(pDevice);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CStaff::SetMatrixInfo(const D3DXMATRIX* pCromcruachInfo, const D3DXMATRIX* pWeaponRef)
{
	m_pmatWeaponRef = pWeaponRef;
	m_pmatCromcruachInfo = pCromcruachInfo;
}

HRESULT CStaff::SetConstantTable(LPD3DXEFFECT	pEffect)
{
	NULL_CHECK_RETURN(pEffect, E_FAIL);

	const D3DLIGHT9* pLightInfo = Engine::Get_LightMgr()->GetLightInfo();
	pEffect->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	pEffect->SetVector("g_vLightDiffuse", (D3DXVECTOR4*)(&pLightInfo->Diffuse));
	pEffect->SetVector("g_vLightAmbient", (D3DXVECTOR4*)(&pLightInfo->Ambient));
	pEffect->SetVector("g_vLightSpecular", (D3DXVECTOR4*)(&pLightInfo->Specular));

	D3DXMATRIX		matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	D3DXMatrixInverse(&matView, NULL, &matView);
	pEffect->SetVector("g_vCamPos", (D3DXVECTOR4*)&matView.m[3][0]);

	return S_OK;
}

void CStaff::StateCheck(void)
{
	switch ((*m_pMonsterState))
	{
	case MONSTER_IDLE:
	case MONSTER_DASH_BEGIN:
	case MONSTER_DASH_END:
	case MONSTER_ATTACK_MAGIC_SQUARE:
	case MONSTER_ATTACK_MAGIC_SQUARE_DURING1:
	case MONSTER_ATTACK_MAGIC_SQUARE_DURING2:
	case MONSTER_ATTACK_JUMP_SMASH1:
	case MONSTER_ATTACK_FINAL_BEGIN:
	case MONSTER_ATTACK_FINAL_END:
		((Engine::CCollision_OBB*)m_pCollisionOBB)->SetCollisionState(false);
		return;
	}
	((Engine::CCollision_OBB*)m_pCollisionOBB)->SetCollisionState(true);
}
