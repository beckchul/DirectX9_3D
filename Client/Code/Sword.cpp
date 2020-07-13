#include "stdafx.h"
#include "Sword.h"
#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Include.h"
#include "Texture.h"
#include "StaticMesh.h"
#include "Shader.h"
#include "RcTex.h"
#include "Collision_OBB.h"
#include "Stage.h"
#include "HitEffect.h"

CSword::CSword(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pMesh(NULL)
, m_pInfo(NULL)
, m_pmatWeaponRef(NULL)
, m_pmatArishaInfo(NULL)
, m_fColCount(1.f)
{
	D3DXMatrixIdentity(&m_CollisionMatrix);
	ZeroMemory(m_vTrailPoint, sizeof(D3DXVECTOR3) * POINT_END);
	m_bTrailRender[0] = false;
	m_bTrailRender[1] = false;
}

CSword::~CSword(void)
{

}

HRESULT CSword::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_pInfo->m_fAngle[Engine::ANGLE_Y] = D3DXToRadian(-90.f);
	m_pInfo->m_vScale = D3DXVECTOR3(2.f, 2.f, 2.f);

	return S_OK;
}

void CSword::Update(void)
{
	m_CollisionMatrix = m_pInfo->m_matWorld;
	Engine::CGameObject::Update();
	m_pInfo->m_matWorld = m_pInfo->m_matWorld * (*m_pmatWeaponRef) * (*m_pmatArishaInfo);
	StateCheck();
	TrailRenderCheck();
}

void CSword::Render(void)
{
	SetConstantTable(m_pShader->GetEffectHandle());
	m_pMesh->RenderMesh_ForSDK(m_pShader->GetEffectHandle()
		, &m_pInfo->m_matWorld);

	TrailRender();

	m_pCollisionOBB->Render(D3DCOLOR_ARGB(255, 0, 255, 255));
}

void CSword::TrailRender(void)
{
	D3DXVECTOR3 vPoint, vTemp;
	memcpy(&vPoint, &m_pInfo->m_matWorld.m[3][0], sizeof(D3DXVECTOR3));
	m_vTrailPoint[NEW_MIN_POINT] = ((Engine::CCollision_OBB*)m_pCollisionOBB)->GetMinPoint();
	vTemp = ((Engine::CCollision_OBB*)m_pCollisionOBB)->GetMaxPoint();
	m_vTrailPoint[NEW_MIN_POINT].x += 0.5f * vTemp.x;
	m_vTrailPoint[NEW_MIN_POINT].y += 0.5f * vTemp.y;
	m_vTrailPoint[NEW_MAX_POINT] = vPoint;//((Engine::CCollision_OBB*)m_pCollisionOBB)->GetMaxPoint();

	D3DXVec3TransformCoord(&m_vTrailPoint[NEW_MIN_POINT], &m_vTrailPoint[NEW_MIN_POINT], &m_pInfo->m_matWorld);
	//D3DXVec3TransformCoord(&m_vTrailPoint[NEW_MAX_POINT], &m_vTrailPoint[NEW_MAX_POINT], &m_pInfo->m_matWorld);

	D3DXVECTOR3 vMin;
	D3DXVECTOR3 vMax;

	D3DXVec3CatmullRom(&vMin
		, &m_vTrailPoint[CURCUR_MIN_POINT]
		, &m_vTrailPoint[CUR_MIN_POINT]
		, &m_vTrailPoint[NEW_MIN_POINT]
		, &(m_vTrailPoint[NEW_MIN_POINT]), 0.f);

	D3DXVec3CatmullRom(&vMax
		, &m_vTrailPoint[CURCUR_MAX_POINT]
		, &m_vTrailPoint[CUR_MAX_POINT]
		, &m_vTrailPoint[NEW_MAX_POINT]
		, &(m_vTrailPoint[NEW_MAX_POINT]), 1.f);


	D3DXMATRIX matWorldInverse;

	D3DXMatrixInverse(&matWorldInverse, NULL, &m_pInfo->m_matWorld);

	D3DXVec3TransformCoord(&vMin, &vMin, &matWorldInverse);
	D3DXVec3TransformCoord(&vMax, &vMax, &matWorldInverse);

	((Engine::CRcTex*)m_pBuffer)->SetEffectBuffer(&vMin, &vMax);

	m_vTrailPoint[CURCUR_MIN_POINT] = m_vTrailPoint[CUR_MIN_POINT];
	m_vTrailPoint[CURCUR_MAX_POINT] = m_vTrailPoint[CUR_MAX_POINT];

	m_vTrailPoint[CUR_MIN_POINT] = m_vTrailPoint[NEW_MIN_POINT];
	m_vTrailPoint[CUR_MAX_POINT] = m_vTrailPoint[NEW_MAX_POINT];

	if (m_bTrailRender[0])
	{
		m_pShader->GetEffectHandle()->SetTexture("g_BaseTexture", m_pTexture->GetTexture(0));

		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

		m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);

		m_pShader->BeginShader(1);
		m_pBuffer->Render(NULL);
		m_pShader->EndShader();
		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}

	if (m_bTrailRender[1])
	{
		m_pShader->GetEffectHandle()->SetTexture("g_BaseTexture", m_pTexture->GetTexture(1));

		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

		m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);

		m_pShader->BeginShader(1);
		m_pBuffer->Render(NULL);
		m_pShader->EndShader();

		m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}

	((Engine::CRcTex*)m_pBuffer)->ReSetBuffer();
}

