#include "stdafx.h"
#include "Stage.h"
#include "Collision_OBB.h"
#include "Include.h"
#include "Export_Function.h"
#include "Layer.h"
#include "Terrain.h"
#include "DynamicCamera.h"
#include "Stone.h"
#include "UI.h"
#include "SkyBox.h"
#include "Arisha.h"
#include "Sword.h"
#include "Cromcruach.h"
#include "ArishaSphere.h"
#include "Staff.h"
#include "HitEffect.h"
#include "CromcruachSphere.h"
#include "StaticCamera.h"
#include "ExplosionEffect.h"
#include "SmashEffect.h"
#include "RuinEffect.h"
#include "PunchEffect.h"
#include "ManaRegionEffect.h"
#include "LightningEffect.h"
#include "ResonanceEffect.h"
#include "ResonanceSubEffect.h"
#include "ResonanceBoomEffect.h"
#include "ArishaHitBox.h"
#include "PlayerHpBar.h"
#include "MonsterHpBar.h"
#include "CromcruachHitBox.h"
#include "DamageFont.h"
#include "DamageScreen.h"
#include "SoundMgr.h"
#include "SkillIcon.h"
#include "ManaHandEffect.h"
#include "ManaMagicEffect.h"
#include "ManaHandEffect2.h"
#include "ManaBoomEffect.h"
#include "EndUI.h"

CStage::CStage(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
{

}

CStage::~CStage(void)
{
	Release();
}

HRESULT CStage::InitScene(void)
{
	HRESULT		hr = NULL;

	hr = Engine::Get_ShaderMgr()->AddShaderFile(m_pDevice
		, L"Shader_Mesh"
		, L"../../Reference/Headers/ShaderMesh.hpp");
	FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ShaderMgr()->AddShaderFile(m_pDevice
		, L"Shader_Map"
		, L"../../Reference/Headers/ShaderMap.hpp");
	FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = Engine::Get_ShaderMgr()->AddShaderFile(m_pDevice
		, L"Shader_DynamicMesh"
		, L"../../Reference/Headers/ShaderDynamic.hpp");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ShaderMgr()->AddShaderFile(m_pDevice
		, L"ShaderUV"
		, L"../../Reference/Headers/ShaderUV.hpp");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_CollisionMgr()->InitCollison(m_pDevice);
	hr = Engine::Get_CollisionMgr()->AddColObject(Engine::CCollisionMgr::COLLISION_OBB);
	hr = Engine::Get_CollisionMgr()->AddColObject(Engine::CCollisionMgr::COLLISION_AABB);
	FAILED_CHECK(hr);

	FAILED_CHECK_RETURN_MSG(Add_Environment_Layer(), E_FAIL, L"Environment_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_GameLogic_Layer(), E_FAIL, L"GameLogic_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_UI_Layer(), E_FAIL, L"Add_UI_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_Light(), E_FAIL, L"Add_Light Add false");
	FAILED_CHECK_RETURN_MSG(Add_NaviMesh(), E_FAIL, L"Add_NaviMesh Add false");

	CSoundMgr::GetInstance()->PlayBGM(L"BGM1.mp3", CHANNEL_BGM, 0.35f);

	return S_OK;
}

void CStage::Update(void)
{
	Engine::CScene::Update();
}

void CStage::Render(void)
{
	//Engine::Get_NaviMgr()->Render_NaviMesh();
}

HRESULT CStage::Add_Environment_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_ENVIRONMENT, pLayer));
	return S_OK;
}

