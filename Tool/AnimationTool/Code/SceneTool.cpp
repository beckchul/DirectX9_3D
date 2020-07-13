#include "stdafx.h"
#include "SceneTool.h"
#include "Export_Function.h"
#include "AniDynamicCamera.h"
#include "Layer.h"
#include "AniArisha.h"
#include "AniCromcruach.h"

CSceneTool::CSceneTool(LPDIRECT3DDEVICE9 pDevice)
: Engine::CScene(pDevice)
{
}

CSceneTool::~CSceneTool(void)
{
	Release();
}

HRESULT CSceneTool::InitScene(void)
{
	HRESULT hr = NULL;
	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"DynamicMesh_Arisha"
		, L"../../../Client/bin/Resources/Mesh/DynamicMesh/Mage/"
		, L"Mage.X");

	hr = Engine::Get_ResourceMgr()->AddMesh(m_pDevice
		, RESOURCE_STAGE
		, Engine::MESH_DYNAMIC
		, L"DynamicMesh_Cromcruach"
		, L"../../../Client/bin/Resources/Mesh/DynamicMesh/Cromcruach/"
		, L"Cromcruach.X");

	Engine::Get_ShaderMgr()->AddShaderFile(m_pDevice
		, L"Shader_Mesh"
		, L"../../../Reference/Headers/ShaderMesh.hpp");

	hr = Engine::Get_ShaderMgr()->AddShaderFile(m_pDevice
		, L"Shader_DynamicMesh"
		, L"../../../Reference/Headers/ShaderDynamic.hpp");
	FAILED_CHECK_RETURN(hr, E_FAIL);

	FAILED_CHECK_RETURN_MSG(Add_Environment_Layer(), E_FAIL, L"Environment_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_GameLogic_Layer(), E_FAIL, L"GameLogic_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_UI_Layer(), E_FAIL, L"Add_UI_Layer Add false");
	FAILED_CHECK_RETURN_MSG(Add_Light(), E_FAIL, L"Add_Light Add false");
	FAILED_CHECK_RETURN_MSG(Add_NaviMesh(), E_FAIL, L"Add_NaviMesh Add false");

	return S_OK;
}

void CSceneTool::Update(void)
{
	Engine::CScene::Update();
}

void CSceneTool::Render(void)
{

}

void CSceneTool::Release(void)
{

}

HRESULT CSceneTool::Add_Environment_Layer(void)
{
	return S_OK;
}

HRESULT CSceneTool::Add_GameLogic_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CAniArisha::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CArisha 积己 角菩");
	pLayer->AddObject(L"AniArisha", pGameObject);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);

	pGameObject = CAniCromcruach::Create(m_pDevice);
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"CAniCromcruach 积己 角菩");
	pLayer->AddObject(L"AniCromcruach", pGameObject);
	Engine::Get_Management()->AddRenderGroup(Engine::TYPE_NONEALPHA, pGameObject);


	m_mapLayer.insert(MAPLAYER::value_type(LAYER_GAMELOGIC, pLayer));
	return S_OK;
}

HRESULT CSceneTool::Add_UI_Layer(void)
{
	Engine::CLayer*		pLayer = Engine::CLayer::Create(m_pDevice);
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	Engine::CGameObject*	pGameObject = NULL;

	pGameObject = CAniDynamicCamera::Create(m_pDevice, &D3DXVECTOR3(0.f, 6.f, 4.f)
		, &D3DXVECTOR3(0.f, 0.f, 0.f));
	NULL_CHECK_RETURN_MSG(pGameObject, E_FAIL, L"Camera 积己 角菩");
	pLayer->AddObject(L"DynamicCamera", pGameObject);


	m_mapLayer.insert(MAPLAYER::value_type(LAYER_UI, pLayer));
	return S_OK;
}

HRESULT CSceneTool::Add_Light(void)
{

	D3DLIGHT9		Light;
	ZeroMemory(&Light, sizeof(D3DLIGHT9));
	Light.Type = D3DLIGHT_DIRECTIONAL;
	Light.Direction = D3DXVECTOR3(1.f, -1.f, -1.f);
	Light.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Light.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Light.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	Light.Attenuation0 = 1.f;

	Engine::Get_LightMgr()->AddLight(m_pDevice, &Light, 0);

	return S_OK;
}

HRESULT CSceneTool::Add_NaviMesh(void)
{
	return S_OK;
}

CSceneTool * CSceneTool::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CSceneTool* pStage = new CSceneTool(pDevice);

	if (FAILED(pStage->InitScene()))
	{
		Engine::Safe_Delete(pStage);
	}

	return pStage;
}