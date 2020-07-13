#include "Renderer.h"
#include "Scene.h"
#include "GameObject.h"
#include "Export_Function.h"

Engine::CRenderer::CRenderer(LPDIRECT3DDEVICE9 pDevice)
: m_pDevice(pDevice)
, m_fTime(0.f)
, m_dwCount(0)
{
	ZeroMemory(m_szFps, sizeof(TCHAR) * 128);
}

Engine::CRenderer::~CRenderer(void)
{
	Release();
}

void Engine::CRenderer::SetCurrentScene(CScene* pScene) {m_pScene = pScene;}

HRESULT Engine::CRenderer::InitScene(void)
{
	return S_OK;
}

void Engine::CRenderer::Render(const float& fTime)
{
	m_pDevice->Clear(0, NULL
		, D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
		, D3DCOLOR_XRGB(0, 0, 0), 1.f, 0);
	m_pDevice->BeginScene();

	Render_Priority();
	Render_NoneAlpha();
	Render_Alpha();
	Render_UI();

	if(m_pScene != NULL)
		m_pScene->Render();

	//Render_FPS(fTime);

	m_pDevice->EndScene();
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}

Engine::CRenderer* Engine::CRenderer::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CRenderer* pRenderer = new CRenderer(pDevice);
	if(FAILED(pRenderer->InitScene()))
	{
		Engine::Safe_Delete(pRenderer);
	}
	return pRenderer;
}

void Engine::CRenderer::AddRenderGroup(RENDERGROUP eRenderID, CGameObject* pGameObject)
{
	if(pGameObject == NULL)
		return ;

	m_RenderGroup[eRenderID].push_back(pGameObject);
}

void Engine::CRenderer::Render_Priority(void)
{
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	RENDERLIST::iterator iter = m_RenderGroup[TYPE_PRIORITY].begin();
	RENDERLIST::iterator iter_end = m_RenderGroup[TYPE_PRIORITY].end();

	for(; iter != iter_end; ++iter)
		(*iter)->Render();

	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void Engine::CRenderer::Render_NoneAlpha(void)
{
	RENDERLIST::iterator iter = m_RenderGroup[TYPE_NONEALPHA].begin();
	RENDERLIST::iterator iter_end = m_RenderGroup[TYPE_NONEALPHA].end();

	for(; iter != iter_end; ++iter)
		(*iter)->Render();
}

bool Compare_ViewZ(Engine::CGameObject* pSour, Engine::CGameObject* pDest)
{
	return pSour->GetViewZ() > pDest->GetViewZ();
}

void Engine::CRenderer::Render_Alpha(void)
{
	m_RenderGroup[TYPE_ALPHA].sort(Compare_ViewZ);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	////m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	////m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	////m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);

	////m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
	////m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);

	RENDERLIST::iterator iter = m_RenderGroup[TYPE_ALPHA].begin();
	RENDERLIST::iterator iter_end = m_RenderGroup[TYPE_ALPHA].end();

	for(; iter != iter_end; ++iter)
		(*iter)->Render();

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void Engine::CRenderer::Render_UI(void)
{
	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	RENDERLIST::iterator iter = m_RenderGroup[TYPE_UI].begin();
	RENDERLIST::iterator iter_end = m_RenderGroup[TYPE_UI].end();

	for(; iter != iter_end; ++iter)
		(*iter)->Render();

	m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

void Engine::CRenderer::Render_FPS(const float& fTime)
{
	m_fTime += fTime;
	++m_dwCount;

	if(m_fTime >= 1.f)
	{
		wsprintf(m_szFps, L"FPS : %d", m_dwCount);
		m_fTime = 0.f;
		m_dwCount = 0;
	}

	Engine::Get_FontMgr()->Render(L"πŸ≈¡", D3DXVECTOR3(0.f, 0.f, 0.f)
		, m_szFps, D3DCOLOR_ARGB(255, 255, 0, 0));
}

void Engine::CRenderer::Release(void)
{
	for(int i = 0; i < TYPE_END; ++i)
		m_RenderGroup[i].clear();
}
