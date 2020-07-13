#include "stdafx.h"
#include "Include.h"
#include "PlayerHpBar.h"
#include "Transform.h"
#include "Export_Function.h"
#include "Texture.h"
#include "RcTex.h"

CPlayerHpBar::CPlayerHpBar(LPDIRECT3DDEVICE9 pDevice)
: Engine::CGameObject(pDevice)
, m_pResourceMgr(Engine::Get_ResourceMgr())
, m_pInfo(NULL)
, m_pBuffer(NULL)
, m_pTexture(NULL) 
, m_fX(0.f)
, m_fY(0.f)
, m_fSizeX(0.f)
, m_fSizeY(0.f)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
}

CPlayerHpBar::~CPlayerHpBar(void)
{
	Release();
}

HRESULT CPlayerHpBar::Initialize(void)
{
	FAILED_CHECK(AddComponent());

	m_fX = float(200);
	m_fY = float(WINCY - 100);
	m_fSizeX = 300.f;
	m_fSizeY = 20.f;

	m_pUIObserver = CUIObserver::Create();
	Engine::Get_InfoSubject()->Subscribe(m_pUIObserver);

	m_wVtxCnt = 4;
	m_pVertex = new Engine::VTXTEX[m_wVtxCnt];

	m_pBuffer->GetVtxInfo(m_pVertex);
	NULL_CHECK_RETURN(m_pVertex, E_FAIL);

	return S_OK;
}

void CPlayerHpBar::Update(void)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0.f, 1.f);

	m_matView._11 = m_fSizeX;
	m_matView._22 = m_fSizeY;
	m_matView._33 = 1.f;


	m_matView._41 = m_fX - (WINCX >> 1);
	m_matView._42 = -m_fY + (WINCY >> 1);

	RECT	rcUI = { long(m_fX - m_fSizeX * 0.5f)
		, long(m_fY - m_fSizeY * 0.5f)
		, long(m_fX + m_fSizeX * 0.5f)
		, long(m_fY + m_fSizeY * 0.5f) };

	Engine::CGameObject::Update();


}

void CPlayerHpBar::Render(void)
{
	if (Engine::Get_Input()->GetDIKeyState(DIK_P))
		return;

	if ((int)*m_pUIObserver->GetMonsterHp() <= 0)
		return;

	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	D3DXMATRIX		matOldView, matOldProj;
	m_pDevice->GetTransform(D3DTS_VIEW, &matOldView);
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matOldProj);

	m_pDevice->SetTransform(D3DTS_VIEW, &m_matView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pTexture->Render(0, 0);
	m_pBuffer->Render(&m_pInfo->m_matWorld);

	SettingHpBar();

	m_pTexture->Render(0, 1);
	m_pBuffer->Render(&m_pInfo->m_matWorld);

	((Engine::CRcTex*)m_pBuffer)->ReSetBuffer();
	((Engine::CRcTex*)m_pBuffer)->ReSetUV();

	m_pDevice->SetTransform(D3DTS_VIEW, &matOldView);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &matOldProj);

	m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

HRESULT CPlayerHpBar::AddComponent(void)
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
	pComponent = m_pResourceMgr->CloneResource(RESOURCE_STAGE, L"PlayerHpBar");
	m_pTexture = dynamic_cast<Engine::CTexture*>(pComponent);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

void CPlayerHpBar::Release(void)
{
	Safe_Delete_Array(m_pVertex);
	Safe_Delete(m_pUIObserver);
}

void CPlayerHpBar::SettingHpBar(void)
{
	int iHp = (int)*m_pUIObserver->GetPlayerHp();
	int iHpMax = (int)*m_pUIObserver->GetPlayerHpMax();

	if (iHp < 0)
		iHp = 0;

	float fProbability = float(iHp) / float(iHpMax);

	m_pVertex[0].vPos = D3DXVECTOR3(-0.5f, 0.5f, 0.f);
	m_pVertex[1].vPos = D3DXVECTOR3(fProbability - 0.5f, 0.5f, 0.f);
	m_pVertex[2].vPos = D3DXVECTOR3(fProbability - 0.5f, -0.5f, 0.f);
	m_pVertex[3].vPos = D3DXVECTOR3(-0.5f, -0.5f, 0.f);

	m_pVertex[0].vTexUV = D3DXVECTOR2(0.f, 0.f);
	m_pVertex[1].vTexUV = D3DXVECTOR2(fProbability, 0.f);
	m_pVertex[2].vTexUV = D3DXVECTOR2(fProbability, 1.f);
	m_pVertex[3].vTexUV = D3DXVECTOR2(0.f, 1.f);

	m_pBuffer->SetVtxInfo(m_pVertex);
}

CPlayerHpBar * CPlayerHpBar::Create(LPDIRECT3DDEVICE9 pDevice)
{
	CPlayerHpBar*	pGameObject = new CPlayerHpBar(pDevice);
	if (FAILED(pGameObject->Initialize()))
		Engine::Safe_Delete(pGameObject);

	return pGameObject;
}
