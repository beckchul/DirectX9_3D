#include "stdafx.h"
#include "ManaScreen.h"

#include "Include.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"

CManaScreen::CManaScreen(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pInfo(NULL)
, m_pBuffer(NULL)
, m_pTexture(NULL)
, m_fX(0.f)
, m_fY(0.f)
, m_fSizeX(0.f)
, m_fSizeY(0.f)
, m_fTime(0.f)
, m_fCount(2.f)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

CManaScreen::~CManaScreen(void)
{

}

HRESULT CManaScreen::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_fX = float(WINCX / 2);
	m_fY = float(WINCY / 2);
	m_fSizeX = WINCX;
	m_fSizeY = WINCY;

	return S_OK;
}

void CManaScreen::Update(void)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_matView._11 = m_fSizeX;
	m_matView._22 = m_fSizeY;
	m_matView._33 = 1.f;

	//x 0 -> -400		800 -> 400
	m_matView._41 = m_fX - (WINCX >> 1);
	m_matView._42 = -m_fY + (WINCY >> 1);

	RECT	rcManaScreen = { long(m_fX - m_fSizeX * 0.5f)
		, long(m_fY - m_fSizeY * 0.5f)
		, long(m_fX + m_fSizeX * 0.5f)
		, long(m_fY + m_fSizeY * 0.5f) };

	Engine::CGameObject::Update();

	m_fTime += Engine::Get_TimeMgr()->GetTime() * 3.f;

	if (m_fTime >= 1.f)
	{
		m_fTime = 0.f;
		++m_fCount;
	}
}

void CManaScreen::Render(void)
{
	if (m_fCount >= 1.f)
		return;

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	D3DXMATRIX		matOldView, matOldProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matOldView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matOldProj);

	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);


	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	m_pTexture->Render(0, 0);
	m_pBuffer->Render(&m_pInfo->m_matWorld);

	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	m_pDevice->SetTransform(D3DTS_VIEW, &matOldView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matOldProj);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

HRESULT CManaScreen::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform
	pComponent = m_pInfo = Engine::CTransform::Create(g_vLook);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	//Buffer
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STATIC, L"Buffer_RcTex");
	m_pBuffer = dynamic_cast<Engine::CVIBuffer*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	//Texture
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"ManaScreen");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

CManaScreen* CManaScreen::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CManaScreen*		pGameObject = new CManaScreen(pDevice);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}