#include "stdafx.h"
#include "ArishaSphere.h"
#include "Stage.h"
#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Shader.h"
#include "Collision_OBB.h"

CArishaSphere::CArishaSphere(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pMesh(NULL)
, m_pInfo(NULL)
, m_pmatSphereRef(NULL)
, m_pmatArishaInfo(NULL)
, m_bRender(TRUE)
, m_bCheckCollision(false)
{
	D3DXMatrixIdentity(&m_CollisionMatrix);
}

CArishaSphere::~CArishaSphere(void)
{

}

HRESULT CArishaSphere::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_vScale = D3DXVECTOR3(1.5f, 1.5f, 1.5f);
	return S_OK;
}

void CArishaSphere::Update(void)
{
	m_CollisionMatrix = m_pInfo->m_matWorld;
	Engine::CGameObject::Update();
	m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pmatSphereRef) * (*m_pmatArishaInfo);

	m_bCheckCollision = CollisionOBB();
}

void CArishaSphere::Render(void)
{
	if (m_bRender)
	{
		m_pCollisionOBB->Render(D3DCOLOR_ARGB(255, 0, 255, 255));
	}
}

HRESULT CArishaSphere::AddComponent(void)
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

CArishaSphere* CArishaSphere::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CArishaSphere*		pGameObject = new CArishaSphere(pDevice);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CArishaSphere::SetMatrixInfo(const D3DXMATRIX* pArishaInfo, const D3DXMATRIX* pSphereRef)
{
	m_pmatSphereRef = pSphereRef;
	m_pmatArishaInfo = pArishaInfo;
}

HRESULT CArishaSphere::SetConstantTable(LPD3DXEFFECT	pEffect)
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

bool CArishaSphere::CollisionOBB(void)
{
	const Engine::CComponent*	pCollider = NULL;
	pCollider = Engine::Get_Management()->GetComponent(
		CStage::LAYER_GAMELOGIC, L"CromcruachSphere", L"Collision_OBB");

	if (pCollider == NULL)
		return false;

	if (m_pCollisionOBB->CheckCollision((const Engine::CCollision_OBB*)pCollider))
		return true;

	pCollider = Engine::Get_Management()->GetComponent(
		CStage::LAYER_GAMELOGIC, L"Staff", L"Collision_OBB");

	if (pCollider == NULL)
		return false;

	if (m_pCollisionOBB->CheckCollision((const Engine::CCollision_OBB*)pCollider))
		return true;


	pCollider = Engine::Get_Management()->GetComponent(
			CStage::LAYER_GAMELOGIC, L"CromcruachHitBox", L"Collision_OBB");

	if (pCollider == NULL)
		return false;

	if (m_pCollisionOBB->CheckCollision((const Engine::CCollision_OBB*)pCollider))
		return true;

	return false;
}
