#include "stdafx.h"
#include "AnimationMainApp.h"
#include "Export_Function.h"
#include "AniSceneSelector.h"

IMPLEMENT_SINGLETON(CAnimationMainApp)

CAnimationMainApp::CAnimationMainApp(void)
: m_pGraphicDev(Engine::Get_GraphicDev())
, m_pManagement(Engine::Get_Management())
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pTimeMgr(Engine::Get_TimeMgr())
{
	InitApp();
}

CAnimationMainApp::~CAnimationMainApp(void)
{
	Release();
}

HRESULT CAnimationMainApp::InitApp(void)
{

	m_pGraphicDev->InitGraphicDev(Engine::CGraphicDev::MODE_WIN, g_hWnd, WINCX, WINCY);

	m_pDevice = m_pGraphicDev->GetDevice();

	Engine::Get_FontMgr()->AddFont(m_pDevice, L"¹ÙÅÁ", 28, 20, FW_NORMAL);
	Engine::Get_FontMgr()->AddFont(m_pDevice, L"±Ã¼­", 38, 28, FW_MEDIUM);
	Engine::Get_FontMgr()->AddFont(m_pDevice, L"±¼¸²", 48, 38, FW_HEAVY);

	m_pManagement->InitManagement(m_pDevice);
	m_pResourceMgr->ReserveContainerSize(RESOURCE_END);

	m_pManagement->SceneChange(CAniSceneSelector(CAniSceneSelector::SCENE_TOOL));

	m_pTimeMgr->InitTime();
	return S_OK;
}

void CAnimationMainApp::Update(void)
{
	m_pTimeMgr->SetTime();
	m_pManagement->Update();
}

void CAnimationMainApp::Render(void)
{
	m_pManagement->Render(m_pTimeMgr->GetTime());
}

void CAnimationMainApp::Release(void)
{
	Engine::Safe_Single_Destory(m_pGraphicDev);
	Engine::Safe_Single_Destory(m_pTimeMgr);
	Engine::Get_FontMgr()->DestroyInstance();
	Engine::Get_Input()->DestroyInstance();
	Engine::Safe_Single_Destory(m_pManagement);
	Engine::Safe_Single_Destory(m_pResourceMgr);
	Engine::Get_LightMgr()->DestroyInstance();
	Engine::Get_ShaderMgr()->DestroyInstance();
}
