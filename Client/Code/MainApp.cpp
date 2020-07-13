#include "stdafx.h"
#include "MainApp.h"
#include "Export_Function.h"
#include "SceneSelector.h"
#include "VIBuffer.h"
#include "SoundMgr.h"

CMainApp::CMainApp(void)
: m_pGraphicDev(Engine::Get_GraphicDev())
, m_pManagement(Engine::Get_Management())
, m_pTimeMgr(Engine::Get_TimeMgr())
{
	srand(unsigned int(time(NULL)));
}

CMainApp::~CMainApp(void)
{
	Release();
}

HRESULT CMainApp::InitApp(void)
{
	HRESULT		hr = NULL;
	hr = m_pGraphicDev->InitGraphicDev(Engine::CGraphicDev::MODE_WIN, g_hWnd, WINCX, WINCY);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"디바이스 생성 실패");

	m_pDevice = m_pGraphicDev->GetDevice();
	NULL_CHECK_RETURN(m_pDevice, E_FAIL);

	FAILED_CHECK(Engine::Get_FontMgr()->AddFont(m_pDevice, L"바탕", 28, 20, FW_NORMAL));
	FAILED_CHECK(Engine::Get_FontMgr()->AddFont(m_pDevice, L"궁서", 38, 28, FW_MEDIUM));
	FAILED_CHECK(Engine::Get_FontMgr()->AddFont(m_pDevice, L"굴림", 48, 38, FW_HEAVY));

	hr = m_pManagement->InitManagement(m_pDevice);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"Management Init Failed");

	hr = Engine::Get_ResourceMgr()->ReserveContainerSize(RESOURCE_END);
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"리소스 컨테이너 할당 실패");

	hr = Engine::Get_ResourceMgr()->AddBuffer(m_pDevice, RESOURCE_STATIC
		, Engine::CVIBuffer::BUFFER_RCTEX, L"Buffer_RcTex");
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"Buffer_RcTex 생성 실패");

	hr = m_pManagement->SceneChange(CSceneSelector(CSceneSelector::SCENE_LOGO));
	FAILED_CHECK_RETURN_MSG(hr, E_FAIL, L"씬 전환 실패");

	hr = Engine::Get_Input()->InitInputDevice(g_hInst, g_hWnd);
	FAILED_CHECK(hr);
	m_pTimeMgr->InitTime();

	CSoundMgr::GetInstance()->Initialize();
	CSoundMgr::GetInstance()->LoadSoundFile();

	CSoundMgr::GetInstance()->PlayBGM(L"bgm_prologue_battle_inside_tower.wav", CHANNEL_BGM, 0.1f);
	ShowCursor(FALSE);
	return S_OK;
}

void CMainApp::Update(void)
{
	Engine::Get_Input()->SetInputState();
	m_pTimeMgr->SetTime();
	m_pManagement->Update();
}

void CMainApp::Render(void)
{
	m_pManagement->Render(m_pTimeMgr->GetTime());
}

void CMainApp::Release(void)
{
	//System
	Engine::Safe_Single_Destory(m_pGraphicDev);
	Engine::Safe_Single_Destory(m_pTimeMgr);
	Engine::Get_FontMgr()->DestroyInstance();
	Engine::Get_Input()->DestroyInstance();

	//Management
	Engine::Safe_Single_Destory(m_pManagement);
	Engine::Get_LightMgr()->DestroyInstance();
	Engine::Get_NaviMgr()->DestroyInstance();
	Engine::Get_ResourceMgr()->DestroyInstance();
	Engine::Get_ShaderMgr()->DestroyInstance();
	Engine::Get_CollisionMgr()->DestroyInstance();

	CSoundMgr::GetInstance()->DestroyInstance();
}

CMainApp* CMainApp::Create(void)
{
	CMainApp*		pMainApp = new CMainApp;
	if(FAILED(pMainApp->InitApp()))
	{
		delete pMainApp;
		pMainApp = NULL;
	}
	return pMainApp;
}