HRESULT CStage::Add_GameLogic_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CStone::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CStone 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"Stone", pGameObject);

	
	pGameObject = CArisha::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CArisha 积己 角菩");
	pLayer->AddObject(L"Arisha", pGameObject);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	
	Engine::CGameObject* pTempObject = pGameObject;

	const D3DXMATRIX* pWeaponRef = ((CArisha*)pTempObject)->GetWeaponRef();
	const D3DXMATRIX* pPlayerInfo = ((CArisha*)pTempObject)->GetArishaInfo();

	pGameObject = CSword::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CSword 积己 角菩");
	pLayer->AddObject(L"Sword", pGameObject);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	((CSword*)pGameObject)->SetMatrixInfo(pPlayerInfo, pWeaponRef);
	((CSword*)pGameObject)->SetPlayerState(((CArisha*)pTempObject)->GetPlayerState());
	Engine::CGameObject* pTempSword = pGameObject;

	pGameObject = CHitEffect::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CEffect 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	pLayer->AddObject(L"HitEffect", pGameObject);
	
	for (int i = 0; i < 8; ++i)
	{
		const D3DXMATRIX* pSphereRef = ((CArisha*)pTempObject)->GetCollisionRef(i);

		pGameObject = CArishaSphere::Create(m_pDevice);
		NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CArishaSphere 积己 角菩");
		pLayer->AddObject(L"ArishaSphere", pGameObject);
		Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
		((CArishaSphere*)pGameObject)->SetMatrixInfo(pPlayerInfo, pSphereRef);
		((CArisha*)pTempObject)->SetCheckCollision(((CArishaSphere*)pGameObject)->GetCheckCollision(), i);
	}

	pGameObject = CArishaHitBox::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CArishaHitBox 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"ArishaHitBox", pGameObject);

	pGameObject = CCromcruachHitBox::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CCromcruachHitBox 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"CromcruachHitBox", pGameObject);

	pGameObject = CExplosionEffect::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CExplosionEffect 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	pLayer->AddObject(L"ExplosionEffect", pGameObject);

	pGameObject = CSmashEffect::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CSmashEffect 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	pLayer->AddObject(L"SmashEffect", pGameObject);

	pGameObject = CRuinEffect::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CRuinEffect 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	pLayer->AddObject(L"RuinEffect", pGameObject);

	pGameObject = CManaRegionEffect::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CManaRegionEffect 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	pLayer->AddObject(L"ManaRegionEffect", pGameObject);

	pGameObject = CResonanceEffect::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CResonanceEffect 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	pLayer->AddObject(L"ResonanceEffect", pGameObject);

	pGameObject = CResonanceSubEffect::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CResonanceSubEffect 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	pLayer->AddObject(L"ResonanceSubEffect", pGameObject);

	pGameObject = CResonanceBoomEffect::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CResonanceBoomEffect 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	pLayer->AddObject(L"ResonanceBoomEffect", pGameObject);

	pGameObject = CManaHandEffect::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CManaHandEffect 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	pLayer->AddObject(L"ManaHandEffect", pGameObject);

	pGameObject = CManaHandEffect2::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CManaHandEffect2 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	pLayer->AddObject(L"ManaHandEffect2", pGameObject);

	pGameObject = CManaMagicEffect::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CManaMagicEffect 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	pLayer->AddObject(L"ManaMagicEffect", pGameObject);

	pGameObject = CManaBoomEffect::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CManaBoomEffect 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
	pLayer->AddObject(L"ManaBoomEffect", pGameObject);

	for (int i = 0; i < 5; ++i)
	{
		pGameObject = CLightningEffect::Create(m_pDevice);
		NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CLightningEffect 积己 角菩");
		Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
		pLayer->AddObject(L"LightningEffect", pGameObject);
	}
	
	// Monster
	pGameObject = CCromcruach::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CCromcruach 积己 角菩");
	pLayer->AddObject(L"Cromcruach", pGameObject);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);

	Engine::CGameObject* pTempMonsterObject = pGameObject;

	((CCromcruach*)pTempMonsterObject)->SetPlayerPos(((CArisha*)pTempObject)->GetPlayerPos());
	const D3DXMATRIX* pStaffRef = ((CCromcruach*)pTempMonsterObject)->GetWeaponRef();
	const D3DXMATRIX* pMonsterInfo = ((CCromcruach*)pTempMonsterObject)->GetCromcruachInfo();

	for (int i = 0; i < 13; ++i)
	{
		const D3DXMATRIX* pSphereRef = ((CCromcruach*)pTempMonsterObject)->GetCollisionRef(i);

		pGameObject = CCromcruachSphere::Create(m_pDevice);
		NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CCromcruachSphere 积己 角菩");
		pLayer->AddObject(L"CromcruachSphere", pGameObject);
		Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
		((CCromcruachSphere*)pGameObject)->SetMatrixInfo(pMonsterInfo, pSphereRef);
		((CCromcruachSphere*)pGameObject)->SetMonsterState(((CCromcruach*)pTempMonsterObject)->GetMonsterState());
		((CCromcruach*)pTempMonsterObject)->SetCheckCollision(((CCromcruachSphere*)pGameObject)->GetCheckCollision(), i);

		if (i == 10)
		{
			pGameObject = CPunchEffect::Create(m_pDevice);
			NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CPunchEffect 积己 角菩");
			Engine::Get_Management()->AddRenderGroup(Engine::TYPE_ALPHA, pGameObject);
			pLayer->AddObject(L"PunchEffect", pGameObject);
			D3DXVECTOR3 vPos;
			memcpy(&vPos, &(*pSphereRef).m[3][0], sizeof(D3DXVECTOR3));
		}
	}

	pGameObject = CStaff::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CStaff 积己 角菩");
	pLayer->AddObject(L"Staff", pGameObject);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	((CStaff*)pGameObject)->SetMatrixInfo(pMonsterInfo, pStaffRef);
	((CStaff*)pGameObject)->SetMonsterState(((CCromcruach*)pTempMonsterObject)->GetMonsterState());

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));
	return S_OK;
}

