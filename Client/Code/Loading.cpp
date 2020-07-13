#include "stdafx.h"
#include "Loading.h"

#include "Export_Function.h"
#include "Include.h"
#include <process.h>

CLoading::CLoading(LOADINGID eLoadID)
: m_pDevice(Engine::Get_GraphicDev()->GetDevice())
, m_eLoadID(eLoadID)
, m_hThread(NULL)
, m_bComplete(false)
{
	ZeroMemory(&m_CSKey, sizeof(CRITICAL_SECTION));
	ZeroMemory(m_szLoadingMessage, sizeof(TCHAR) * 256);
}

CLoading::~CLoading(void)
{
	Release();
}

HRESULT CLoading::InitLoading(void)
{
	//InitializeCriticalSection(&m_CSKey);
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, LoadingFunction, this, 0, NULL);
	NULL_CHECK_RETURN(m_hThread, E_FAIL);
	return S_OK;
}

HRESULT CLoading::StageLoading(void)
{
	HRESULT hr = NULL;
	lstrcpy(m_szLoadingMessage, L"버퍼 로딩중...");
	//Buffer
	//hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice
	//	, RESOURCE_STAGE
	//	, Engine::CVIBuffer::BUFFER_TERRAIN
	//	, L"Buffer_TerrainTex"
	//	, VTXCNTX, VTXCNTZ, VTXITV);
	//FAILED_CHECK_RETURN(hr, E_FAIL);

	//hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice
	//	, RESOURCE_STAGE
	//	, Engine::CVIBuffer::BUFFER_CUBETEX
	//	, L"Buffer_CubeTex");
	//FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadingMessage, L"메쉬 로딩중..");
	//Mesh

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"DynamicMesh_Arisha"
		, L"../bin/Resources/Mesh/DynamicMesh/Arisha/"
		, L"Arisha.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"DynamicMesh_Cromcruach"
		, L"../bin/Resources/Mesh/DynamicMesh/Cromcruach/"
		, L"Cromcruach.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadingMessage, L"");

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"StaticMesh_MapModel"
		, L"../bin/Resources/Mesh/StaticMesh/Map/"
		, L"MapModel.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"StaticMesh_Blade"
		, L"../bin/Resources/Mesh/StaticMesh/Blade/"
		, L"Blade.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"StaticMesh_CollisionSphere"
		, L"../bin/Resources/Mesh/StaticMesh/CollisionSphere/"
		, L"CollisionSphere.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_STATIC
		, L"StaticMesh_Staff"
		, L"../bin/Resources/Mesh/StaticMesh/Staff/"
		, L"Staff.X");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadingMessage, L"텍스쳐 로딩중...");
	//Texture
	

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"heroes_effect_blade_trail"
		, L"../bin/Resources/Texture/heroes_effect_blade_trail%d.tga", 2);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"HitEffect"
		, L"../bin/Resources/Texture/HitEffect%d.png", 2);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"RuinEffect"
		, L"../bin/Resources/Texture/Skill/Ruin/0.png");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"ManaRegion"
		, L"../bin/Resources/Texture/Skill/ManaRegion/0.png");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"SmashEffect"
		, L"../bin/Resources/Texture/SmashEffect0.png");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"ResonanceBoomBegin"
		, L"../bin/Resources/Texture/Skill/Resonance/BoomBegin.png");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Explosion"
		, L"../bin/Resources/Texture/Effect0.jpg");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Resonance"
		, L"../bin/Resources/Texture/Skill/Resonance/%d.png", 3);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"ManaHand"
		, L"../bin/Resources/Texture/Skill/RordOfMana/Hand.jpg");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"ManaHand2"
		, L"../bin/Resources/Texture/Skill/RordOfMana/Hand2.png");
	FAILED_CHECK_RETURN(hr, E_FAIL);
	
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"ManaScreen"
		, L"../bin/Resources/Texture/Skill/RordOfMana/ManaScreen.png");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"ManaMagic"
		, L"../bin/Resources/Texture/Skill/RordOfMana/Magic%d.png", 2);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"ResonanceBoomEnd"
		, L"../bin/Resources/Texture/Skill/Boom/Boom%d.png", 130);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"PunchEffect"
		, L"../bin/Resources/Texture/MonSkill/Punch/0.jpg");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Lightning"
		, L"../bin/Resources/Texture/MonSkill/Lightning/%d.png", 2);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"LightningAura"
		, L"../bin/Resources/Texture/MonSkill/Lightning/Aura.tga");
	FAILED_CHECK_RETURN(hr, E_FAIL);
	
	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"StartUI"
		, L"../bin/Resources/Texture/NameCard.png");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"PlayerHpBar"
		, L"../bin/Resources/Texture/HpBar/PlayerHpBar%d.png", 2);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"MonsterHpBar"
		, L"../bin/Resources/Texture/HpBar/MonsterHpBar%d.png", 2);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"DamageFont"
		, L"../bin/Resources/Texture/DamageFont.png");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"DamageScreen"
		, L"../bin/Resources/Texture/DamageScreen.png");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"Skillicon"
		, L"../bin/Resources/Texture/icon/skill_icon%d.tga", 6);
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = Engine::Get_ResourceMgr()->AddTexture(m_pDevice
		, RESOURCE_STAGE
		, Engine::TEXTURE_NORMAL
		, L"EndUI"
		, L"../bin/Resources/Texture/EndUI.jpg");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	lstrcpy(m_szLoadingMessage, L"로딩완료...");

	m_bComplete = true;
	return S_OK;
}

HRESULT CLoading::StageLoading1(void)
{
	return S_OK;
}

HRESULT CLoading::StageLoading2(void)
{
	return S_OK;
}

CLoading* CLoading::Create(LOADINGID eLoadID)
{
	CLoading* pLoad = new CLoading(eLoadID);
	if(FAILED(pLoad->InitLoading()))
		Engine::Safe_Delete(pLoad);

	return pLoad;
}

void CLoading::Release(void)
{
	//DeleteCriticalSection(&m_CSKey);
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
}

UINT WINAPI CLoading::LoadingFunction(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	//EnterCriticalSection(&pLoading->m_CSKey);

	switch(pLoading->m_eLoadID)
	{
	case CLoading::LOADING_STAGE:
		pLoading->StageLoading();
		break;

	case CLoading::LOADING_STAGE1:
		pLoading->StageLoading1();
		break;

	case CLoading::LOADING_STAGE2:
		pLoading->StageLoading2();
		break;
	}

	//LeaveCriticalSection(&pLoading->m_CSKey);
	return 0;
}

