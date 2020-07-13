#include "Management.h"

#include "Renderer.h"
#include "Scene.h"

IMPLEMENT_SINGLETON(Engine::CManagement)

Engine::CManagement::CManagement(void)
: m_pScene(NULL)
, m_pRenderer(NULL)
, m_pDevice(NULL)
{

}

Engine::CManagement::~CManagement(void)
{
	Release();
}

HRESULT Engine::CManagement::InitManagement(LPDIRECT3DDEVICE9 pDevice)
{
	m_pDevice = pDevice;

	m_pRenderer = CRenderer::Create(pDevice);
	NULL_CHECK_RETURN_MSG(m_pRenderer, E_FAIL, L"Renderer Create Failed");

	return S_OK;
}

void Engine::CManagement::Update(void)
{
	if(m_pScene != NULL)
		m_pScene->Update();
}

void Engine::CManagement::Render(const float& fTime)
{
	if(m_pRenderer != NULL)
		m_pRenderer->Render(fTime);
}

void Engine::CManagement::Release(void)
{
	Engine::Safe_Delete(m_pRenderer);  
	Engine::Safe_Delete(m_pScene);
}

const Engine::CComponent* Engine::CManagement::GetComponent(WORD LayerID , const TCHAR* pObjectKey , const TCHAR* pComponentKey)
{
	return m_pScene->GetComponent(LayerID, pObjectKey, pComponentKey);
}

const list<Engine::CGameObject*> Engine::CManagement::GetGameObjectlist(WORD LayerID, const TCHAR* pObjectKey)
{
	return m_pScene->GetGameObjectlist(LayerID, pObjectKey);
}

void Engine::CManagement::AddRenderGroup(RENDERGROUP eRenderID, CGameObject* pGameObject)
{
	m_pRenderer->AddRenderGroup(eRenderID, pGameObject);
}

list<Engine::CGameObject*> Engine::CManagement::GetNotConstGameObjectlist(WORD LayerID, const TCHAR* pObjectKey)
{
	return m_pScene->GetNotConstGameObjectlist(LayerID, pObjectKey);
}