HRESULT CStage::Add_UI_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;

	//pGameObject = CDynamicCamera::Create(m_pDevice, &D3DXVECTOR3(0.f, 5.f, -10.f)
	//	, &D3DXVECTOR3(0.f, 0.f, 0.f));
	//NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Camera 积己 角菩");
	//pLayer->AddObject(L"DynamicCamera" , pGameObject);

	MAPLAYER::iterator iter = m_mapLayer.find(LAYER_GAMELOGIC);
	const Engine::CComponent* pComponent = NULL;
	pComponent = iter->second->GetComponent(L"Arisha", L"Transform");

	pGameObject = CStaticCamera::Create(m_pDevice, (const Engine::CTransform*)pComponent, 
		&D3DXVECTOR3(-35.f, 2.4f, 0.f), &D3DXVECTOR3(-25.f, 0.4f, 0.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	pLayer->AddObject(L"StaticCamera", pGameObject);

	pGameObject = CUI::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CUI 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_UI, pGameObject);
	pLayer->AddObject(L"UI" , pGameObject);

	pGameObject = CPlayerHpBar::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CPlayerHpBar 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_UI, pGameObject);
	pLayer->AddObject(L"PlayerHpBar", pGameObject);

	pGameObject = CMonsterHpBar::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CMonsterHpBar 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_UI, pGameObject);
	pLayer->AddObject(L"MonsterHpBar", pGameObject);

	pGameObject = CDamageFont::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CDamageFont 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_UI, pGameObject);
	pLayer->AddObject(L"DamageFont", pGameObject);

	pGameObject = CDamageScreen::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CDamageScreen 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_UI, pGameObject);
	pLayer->AddObject(L"DamageScreen", pGameObject);

	pGameObject = CEndUI::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CEndUI 积己 角菩");
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);
	pLayer->AddObject(L"EndUI", pGameObject);


	for (int i = 0; i < 6; ++i)
	{
		pGameObject = CSkillIcon::Create(m_pDevice, i);
		NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CSkillIcon 积己 角菩");
		Engine::Get_Management()->AddRenderGroup(Engine::TYPE_UI, pGameObject);
		pLayer->AddObject(L"SkillIcon", pGameObject);
	}
	

	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));
	return S_OK;
}

void CStage::Release(void)
{
	Engine::Get_LightMgr()->DestroyInstance();
	Engine::Get_NaviMgr()->DestroyInstance();
}

CStage* CStage::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CStage* pStage = new CStage(pDevice);

	if(FAILED(pStage->InitScene()))
	{
		Engine::Safe_Delete(pStage);
	}

	return pStage;
}

HRESULT CStage::Add_Light(void)
{
	HRESULT hr = NULL;
	D3DLIGHT9		Light;
	ZeroMemory(&Light, sizeof(D3DLIGHT9));
	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Direction = D3DXVECTOR3(1.f, -1.f, 1.f);
	Light.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Light.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Light.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	/*Light.Attenuation0 = 1.f;*/

	hr = Engine::Get_LightMgr()->AddLight(m_pDevice, &Light, 0);
	FAILED_CHECK(hr);
	return S_OK;
}

HRESULT CStage::Add_NaviMesh(void)
{
	Engine::Get_NaviMgr()->ReserveCellSize(m_pDevice, 6);
	HRESULT		hr = NULL;
	D3DXVECTOR3		vPoint[3];

	vPoint[0] = D3DXVECTOR3(0.f, 0.4f, 37.f);
	vPoint[1] = D3DXVECTOR3(32.f, 0.4f, 19.f);
	vPoint[2] = D3DXVECTOR3(0.f, 0.6f, 0.f);
	hr = Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(32.f, 0.4f, 19.f);
	vPoint[1] = D3DXVECTOR3(32.f, 0.4f, -19.f);
	vPoint[2] = D3DXVECTOR3(0.f, 0.6f, 0.f);
	hr = Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(0.f, 0.6f, 0.f);
	vPoint[1] = D3DXVECTOR3(32.f, 0.4f, -19.f);
	vPoint[2] = D3DXVECTOR3(0.f, 0.4f, -37.f);
	hr = Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-32.f, 0.4f, -19.f);
	vPoint[1] = D3DXVECTOR3(0.f, 0.6f, 0.f);
	vPoint[2] = D3DXVECTOR3(0.f, 0.4f, -37.f);
	hr = Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-32.f, 0.4f, 19.f);
	vPoint[1] = D3DXVECTOR3(0.f, 0.6f, 0.f);
	vPoint[2] = D3DXVECTOR3(-32.f, 0.4f, -19.f);
	hr = Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	vPoint[0] = D3DXVECTOR3(-32.f, 0.4f, 19.f);
	vPoint[1] = D3DXVECTOR3(0.f, 0.4f, 37.f);
	vPoint[2] = D3DXVECTOR3(0.f, 0.6f, 0.f);
	hr = Engine::Get_NaviMgr()->AddCell(&vPoint[0], &vPoint[1], &vPoint[2]);
	FAILED_CHECK(hr);

	Engine::Get_NaviMgr()->LinkCell();
	return S_OK;
}