HRESULT CSword::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Mesh
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"StaticMesh_Blade");
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

	//Sword Trail
	//Buffer
	pComponent = Engine::Get_ResourceMgr()->CloneResource(RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	//Texture
	pComponent = Engine::Get_ResourceMgr()->CloneResource(RESOURCE_STAGE, L"heroes_effect_blade_trail");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

CSword* CSword::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSword*		pGameObject = new CSword(pDevice);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}

void CSword::SetMatrixInfo(const D3DXMATRIX* pArishaInfo, const D3DXMATRIX* pWeaponRef)
{
	m_pmatWeaponRef = pWeaponRef;
	m_pmatArishaInfo = pArishaInfo;
}

void CSword::SetPlayerState(const PLAYERSTATE* pPlayerSteate)
{
	m_pPlayerState = pPlayerSteate;
}

HRESULT CSword::SetConstantTable(LPD3DXEFFECT	pEffect)
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

void CSword::StateCheck(void)
{
	switch ((*m_pPlayerState))
	{
	case PLAYER_ATTACK_1:
	case PLAYER_ATTACK_2:
	case PLAYER_ATTACK_3:
	case PLAYER_ATTACK_STRONG_1:
	case PLAYER_ATTACK_STRONG_4:
	case PLAYER_ATTACK_STRONG_5:
	case PLAYER_ATTACK_MB_STRONG_1:
	case PLAYER_ATTACK_MB_STRONG_2:
	case PLAYER_ATTACK_MB_STRONG_3:
	case PLAYER_ATTACK_MB_STRONG_4:
	case PLAYER_ATTACK_MB_STRONG_5:
	case PLAYER_ATTACK_MB_STRONG_6:
	case PLAYER_ATTACK_DOOMSAYER:
		//case PLAYER_ATTACK_RESONANCE:
		//case PLAYER_ATTACK_RUIN_BLACE:
		((Engine::CCollision_OBB*)m_pCollisionOBB)->SetCollisionState(true);
		return;
	}
	((Engine::CCollision_OBB*)m_pCollisionOBB)->SetCollisionState(false);
	m_ePrePlayerState = (*m_pPlayerState);
}

void CSword::TrailRenderCheck(void)
{
	switch ((*m_pPlayerState))
	{
	case PLAYER_ATTACK_1:
	case PLAYER_ATTACK_2:
	case PLAYER_ATTACK_3:
	case PLAYER_ATTACK_STRONG_1:
	case PLAYER_ATTACK_STRONG_4:
	case PLAYER_ATTACK_STRONG_5:
	//case PLAYER_ATTACK_RESONANCE:
	//case PLAYER_ATTACK_RUIN_BLACE:
		m_bTrailRender[0] = true;
		break;
	case PLAYER_ATTACK_MB_STRONG_1:
	case PLAYER_ATTACK_MB_STRONG_2:
	case PLAYER_ATTACK_MB_STRONG_3:
	case PLAYER_ATTACK_MB_STRONG_4:
	case PLAYER_ATTACK_MB_STRONG_5:
	case PLAYER_ATTACK_MB_STRONG_6:
	case PLAYER_ATTACK_DOOMSAYER:
		m_bTrailRender[0] = true;
		m_bTrailRender[1] = true;
		break;
	default:
		m_bTrailRender[0] = false;
		m_bTrailRender[1] = false;
		break;
	}
}
