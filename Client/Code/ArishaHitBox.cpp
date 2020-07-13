#include "stdafx.h"
#include "ArishaHitBox.h"
#include "Stage.h"
#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Collision_OBB.h"

#include "Shader.h"

CArishaHitBox::CArishaHitBox(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pMesh(NULL)
, m_pInfo(NULL)
, m_bCheckCollision(false) 
, m_vCollisionPos(0.f, 0.f, 0.f)
{
}

CArishaHitBox::~CArishaHitBox(void)
{
	
}

HRESULT CArishaHitBox::Initialize(void)
{
	FAILED_CHECK(AddComponent());
	memcpy(&m_pInfo->m_matWorld.m[3][0], &m_vCollisionPos, sizeof(D3DXVECTOR3));
	m_pInfo->m_vScale = D3DXVECTOR3(30.f, 30.f, 30.f);
	return S_OK;
}


void CArishaHitBox::SetColScale(D3DXVECTOR3 _vScale)
{
	m_pInfo->m_vScale = _vScale;
}

void CArishaHitBox::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDir, &D3DXVECTOR3(0.f, 0.f, 1.f), &m_pInfo->m_matWorld);
	m_pInfo->m_vPos = m_vCollisionPos;
	Engine::CGameObject::Update();
	m_pCollisionOBB->SetCollisionState(m_bCheckCollision);
}

void CArishaHitBox::Render(void)
{
	if (!m_bCheckCollision)
		return;
	const D3DLIGHT9* pLightInfo = Engine::Get_LightMgr()->GetLightInfo();
	m_pShader->GetEffectHandle()->SetVector("g_vLightDir", &D3DXVECTOR4(pLightInfo->Direction, 0.f));
	m_pShader->GetEffectHandle()->SetVector("g_vLightDiffuse", (D3DXVECTOR4*)(&pLightInfo->Diffuse));
	m_pShader->GetEffectHandle()->SetVector("g_vLightAmbient", (D3DXVECTOR4*)(&pLightInfo->Ambient));
	m_pShader->GetEffectHandle()->SetVector("g_vLightSpecular", (D3DXVECTOR4*)(&pLightInfo->Specular));

	D3DXMATRIX		matView, matProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pShader->GetEffectHandle()->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);
	m_pShader->GetEffectHandle()->SetMatrix("g_matView", &matView);
	m_pShader->GetEffectHandle()->SetMatrix("g_matProj", &matProj);

	D3DXMatrixInverse(&matView, NULL, &matView);
	m_pShader->GetEffectHandle()->SetVector("g_vCamPos", (D3DXVECTOR4*)&matView.m[3][0]);
}


HRESULT CArishaHitBox::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"StaticMesh_CollisionSphere");
	m_pMesh = dynamic_cast<Engine::CStaticMesh*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"StaticMesh", pComponent));

	//CollisionOBB
	pComponent = Engine::Get_CollisionMgr()->CloneColObject(Engine::CCollisionMgr::COLLISION_OBB);
	m_pCollisionOBB = dynamic_cast<Engine::CCollision_OBB*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Collision_OBB", pComponent));
	m_pCollisionOBB->SetColInfo(&m_pInfo->m_matWorld, m_pMesh);



	//Shader
	pComponent = Engine::Get_ShaderMgr()->CloneShader(L"Shader_Mesh");
	m_pShader = dynamic_cast<Engine::CShader*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Shader", pComponent));

	return S_OK;
}

CArishaHitBox * CArishaHitBox::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CArishaHitBox*		pGameObject = new CArishaHitBox(pDevice);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}
